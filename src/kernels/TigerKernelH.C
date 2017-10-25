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
    _k(getMaterialProperty<RankTwoTensor>("permeability_tensor")),
    _mu(getMaterialProperty<Real>("viscosity")),
    _rho_f(getMaterialProperty<Real>("fluid_density")),
    _n(getMaterialProperty<Real>("porosity")),
    _beta_f(getMaterialProperty<Real>("fluid_compressibility")),
    _beta_s(getMaterialProperty<Real>("solid_compressibility")),
    _gravity(getMaterialProperty<RealVectorValue>("gravity_vector"))
{
}


Real
TigerKernelH::computeQpResidual()
{
  Real _dt_coeff = 0.0;
  if (_fe_problem.isTransient())
    _dt_coeff = -1.0/(_beta_s[_qp] +  _beta_f[_qp] * _n[_qp]);
  else
    _dt_coeff = -1.0;

  return _dt_coeff * _grad_test[_i][_qp] * ( (_k[_qp]/_mu[_qp]) * ( _grad_u[_qp] - _rho_f[_qp] * _gravity[_qp] ) );
}

Real
TigerKernelH::computeQpJacobian()
{
  Real _dt_coeff = 0.0;
  if (_fe_problem.isTransient())
    _dt_coeff = -1.0/(_beta_s[_qp] +  _beta_f[_qp] * _n[_qp]);
  else
    _dt_coeff = -1.0;

  return _dt_coeff * _grad_test[_i][_qp] * ( (_k[_qp]/_mu[_qp]) * _grad_phi[_j][_qp] );
}
