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

#include "TigerCoupledDirichletBC.h"

registerMooseObject("TigerApp", TigerCoupledDirichletBC2);

template <>
InputParameters
validParams<TigerCoupledDirichletBC2>()
{
  // InputParameters params = validParams<IntegratedBC>();
  InputParameters params = validParams<NodalBC>();
  params.addRequiredCoupledVar("coupled_var", "Value on the Boundary");
  return params;
}

TigerCoupledDirichletBC2::TigerCoupledDirichletBC2(const InputParameters & parameters)
  : NodalBC(parameters),

  _coupled_var(coupledValue("coupled_var")),
  _coupled_var_number(coupled("coupled_var"))
  // _k_vis(getMaterialProperty<RankTwoTensor>("permeability_by_viscosity"))
{
}

Real
TigerCoupledDirichletBC2::computeQpResidual()
{
  // return -_test[_i][_qp] * _k_vis[_qp] * _grad_u[_qp] * _normals[_qp];
  return _u[_qp] - _coupled_var[_qp];
}

Real
TigerCoupledDirichletBC2::computeQpJacobian()
{
  return 1;
}

Real
TigerCoupledDirichletBC2::computeQpOffDiagJacobian(unsigned int jvar)
{

if (jvar == _coupled_var_number)
{
  return -1;
}
else
return 0;
}
