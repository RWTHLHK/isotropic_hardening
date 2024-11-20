#include "LudwikHardeningStressUpdate.h"
#include "ElasticityTensorTools.h"

registerMooseObject("SolidMechanicsApp", LudwikHardeningStressUpdate);
registerMooseObject("SolidMechanicsApp", ADLudwikHardeningStressUpdate);

template <bool is_ad>
InputParameters
LudwikHardeningStressUpdateTempl<is_ad>::validParams()
{
  InputParameters params = IsotropicPlasticityStressUpdateTempl<is_ad>::validParams();
  params.addClassDescription("This class uses the discrete material in a radial return isotropic "
                             "plasticity power law hardening model, solving for the yield stress "
                             "as the intersection of the power law relation curve and Hooke's law. "
                             " This class can be used in conjunction with other creep and "
                             "plasticity materials for more complex simulations.");

  // Set and Suppress parameters to enable calculation of the yield stress
  params.addRequiredParam<Real>("yield_stress",
                                "The point at which plastic strain begins accumulating");
  params.set<Real>("hardening_constant") = 1.0;
  params.suppressParameter<Real>("hardening_constant");

  // Power law hardening specific parameters
  params.addRequiredParam<Real>("strength_coefficient",
                                "The strength coefficient (K) for power law hardening");
  params.addRequiredRangeCheckedParam<Real>(
      "strain_hardening_exponent",
      "strain_hardening_exponent>=0.0 & strain_hardening_exponent <=1.0",
      "The strain hardening exponent (n) for power law hardening");

  return params;
}

template <bool is_ad>
LudwikHardeningStressUpdateTempl<is_ad>::LudwikHardeningStressUpdateTempl(
    const InputParameters & parameters)
  : IsotropicPlasticityStressUpdateTempl<is_ad>(parameters),
    _yield_stress(parameters.get<Real>("yield_stress")),
    _elasticity_tensor(
        this->template getMaterialProperty<RankFourTensor>(_base_name + "elasticity_tensor")),
    _K(parameters.get<Real>("strength_coefficient")),
    _strain_hardening_exponent(parameters.get<Real>("strain_hardening_exponent"))
{
}

template <bool is_ad>
GenericReal<is_ad>
LudwikHardeningStressUpdateTempl<is_ad>::computeHardeningValue(const GenericReal<is_ad> & scalar)
{
  const Real strain_old = this->_effective_inelastic_strain_old[_qp];
  return _K * std::pow(strain_old + scalar, _strain_hardening_exponent);
}

template <bool is_ad>
GenericReal<is_ad>
LudwikHardeningStressUpdateTempl<is_ad>::computeHardeningDerivative(
    const GenericReal<is_ad> & scalar)
{
  computeYoungsModulus();
  const GenericReal<is_ad> stress_delta = _effective_trial_stress - _three_shear_modulus * scalar;
  GenericReal<is_ad> slope = std::pow(stress_delta, (1.0 / _strain_hardening_exponent - 1.0)) /
                             _strain_hardening_exponent * 1.0 /
                             std::pow(_K, 1.0 / _strain_hardening_exponent);
  slope -= 1.0 / _youngs_modulus;

  return 1.0 / slope;
}

template <bool is_ad>
void
LudwikHardeningStressUpdateTempl<is_ad>::computeStressInitialize(
    const GenericReal<is_ad> & effective_trial_stress,
    const GenericRankFourTensor<is_ad> & elasticity_tensor)
{
  RadialReturnStressUpdateTempl<is_ad>::computeStressInitialize(effective_trial_stress,
                                                                elasticity_tensor);

  computeYieldStress(elasticity_tensor);

  _effective_trial_stress = effective_trial_stress;
  this->_yield_condition =
      effective_trial_stress - this->_hardening_variable_old[_qp] - this->_yield_stress;

  this->_hardening_variable[_qp] = this->_hardening_variable_old[_qp];
  this->_plastic_strain[_qp] = this->_plastic_strain_old[_qp];
}

template <bool is_ad>
void
LudwikHardeningStressUpdateTempl<is_ad>::computeYoungsModulus()
{
  // Pull in the Lam\`{e} lambda, and caculate E
  const GenericReal<is_ad> lambda = getIsotropicLameLambda(_elasticity_tensor[_qp]);
  const GenericReal<is_ad> shear_modulus = _three_shear_modulus / 3.0;

  _youngs_modulus = shear_modulus * (3.0 * lambda + 2 * shear_modulus) / (lambda + shear_modulus);
}

template <bool is_ad>
GenericReal<is_ad>
LudwikHardeningStressUpdateTempl<is_ad>::getIsotropicLameLambda(
    const GenericRankFourTensor<is_ad> & elasticity_tensor)
{
  const GenericReal<is_ad> lame_lambda = elasticity_tensor(0, 0, 1, 1);

  if (this->_mesh.dimension() == 3 &&
      MetaPhysicL::raw_value(lame_lambda) != MetaPhysicL::raw_value(elasticity_tensor(1, 1, 2, 2)))
    mooseError(
        "Check to ensure that your Elasticity Tensor is truly Isotropic: different lambda values");
  return lame_lambda;
}

template class LudwikHardeningStressUpdateTempl<false>;
template class LudwikHardeningStressUpdateTempl<true>;
