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

#include "TigerFluidConst.h"

template <>
InputParameters
validParams<TigerFluidConst>()
{
  InputParameters params = validParams<TigerFluidPropertiesTP>();
  params.addParam<Real>("density", 1000.0, "Constant density for the fluid (kg/m^3)");
  params.addParam<Real>("viscosity", 1.0e-3, "Constant viscosity for the fluid (Pa.s)");
  params.addParam<Real>("specific_heat", 4.2e3, "Constant specific heat for the fluid (J/(kg.K))");
  params.addParam<Real>("conductivity", 0.6, "Constant thermal conductivity for the fluid (W/(m.K))");
  params.addParam<Real>("compressibility", 4.4e-10, "Compressibility of the fluid (1/Pa)");
  params.addClassDescription("fluid properties constant with tempreture and pressure (default water)");
  return params;
}

TigerFluidConst::TigerFluidConst(const InputParameters & parameters)
  : TigerFluidPropertiesTP(parameters),
    _rho(getParam<Real>("density")),
    _mu(getParam<Real>("viscosity")),
    _beta(getParam<Real>("compressibility")),
    _cp(getParam<Real>("specific_heat")),
    _lambda(getParam<Real>("conductivity"))
{
}

std::string
TigerFluidConst::fluidName() const
{
  return "Fluid Const";
}

Real
TigerFluidConst::rho(Real /*pressure*/, Real /*temperature*/) const
{
  return _rho;
}


Real
TigerFluidConst::mu(Real /*pressure*/, Real /*temperature*/) const
{
  return _mu;
}

Real
TigerFluidConst::beta(Real /*pressure*/, Real /*temperature*/) const
{
  return _beta;
}

Real
TigerFluidConst::cp(Real /*pressure*/, Real /*temperature*/) const
{
  return _cp;
}

Real
TigerFluidConst::lambda(Real /*pressure*/, Real /*temperature*/) const
{
  return _lambda;
}
