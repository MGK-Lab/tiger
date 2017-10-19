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

#include "TigerKernelH.h"

template <>
InputParameters
validParams<TigerKernelH>()
{
  InputParameters params = validParams<Kernel>();
  return params;
}

TigerKernelH::TigerKernelH(const InputParameters & parameters)
  : Kernel(parameters),
    _k(getMaterialProperty<RankTwoTensor>("permeability")),
    _fluid_viscosity(getMaterialProperty<Real>("viscosity")),
    _solid_porosity(getMaterialProperty<Real>("porosity")),
    _fluid_compressibility(getMaterialProperty<Real>("fluid_compressibility")),
    _solid_compressibility(getMaterialProperty<Real>("solid_compressibility"))
{
}

Real
TigerKernelH::computeQpResidual()
{
  Real _dt_coeff = 0.0;
  if (_fe_problem.isTransient())
    _dt_coeff = -1.0/(_solid_compressibility[_qp] +  _fluid_compressibility[_qp] * _solid_porosity[_qp]);
  else
    _dt_coeff = -1.0;

  return _dt_coeff * _grad_test[_i][_qp] * ( (_k[_qp]/_fluid_viscosity[_qp]) * _grad_u[_qp]);
}

Real
TigerKernelH::computeQpJacobian()
{
  Real _dt_coeff = 0.0;
  if (_fe_problem.isTransient())
    _dt_coeff = -1.0/(_solid_compressibility[_qp] +  _fluid_compressibility[_qp] * _solid_porosity[_qp]);
  else
    _dt_coeff = -1.0;

  return _dt_coeff * _grad_test[_i][_qp] * ( (_k[_qp]/_fluid_viscosity[_qp]) * _grad_phi[_j][_qp]);
}
