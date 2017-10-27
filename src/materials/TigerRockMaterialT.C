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
  InputParameters params = validParams<Material>();
  params.addRequiredParam<Real>("specific_heat", "Specific heat of rock matrix (J/(kg K))");
  params.addRequiredParam<Real>("density", "density of rock matrix (kg/m^3)");

  MooseEnum CT("isotropic=1 orthotropic=2");
  params.addRequiredParam<MooseEnum>("conductivity_type", CT, "Thermal conductivity distribution type [isotropic, orthotropic].");
  params.addRequiredParam<std::vector<Real>>("lambda", "Initial thermal conductivity of rock matrix (W/(m K))");

  params.addClassDescription("Rock matrix thermal properties");
  return params;
}

TigerRockMaterialT::TigerRockMaterialT(const InputParameters & parameters)
  : Material(parameters),
    _conductivity_type(getParam<MooseEnum>("conductivity_type")),
    _lambda0(getParam<std::vector<Real>>("lambda")),
    _cp0(getParam<Real>("specific_heat")),
    _rho0(getParam<Real>("density")),
    _lambda(declareProperty<std::vector<Real>>("solid_conductivity_vector")),
    _cp(declareProperty<Real>("solid_specific_heat")),
    _rho(declareProperty<Real>("solid_density"))
{
  switch (_conductivity_type)
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

void
TigerRockMaterialT::computeQpProperties()
{
  _lambda[_qp] = _lambda0;
  _cp    [_qp] = _cp0;
  _rho   [_qp] = _rho0;
}
