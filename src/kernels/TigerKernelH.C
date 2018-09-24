/**************************************************************************/
/*  TIGER - THMC sImulator for GEoscience Research                        */
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
    _scaling_lowerD(getMaterialProperty<Real>("lowerD_scale_factor_h")),
    _k_vis(getMaterialProperty<RankTwoTensor>("permeability_by_viscosity")),
    _rhof_g(getMaterialProperty<RealVectorValue>("rho_times_gravity"))
{
}

Real
TigerKernelH::computeQpResidual()
{
  return _grad_test[_i][_qp] * (_scaling_lowerD[_qp] * _k_vis[_qp] * ( _grad_u[_qp] - _rhof_g[_qp] ) );
}

Real
TigerKernelH::computeQpJacobian()
{
  return _grad_test[_i][_qp] * (_scaling_lowerD[_qp] *  _k_vis[_qp] * _grad_phi[_j][_qp] );
}
