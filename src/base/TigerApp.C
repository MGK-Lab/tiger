#include "TigerApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

//Material
#include "TigerFluidMaterialTP.h"

template <>
InputParameters
validParams<TigerApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

TigerApp::TigerApp(InputParameters parameters) : MooseApp(parameters)
{
  Moose::registerObjects(_factory);
  ModulesApp::registerObjects(_factory);
  TigerApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  ModulesApp::associateSyntax(_syntax, _action_factory);
  TigerApp::associateSyntax(_syntax, _action_factory);
}

TigerApp::~TigerApp() {}

// External entry point for dynamic application loading
extern "C" void
TigerApp__registerApps()
{
  TigerApp::registerApps();
}
void
TigerApp::registerApps()
{
  registerApp(TigerApp);
}

void
TigerApp::registerObjectDepends(Factory & /*factory*/)
{
}

// External entry point for dynamic object registration
extern "C" void
TigerApp__registerObjects(Factory & factory)
{
  TigerApp::registerObjects(factory);
}
void
TigerApp::registerObjects(Factory & factory)
{
  registerMaterial(TigerFluidMaterialTP);
}

void
TigerApp::associateSyntaxDepends(Syntax & /*syntax*/, ActionFactory & /*action_factory*/)
{
}

// External entry point for dynamic syntax association
extern "C" void
TigerApp__associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
  TigerApp::associateSyntax(syntax, action_factory);
}
void
TigerApp::associateSyntax(Syntax & /*syntax*/, ActionFactory & /*action_factory*/)
{
}
