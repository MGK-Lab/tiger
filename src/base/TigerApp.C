#include "TigerApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

//Kernels
#include "TigerKernelH.h"
#include "TigerDiffusionKernelT.h"
#include "TigerAdvectionKernelTH.h"
#include "TigerPointSourceH.h"
#include "TigerTimeDerivativeH.h"
#include "TigerTimeDerivativeT.h"
#include "TigerHeatSourceT.h"

//AuxKernels
#include "TigerDarcyVelocityComponent.h"
#include "test.h"

//Material
#include "TigerRockMaterialH.h"

#include "TigerPermeability.h"
#include "TigerPermeabilityConst.h"
#include "TigerPermeabilityCubicLaw.h"

//Boundary Condition
#include "TigerHeatConductionOutflowT.h"


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
  Registry::registerObjectsTo(_factory, {type()});

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
  registerMaterial(TigerRockMaterialH);

  registerUserObject(TigerPermeabilityConst);
  registerUserObject(TigerPermeabilityCubicLaw);

  registerKernel(TigerKernelH);
  registerKernel(TigerDiffusionKernelT);
  registerKernel(TigerAdvectionKernelTH);
  registerKernel(TigerTimeDerivativeH);
  registerKernel(TigerTimeDerivativeT);
  registerKernel(TigerHeatSourceT);

  registerAux(TigerDarcyVelocityComponent);
  registerAux(test);

  registerDiracKernel(TigerPointSourceH);

  registerBoundaryCondition(TigerHeatConductionOutflowT);
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
