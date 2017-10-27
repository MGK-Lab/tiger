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

#include "TigerFluidMaterialTP.h"

template <>
InputParameters
validParams<TigerFluidMaterialTP>()
{
  InputParameters params = validParams<Material>();
  params.addRequiredCoupledVar("pressure", "Fluid pressure (Pa)");
  params.addRequiredCoupledVar("temperature", "Fluid temperature (C)");
  params.addRequiredParam<UserObjectName>("fp_UO", "The name of the user object for fluid properties");
  params.addClassDescription("Fluid properties using the (pressure, temperature) formulation");
  return params;
}

TigerFluidMaterialTP::TigerFluidMaterialTP(const InputParameters & parameters)
  : Material(parameters),
    _pressure(coupledValue("pressure")),
    _temperature(coupledValue("temperature")),
    _rho(declareProperty<Real>("fluid_density")),
    _mu(declareProperty<Real>("viscosity")),
    _beta(declareProperty<Real>("fluid_compressibility")),
    _cp(declareProperty<Real>("fluid_specific_heat")),
    _lambda(declareProperty<Real>("fluid_thermal_conductivity")),
    _fluid_properties_UO(getUserObject<TigerFluidPropertiesTP>("fp_UO"))
{
}

void
TigerFluidMaterialTP::computeQpProperties()
{
  _rho   [_qp] = _fluid_properties_UO.rho   (_pressure[_qp], _temperature[_qp]);
  _mu    [_qp] = _fluid_properties_UO.mu    (_pressure[_qp], _temperature[_qp]);
  _beta  [_qp] = _fluid_properties_UO.beta  (_pressure[_qp], _temperature[_qp]);
  _cp    [_qp] = _fluid_properties_UO.cp    (_pressure[_qp], _temperature[_qp]);
  _lambda[_qp] = _fluid_properties_UO.lambda(_pressure[_qp], _temperature[_qp]);
}
