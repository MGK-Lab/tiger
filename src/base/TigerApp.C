#include "TigerApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

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

  Moose::registerExecFlags(_factory);
  ModulesApp::registerExecFlags(_factory);
  TigerApp::registerExecFlags(_factory);
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
    Registry::registerObjectsTo(factory, {"TigerApp"});
}

void
TigerApp::associateSyntaxDepends(Syntax & /*syntax*/, ActionFactory & /*action_factory*/)
{
}

void
TigerApp::registerExecFlags(Factory & /*factory*/)
{
  /* Uncomment Factory parameter and register your new execution flags here! */
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/




extern "C" void
TigerApp__associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
  TigerApp::associateSyntax(syntax, action_factory);
}

void
TigerApp::associateSyntax(Syntax & /*syntax*/, ActionFactory & action_factory)
{
  Registry::registerActionsTo(action_factory, {"TigerApp"});

  /* Uncomment Syntax parameter and register your new production objects here! */
}

extern "C" void
TigerApp__registerExecFlags(Factory & factory)
{
  TigerApp::registerExecFlags(factory);
}
