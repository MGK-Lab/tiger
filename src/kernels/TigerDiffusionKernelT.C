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

#include "TigerDiffusionKernelT.h"

template <>
InputParameters
validParams<TigerDiffusionKernelT>()
{
  InputParameters params = validParams<Kernel>();
  return params;
}

TigerDiffusionKernelT::TigerDiffusionKernelT(const InputParameters & parameters)
  : Kernel(parameters),
    _rho_f(getMaterialProperty<Real>("fluid_density")),
    _rho_s(getMaterialProperty<Real>("solid_density")),
    _cp_f(getMaterialProperty<Real>("fluid_specific_heat")),
    _cp_s(getMaterialProperty<Real>("solid_specific_heat")),
    _n(getMaterialProperty<Real>("porosity")),
    _lambda_f(getMaterialProperty<Real>("fluid_thermal_conductivity")),
    _lambda_s(getMaterialProperty<std::vector<Real>>("solid_conductivity_vector"))
{
}

RankTwoTensor
TigerDiffusionKernelT::ConductivityTensorCalculator(Real const & n, Real const & lambda_f, std::vector<Real> lambda_s)
{
  RankTwoTensor temp;
  switch (_lambda_s.size())
  {
    case 1:
      temp = RankTwoTensor(1., 1., 1., 0., 0., 0.) * std::pow(lambda_f,n)*std::pow(lambda_s[0],(1.-n));
      break;

    case 3:
      temp = RankTwoTensor(std::pow(lambda_s[0],(1.-n)), std::pow(lambda_s[1],(1.-n)), std::pow(lambda_s[2],(1.-n)), 0., 0., 0.) * std::pow(lambda_f,n);
      break;
  }
  return temp;
}

Real
TigerDiffusionKernelT::computeQpResidual()
{
  Real _dt_coeff = 0.0;
  if (_fe_problem.isTransient())
    _dt_coeff = -1.0/( (1-_n[_qp])*_rho_s[_qp]*_cp_s[_qp] + _rho_f[_qp]*_cp_f[_qp]*_n[_qp]);
  else
    _dt_coeff = -1.0;

  return _dt_coeff * _grad_test[_i][_qp] * ( ConductivityTensorCalculator(_n[_qp], _lambda_f[_qp], _lambda_s[_qp]) * _grad_u[_qp]);
}

Real
TigerDiffusionKernelT::computeQpJacobian()
{
  Real _dt_coeff = 0.0;
  if (_fe_problem.isTransient())
    _dt_coeff = -1.0/( (1-_n[_qp])*_rho_s[_qp]*_cp_s[_qp] + _rho_f[_qp]*_cp_f[_qp]*_n[_qp]);
  else
    _dt_coeff = -1.0;

  return _dt_coeff * _grad_test[_i][_qp] * ( ConductivityTensorCalculator(_n[_qp], _lambda_f[_qp], _lambda_s[_qp]) * _grad_phi[_j][_qp]);
}
