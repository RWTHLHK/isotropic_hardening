#pragma once

#include "ComputeLinearElasticPFFractureStress.h"
#include "MooseEnum.h"
#include "ComputeMultipleInelasticStress.h"
class ComputePFCFStress : public ComputeMultipleInelasticStress
{
public:
  static InputParameters validParams();
  ComputePFCFStress(const InputParameters & parameters);

protected:
  void initialSetup() override;
  void UpdateElasticityTensor();
};
