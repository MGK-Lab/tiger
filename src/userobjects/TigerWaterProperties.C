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

#include "TigerWaterProperties.h"

template <>
InputParameters
validParams<TigerWaterProperties>()
{
  InputParameters params = validParams<TigerFluidPropertiesTP>();
  params.addClassDescription("Water properties varies with tempreture and pressure (FRACture model)");
  return params;
}

TigerWaterProperties::TigerWaterProperties(const InputParameters & parameters)
  : TigerFluidPropertiesTP(parameters)
{
}

std::string
TigerWaterProperties::fluidName() const
{
  return "Water (FRACture model)";
}

Real
TigerWaterProperties::rho(Real pressure, Real temperature) const
{
  Real _a = -9.9559 + 7.0845*std::exp(-1.638e-4*temperature)+3.909*std::exp(2.551e-10*pressure);
  return (-3.033405 + 10.128163*_a - 8.750567*_a*_a + 2.663107*_a*_a*_a)*1.0e3;
}


Real
TigerWaterProperties::mu(Real temperature) const
{
  return 2.4e-5*std::pow(10.0, 248.37/(temperature+133.15));
}

Real
TigerWaterProperties::beta(Real /*pressure*/, Real /*temperature*/) const
{
  return 4.4e-10;
}

