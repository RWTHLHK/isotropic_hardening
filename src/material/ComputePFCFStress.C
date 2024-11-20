//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ComputePFCFStress.h"
#include "MathUtils.h"
#include "libmesh/utility.h"

registerMooseObject("SolidMechanicsApp", ComputePFCFStress);

InputParameters
ComputePFCFStress::validParams()
{
  InputParameters params = ComputeMultipleInelasticStress::validParams();
  params += ComputeLinearElasticPFFractureStres::validParams();
  params.addClassDescription("phase field brittle fracture coupled with palsticity");
  return params;
}

ComputePFCFStress::ComputePFCFStress(const InputParameters & parameters)
  : ComputeMultipleInelasticStress(parameters), ComputeLinearElasticPFFractureStres(parameters)
{
}

void
ComputePFCFStress::initialSetup()
{
  if ((ComputeLinearElasticPFFractureStress::_decomposition_type ==
           Decomposition_type::strain_vol_dev ||
       ComputeLinearElasticPFFractureStress::_decomposition_type ==
           Decomposition_type::strain_spectral) &&
      !ComputeLinearElasticPFFractureStress::hasGuaranteedMaterialProperty(_elasticity_tensor_name,
                                                                           Guarantee::ISOTROPIC))
    ComputeLinearElasticPFFractureStress::mooseError(
        "Decomposition approach of strain_vol_dev and strain_spectral can only be used with "
        "isotropic elasticity tensor materials, use stress_spectral for anistropic "
        "elasticity tensor materials");
}

void
ComputePFCFStress::UpdateElasticityTensor()
{
}
