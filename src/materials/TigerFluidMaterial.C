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

#include "TigerFluidMaterial.h"

registerMooseObject("TigerApp", TigerFluidMaterial);

template <>
InputParameters
validParams<TigerFluidMaterial>()
{
  InputParameters params = validParams<Material>();

  params.addCoupledVar("pressure", 0.0, "Pore pressure nonlinear variable (Pa)");
  params.addCoupledVar("temperature", 273.15, "temperature nonlinear variable (K)");
  params.addRequiredParam<UserObjectName>("fp_uo", "The name of the userobject "
                                          "for fluid properties");

  return params;
}

TigerFluidMaterial::TigerFluidMaterial(const InputParameters & parameters)
  : Material(parameters),
    _P(coupledValue("pressure")),
    _T(coupledValue("temperature")),
    _fp_uo(getUserObject<SinglePhaseFluidPropertiesPT>("fp_uo")),
    _rho_f(declareProperty<Real>("fluid_density")),
    _mu(declareProperty<Real>("fluid_viscosity")),
    _beta_f(declareProperty<Real>("fluid_compressibility")),
    _cp_f(declareProperty<Real>("fluid_specific_heat")),
    _lambda_f(declareProperty<Real>("fluid_thermal_conductivity"))
{
}

void
TigerFluidMaterial::computeQpProperties()
{
  _rho_f[_qp] = _fp_uo.rho(_P[_qp], _T[_qp]);
  _mu[_qp] = _fp_uo.mu(_P[_qp], _T[_qp]);
  _beta_f[_qp] = 1.0 / (std::pow(_fp_uo.c(_P[_qp], _T[_qp]),2.0) * _rho_f[_qp]);
  _cp_f[_qp] = _fp_uo.cp(_P[_qp], _T[_qp]);
  _lambda_f[_qp] = _fp_uo.k(_P[_qp], _T[_qp]);
}
