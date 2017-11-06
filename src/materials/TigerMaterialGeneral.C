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

#include "TigerMaterialGeneral.h"

template <>
InputParameters
validParams<TigerMaterialGeneral>()
{
  InputParameters params = validParams<Material>();
  params.addCoupledVar("pressure", 0.0, "Fluid pressure (Pa)");
  params.addCoupledVar("temperature", 0.0, "Fluid temperature (C)");
  params.addRequiredParam<UserObjectName>("fp_UO", "The name of the user object for fluid properties");
  params.addClassDescription("General properties + fluid properties");
  return params;
}

TigerMaterialGeneral::TigerMaterialGeneral(const InputParameters & parameters)
  : Material(parameters),
    _P(coupledValue("pressure")),
    _T(coupledValue("temperature")),
    _fp_UO(getUserObject<TigerFluidPropertiesTP>("fp_UO"))
{
}
