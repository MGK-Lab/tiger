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

#include "TigerWaterElder.h"

template <>
InputParameters
validParams<TigerWaterElder>()
{
  InputParameters params = validParams<TigerFluidPropertiesTP>();
  params.addClassDescription("Water properties varies with tempreture");
  return params;
}

TigerWaterElder::TigerWaterElder(const InputParameters & parameters)
  : TigerFluidPropertiesTP(parameters)
{
}

std::string
TigerWaterElder::fluidName() const
{
  return "Water (Elder)";
}

Real
TigerWaterElder::rho(Real /*pressure*/, Real temperature) const
{
  return 1.0e3 * ( 1.0 + 0.2 * temperature);
}


Real
TigerWaterElder::mu(Real /*pressure*/, Real /*temperature*/) const
{
  return 0.001;
}

Real
TigerWaterElder::beta(Real /*pressure*/, Real /*temperature*/) const
{
  return 4.4e-10;
}

Real
TigerWaterElder::cp(Real /*pressure*/, Real temperature) const
{
  return 1.0 / (1.0e3 * ( 1.0 + 0.2 * temperature));
}

Real
TigerWaterElder::lambda(Real /*pressure*/, Real /*temperature*/) const
{
  return 3.56e-6;
}
