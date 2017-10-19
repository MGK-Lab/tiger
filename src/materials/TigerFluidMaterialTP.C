/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/

#include "TigerFluidMaterialTP.h"

template <>
InputParameters
validParams<TigerFluidMaterialTP>()
{
  InputParameters params = validParams<Material>();
  params.addRequiredCoupledVar("pressure", "Fluid pressure (Pa)");
  params.addRequiredCoupledVar("temperature", "Fluid temperature (C)");
  params.addRequiredParam<UserObjectName>("fp_UO", "The name of the user object for fluid properties");
  params.addClassDescription("Fluid properties using the (pressure, temperature) formulation");
  return params;
}

TigerFluidMaterialTP::TigerFluidMaterialTP(const InputParameters & parameters)
  : Material(parameters),
    _pressure(coupledValue("pressure")),
    _temperature(coupledValue("temperature")),
    _rho(declareProperty<Real>("density")),
    _mu(declareProperty<Real>("viscosity")),
    _beta(declareProperty<Real>("fluid_compressibility")),
    _fluid_properties_UO(getUserObject<TigerFluidPropertiesTP>("fp_UO"))
{
}

void
TigerFluidMaterialTP::computeQpProperties()
{
  _rho[_qp] = _fluid_properties_UO.rho (_pressure[_qp], _temperature[_qp]);
  _mu [_qp] = _fluid_properties_UO.mu  (_temperature[_qp]);
  _beta[_qp]= _fluid_properties_UO.beta(_pressure[_qp], _temperature[_qp]);
}
