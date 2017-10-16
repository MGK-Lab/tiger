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
  params.addParam<Real>("density", 1000.0, "doc");
  params.addParam<Real>("viscosity", 1.0e-3, "doc");
  params.addClassDescription("Fluid properties using the (pressure, temperature) formulation");
  return params;
}

TigerFluidMaterialTP::TigerFluidMaterialTP(const InputParameters & parameters)
  : Material(parameters),
    _density(getParam<Real>("density")),
    _viscosity(getParam<Real>("viscosity")),
    _rho(declareProperty<Real>("density")),
    _mu(declareProperty<Real>("viscosity"))
{
}

void
TigerFluidMaterialTP::computeQpProperties()
{
  _rho[_qp] = _density;
  _mu [_qp] = _viscosity;
}
