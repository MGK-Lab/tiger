#include "TigerTestApp.h"
#include "TigerApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"

template <>
InputParameters
validParams<TigerTestApp>()
{
  InputParameters params = validParams<TigerApp>();
  return params;
}

TigerTestApp::TigerTestApp(InputParameters parameters) : MooseApp(parameters)
{
  Moose::registerObjects(_factory);
  TigerApp::registerObjectDepends(_factory);
  TigerApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  TigerApp::associateSyntaxDepends(_syntax, _action_factory);
  TigerApp::associateSyntax(_syntax, _action_factory);

  bool use_test_objs = getParam<bool>("allow_test_objects");
  if (use_test_objs)
  {
    TigerTestApp::registerObjects(_factory);
    TigerTestApp::associateSyntax(_syntax, _action_factory);
  }
}

TigerTestApp::~TigerTestApp() {}

// External entry point for dynamic application loading
extern "C" void
TigerTestApp__registerApps()
{
  TigerTestApp::registerApps();
}
void
TigerTestApp::registerApps()
{
  registerApp(TigerApp);
  registerApp(TigerTestApp);
}

// External entry point for dynamic object registration
extern "C" void
TigerTestApp__registerObjects(Factory & factory)
{
  TigerTestApp::registerObjects(factory);
}
void
TigerTestApp::registerObjects(Factory & /*factory*/)
{
}

// External entry point for dynamic syntax association
extern "C" void
TigerTestApp__associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
  TigerTestApp::associateSyntax(syntax, action_factory);
}
void
TigerTestApp::associateSyntax(Syntax & /*syntax*/, ActionFactory & /*action_factory*/)
{
}
