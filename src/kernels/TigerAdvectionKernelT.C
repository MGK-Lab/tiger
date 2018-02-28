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
  params.addParam<UserObjectName>("supg_uo", "The name of the Streamline Upwinding (SU/PG) user object.");
  return params;
}

TigerAdvectionKernelT::TigerAdvectionKernelT(const InputParameters & parameters)
  : Kernel(parameters),
    _has_SUPG_upwind(isParamValid("supg_uo") ? true : false),
    _supg_uo(_has_SUPG_upwind ? &getUserObject<TigerSU_PG>("supg_uo") : NULL),
    _lambda_sf_eq(getMaterialProperty<Real>("conductivity_mixture_equivalent")),
    _rho_cp_f(getMaterialProperty<Real>("fluid_thermal_capacity")),
    _gradient_pore_press(coupledGradient("gradient_variable")),
    _k_vis(getMaterialProperty<RankTwoTensor>("permeability_by_viscosity")),
    _rhof_g(getMaterialProperty<RealVectorValue>("rho_times_gravity"))
{
}


Real
TigerAdvectionKernelT::computeQpResidual()
{
  RealVectorValue _dv = -_k_vis[_qp] * (_gradient_pore_press[_qp] - _rhof_g[_qp]);
  Real _SUPG_residue = 0.0;

  if (_has_SUPG_upwind && _dv.norm()!=0.0)
    _SUPG_residue = _supg_uo->k_bar(_dv, _lambda_sf_eq[_qp], 0.0, _current_elem) * _dv * _grad_test[_i][_qp] / _dv.norm_sq();

  return _rho_cp_f[_qp] * ((_test[_i][_qp] + _SUPG_residue) * ( _dv * _grad_u[_qp]));
}

Real
TigerAdvectionKernelT::computeQpJacobian()
{
  RealVectorValue _dv = -_k_vis[_qp] * (_gradient_pore_press[_qp] - _rhof_g[_qp]);
  Real _SUPG_residue = 0.0;

  if (_has_SUPG_upwind && _dv.norm()!=0.0)
    _SUPG_residue = _supg_uo->k_bar(_dv, _lambda_sf_eq[_qp], 0.0, _current_elem) * _dv * _grad_test[_i][_qp] / _dv.norm_sq();

  return _rho_cp_f[_qp] * ((_test[_i][_qp] + _SUPG_residue) * ( _dv * _grad_phi[_j][_qp]));
}
