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

#include "TigerWaterPropertiesConst.h"

template <>
InputParameters
validParams<TigerWaterPropertiesConst>()
{
  InputParameters params = validParams<TigerFluidPropertiesTP>();
  params.addClassDescription("Water properties constant with tempreture and pressure");
  return params;
}

TigerWaterPropertiesConst::TigerWaterPropertiesConst(const InputParameters & parameters)
  : TigerFluidPropertiesTP(parameters)
{
}

std::string
TigerWaterPropertiesConst::fluidName() const
{
  return "Water Const";
}

Real
TigerWaterPropertiesConst::rho(Real /*pressure*/, Real /*temperature*/) const
{
  return 1000.0;
}


Real
TigerWaterPropertiesConst::mu(Real /*temperature*/) const
{
  return 0.001;
}

Real
TigerWaterPropertiesConst::beta(Real /*pressure*/, Real /*temperature*/) const
{
  return 4.4e-10;
}
