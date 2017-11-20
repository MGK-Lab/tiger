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

#include "TigerRockMaterialT.h"

template <>
InputParameters
validParams<TigerRockMaterialT>()
{
  InputParameters params = validParams<TigerMaterialGeneral>();
  params.addRequiredParam<Real>("specific_heat", "Specific heat of rock matrix (J/(kg K))");
  params.addRequiredParam<Real>("density", "density of rock matrix (kg/m^3)");
  params.addRequiredParam<Real>("porosity", "Porosity of rock matrix");
  MooseEnum CT("isotropic=1 orthotropic=2");
  params.addRequiredParam<MooseEnum>("conductivity_type", CT, "Thermal conductivity distribution type [isotropic, orthotropic].");
  params.addRequiredParam<std::vector<Real>>("lambda", "Initial thermal conductivity of rock matrix (W/(m K))");

  params.addClassDescription("Rock matrix thermal properties");
  return params;
}

TigerRockMaterialT::TigerRockMaterialT(const InputParameters & parameters)
  : TigerMaterialGeneral(parameters),
    _ct(getParam<MooseEnum>("conductivity_type")),
    _lambda0(getParam<std::vector<Real>>("lambda")),
    _cp0(getParam<Real>("specific_heat")),
    _rho0(getParam<Real>("density")),
    _n0(getParam<Real>("porosity")),
    _lambda_sf(declareProperty<RankTwoTensor>("conductivity_mixture")),
    _T_Kernel_dt(declareProperty<Real>("T_Kernel_dt_coefficient")),
    _rho_cp_f(declareProperty<Real>("fluid_thermal_capacity"))
{
  switch (_ct)
  {
    case 1:
      if (_lambda0.size() != 1)
        mooseError("One input value is needed for isotropic distribution of rock thermal conductivity! You provided ", _lambda0.size(), " values.\n");
      break;

    case 2:
      if (_lambda0.size() != 3)
        mooseError("Three input values are needed for orthotropic distribution of rock thermal conductivity! You provided ", _lambda0.size(), " values.\n");
      break;
  }
}

RankTwoTensor
TigerRockMaterialT::ConductivityTensorCalculator(Real const & n, Real const & lambda_f, std::vector<Real> lambda_s)
{
  RankTwoTensor temp;
  switch (lambda_s.size())
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

void
TigerRockMaterialT::computeQpProperties()
{
  _T_Kernel_dt[_qp] = (1.0-_n0)*_rho0*_cp0 + _fp_UO.rho(_P[_qp], _T[_qp])*_fp_UO.cp(_P[_qp], _T[_qp])*_n0;
  _lambda_sf  [_qp] = ConductivityTensorCalculator(_n0, _fp_UO.lambda(_P[_qp], _T[_qp]), _lambda0);
  _rho_cp_f   [_qp] = _fp_UO.rho(_P[_qp], _T[_qp])*_fp_UO.cp(_P[_qp], _T[_qp]);
}
