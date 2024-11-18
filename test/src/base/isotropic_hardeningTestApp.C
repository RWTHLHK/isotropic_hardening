//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "isotropic_hardeningTestApp.h"
#include "isotropic_hardeningApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"

InputParameters
isotropic_hardeningTestApp::validParams()
{
  InputParameters params = isotropic_hardeningApp::validParams();
  params.set<bool>("use_legacy_material_output") = false;
  params.set<bool>("use_legacy_initial_residual_evaluation_behavior") = false;
  return params;
}

isotropic_hardeningTestApp::isotropic_hardeningTestApp(InputParameters parameters) : MooseApp(parameters)
{
  isotropic_hardeningTestApp::registerAll(
      _factory, _action_factory, _syntax, getParam<bool>("allow_test_objects"));
}

isotropic_hardeningTestApp::~isotropic_hardeningTestApp() {}

void
isotropic_hardeningTestApp::registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs)
{
  isotropic_hardeningApp::registerAll(f, af, s);
  if (use_test_objs)
  {
    Registry::registerObjectsTo(f, {"isotropic_hardeningTestApp"});
    Registry::registerActionsTo(af, {"isotropic_hardeningTestApp"});
  }
}

void
isotropic_hardeningTestApp::registerApps()
{
  registerApp(isotropic_hardeningApp);
  registerApp(isotropic_hardeningTestApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
isotropic_hardeningTestApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  isotropic_hardeningTestApp::registerAll(f, af, s);
}
extern "C" void
isotropic_hardeningTestApp__registerApps()
{
  isotropic_hardeningTestApp::registerApps();
}
