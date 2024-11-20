#pragma once

#include "IsotropicPlasticityStressUpdate.h"
#include "MooseMesh.h"
// scalar d delta p
template <bool is_ad>
class LudwikHardeningStressUpdateTempl : public IsotropicPlasticityStressUpdateTempl<is_ad>
{
public:
  static InputParameters validParams();

  LudwikHardeningStressUpdateTempl(const InputParameters & parameters);

  using Material::_qp;
  using RadialReturnStressUpdateTempl<is_ad>::_three_shear_modulus;
  using IsotropicPlasticityStressUpdateTempl<is_ad>::computeYieldStress;
  using RadialReturnStressUpdateTempl<is_ad>::_base_name;

protected:
  virtual void
  computeStressInitialize(const GenericReal<is_ad> & effective_trial_stress,
                          const GenericRankFourTensor<is_ad> & elasticity_tensor) override;
  virtual GenericReal<is_ad> computeHardeningValue(const GenericReal<is_ad> & scalar);
  virtual GenericReal<is_ad> computeHardeningDerivative(const GenericReal<is_ad> & scalar) override;

  // void computeYoungsModulus(const GenericRankFourTensor<is_ad> & elasticity_tensor);
  ///@{ Power law hardening coefficients
  Real _yield_stress;
  const MaterialProperty<RankFourTensor> & _elasticity_tensor;
  /// Elastic constants
  Real _K;
  Real _strain_hardening_exponent;
  GenericReal<is_ad> _youngs_modulus;
  GenericReal<is_ad> _effective_trial_stress;
  void computeYoungsModulus();
  GenericReal<is_ad> getIsotropicLameLambda(const GenericRankFourTensor<is_ad> & elasticity_tensor);
  ///@}
};

typedef LudwikHardeningStressUpdateTempl<false> LudwikHardeningStressUpdate;
typedef LudwikHardeningStressUpdateTempl<true> ADLudwikHardeningStressUpdate;
