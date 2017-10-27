/**************************************************************************/
/*  TIGER - Hydro-thermal sImulator GEothermal Reservoirs                 */
/*                                                                        */
/*  Copyright (C) 2017 by Maziar Gholami Korzani                          */
/*  Karlsruhe Institute of Technology, Institute of Applied Geosciences   */
/*  Division of Geothermal Research                                       */
/*                                                                        */
/*  This file is part of TIGER App                                        */
/*                                                                        */
/*  This program is free software: you can redistribute it and/or modify  */
/*  it under the terms of the GNU General Public License as published by  */
/*  the Free Software Foundation, either version 3 of the License, or     */
/*  (at your option) any later version.                                   */
/*                                                                        */
/*  This program is distributed in the hope that it will be useful,       */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of        */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          */
/*  GNU General Public License for more details.                          */
/*                                                                        */
/*  You should have received a copy of the GNU General Public License     */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>  */
/**************************************************************************/

#include "TigerAdvectionKernelT.h"

template <>
InputParameters
validParams<TigerAdvectionKernelT>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar("gradient_variable", "Variable name for pressure field");
  return params;
}

TigerAdvectionKernelT::TigerAdvectionKernelT(const InputParameters & parameters)
  : Kernel(parameters),
    _rho_f(getMaterialProperty<Real>("fluid_density")),
    _rho_s(getMaterialProperty<Real>("solid_density")),
    _cp_f(getMaterialProperty<Real>("fluid_specific_heat")),
    _cp_s(getMaterialProperty<Real>("solid_specific_heat")),
    _n(getMaterialProperty<Real>("porosity")),
    _gradient_pore_pressure(coupledGradient("gradient_variable")),
    _kf(getMaterialProperty<RankTwoTensor>("permeability_tensor")),
    _viscosity(getMaterialProperty<Real>("viscosity")),
    _gravity(getMaterialProperty<RealVectorValue>("gravity_vector"))
{
}


Real
TigerAdvectionKernelT::computeQpResidual()
{
  Real _dt_coeff = 0.0;
  if (_fe_problem.isTransient())
    _dt_coeff = 1.0/( (1-_n[_qp])*_rho_s[_qp]*_cp_s[_qp] + _rho_f[_qp]*_cp_f[_qp]*_n[_qp]);
  else
    _dt_coeff = 1.0;

  RealVectorValue _Darcy_Vel = (_kf[_qp]/_viscosity[_qp]) * (_gradient_pore_pressure[_qp]-_rho_f[_qp]*_gravity[_qp]);

  return _dt_coeff * _rho_f[_qp]*_cp_f[_qp] * _test[_i][_qp] * (_Darcy_Vel * _grad_u[_qp]);
}

Real
TigerAdvectionKernelT::computeQpJacobian()
{
  Real _dt_coeff = 0.0;
  if (_fe_problem.isTransient())
    _dt_coeff = 1.0/( (1-_n[_qp])*_rho_s[_qp]*_cp_s[_qp] + _rho_f[_qp]*_cp_f[_qp]*_n[_qp]);
  else
    _dt_coeff = 1.0;

  RealVectorValue _Darcy_Vel = (_kf[_qp]/_viscosity[_qp]) * _gradient_pore_pressure[_qp];

  return _dt_coeff * _rho_f[_qp]*_cp_f[_qp] * _test[_i][_qp] * (_Darcy_Vel * _grad_phi[_j][_qp]);
}
