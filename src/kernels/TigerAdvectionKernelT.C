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
    _rho_cp_f(getMaterialProperty<Real>("fluid_thermal_capacity")),
    _gradient_pore_press(coupledGradient("gradient_variable")),
    _T_Kernel_dt(getMaterialProperty<Real>("T_Kernel_dt_coefficient")),
    _k_vis(getMaterialProperty<RankTwoTensor>("permeability_by_viscosity")),
    _rhof_g(getMaterialProperty<RealVectorValue>("rho_times_gravity"))
{
  _dt_coeff = 1.0;
}


Real
TigerAdvectionKernelT::computeQpResidual()
{
  if (_fe_problem.isTransient())
    _dt_coeff = 1.0/_T_Kernel_dt[_qp];

  RealVectorValue _dv = -_k_vis[_qp] * (_gradient_pore_press[_qp] - _rhof_g[_qp]);

  return _dt_coeff * _rho_cp_f[_qp] * (_test[_i][_qp] * ( _dv * _grad_u[_qp]));
}

Real
TigerAdvectionKernelT::computeQpJacobian()
{
  if (_fe_problem.isTransient())
    _dt_coeff = 1.0/_T_Kernel_dt[_qp];

  RealVectorValue _dv = -_k_vis[_qp] * (_gradient_pore_press[_qp] - _rhof_g[_qp]);

  return _dt_coeff * _rho_cp_f[_qp] * (_test[_i][_qp] * ( _dv * _grad_phi[_j][_qp]));
}
