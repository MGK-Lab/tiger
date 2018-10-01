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

#include "TigerMaterialGeneral.h"

template <>
InputParameters
validParams<TigerMaterialGeneral>()
{
  InputParameters params = validParams<Material>();

  params.addCoupledVar("pressure", 0.0, "Pore pressure nonlinear variable (Pa)");
  params.addCoupledVar("temperature", 273.15, "temperature nonlinear variable (K)");
  params.addRequiredParam<UserObjectName>("fp_uo", "The name of the userobject "
                                          "for fluid properties");
  params.addParam<UserObjectName>("supg_uo", "", "The name of the userobject "
                                          "for SU/PG");

  return params;
}

TigerMaterialGeneral::TigerMaterialGeneral(const InputParameters & parameters)
  : Material(parameters),
    _P(coupledValue("pressure")),
    _T(coupledValue("temperature")),
    _fp_uo(getUserObject<SinglePhaseFluidPropertiesPT>("fp_uo"))
{
  if (parameters.isParamSetByUser("supg_uo"))
    _supg_uo = &getUserObject<TigerSUPG>("supg_uo");
  else
    _supg_uo = NULL;
}
