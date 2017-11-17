/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "TigerHeatSourceT.h"

// MOOSE
#include "Function.h"

template <>
InputParameters
validParams<TigerHeatSourceT>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<Real>("value", 1.0, "Constant heat source (sink) (W/m^3) (positive is a source, and negative is a sink) or a multiplier for the the provided function");
  params.addParam<FunctionName>("function", "1", "Heat source (sink) as a function (W/m^3) (positive is a source, and negative is a sink)");
  return params;
}

TigerHeatSourceT::TigerHeatSourceT(const InputParameters & parameters)
  : Kernel(parameters),
    _scale(getParam<Real>("value")),
    _function(getFunction("function"))
{
}

Real
TigerHeatSourceT::computeQpResidual()
{
  Real factor = _scale * _function.value(_t, _q_point[_qp]);
  return _test[_i][_qp] * -factor;
}
