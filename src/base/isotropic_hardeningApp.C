#include "isotropic_hardeningApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

InputParameters
isotropic_hardeningApp::validParams()
{
  InputParameters params = MooseApp::validParams();
  params.set<bool>("use_legacy_material_output") = false;
  params.set<bool>("use_legacy_initial_residual_evaluation_behavior") = false;
  return params;
}

isotropic_hardeningApp::isotropic_hardeningApp(InputParameters parameters) : MooseApp(parameters)
{
  isotropic_hardeningApp::registerAll(_factory, _action_factory, _syntax);
}

isotropic_hardeningApp::~isotropic_hardeningApp() {}

void
isotropic_hardeningApp::registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  ModulesApp::registerAllObjects<isotropic_hardeningApp>(f, af, s);
  Registry::registerObjectsTo(f, {"isotropic_hardeningApp"});
  Registry::registerActionsTo(af, {"isotropic_hardeningApp"});

  /* register custom execute flags, action syntax, etc. here */
}

void
isotropic_hardeningApp::registerApps()
{
  registerApp(isotropic_hardeningApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
isotropic_hardeningApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  isotropic_hardeningApp::registerAll(f, af, s);
}
extern "C" void
isotropic_hardeningApp__registerApps()
{
  isotropic_hardeningApp::registerApps();
}
