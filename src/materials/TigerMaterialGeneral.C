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
  MooseEnum mat_type("well=0 fracture=1 matrix=2");
  params.addRequiredParam<MooseEnum>("material_type", mat_type, "The type of the geological structure (matrix, fracture, well)");
  return params;
}

TigerMaterialGeneral::TigerMaterialGeneral(const InputParameters & parameters)
  : Material(parameters),
    _P(coupledValue("pressure")),
    _T(coupledValue("temperature")),
    _material_type(getParam<MooseEnum>("material_type")),
    _fp_UO(getUserObject<TigerFluidPropertiesTP>("fp_UO"))
{
}

void
TigerMaterialGeneral::computeRotationMatrix()
{
  RealVectorValue xp, yp, zp;
  xp = _current_elem->point(1) - _current_elem->point(0);
  switch (_material_type)
  {
    case 0:
      for (unsigned int i = 0; i < 3; ++i)
        yp(i) = 0.0;
      if (std::fabs(xp(0)) > 0.0 && std::fabs(xp(1)) + std::fabs(xp(2)) < DBL_MIN)
        yp(2) = 1.0;
      else if (std::fabs(xp(1)) > 0.0 && std::fabs(xp(0)) + std::fabs(xp(2)) < DBL_MIN)
        yp(0) = 1.0;
      else if (std::fabs(xp(2)) > 0.0 && std::fabs(xp(0)) + std::fabs(xp(1)) < DBL_MIN)
        yp(1) = 1.0;
      else
      {
        for (unsigned int i = 0; i < 3; ++i)
          if (std::fabs(xp(i)) > 0.0)
          {
            yp(i) = -xp(i);
            break;
          }
      }
      break;
    case 1:
      yp = _current_elem->point(2) - _current_elem->point(1);
      break;
    case 2:
      break;
  }
  if (_material_type<2)
  {
    zp = xp.cross(yp);
    if (!((std::fabs(zp(0)) + std::fabs(zp(1)))/zp.norm() < DBL_MIN))
      xp = RealVectorValue(0.,0.,1.).cross(zp);
    yp = zp.cross(xp);
    for (unsigned int i = 0; i < 3; ++i)
    {
      (_rot_mat)(i, 0) = xp(i) / xp.norm();
      (_rot_mat)(i, 1) = yp(i) / yp.norm();
      (_rot_mat)(i, 2) = zp(i) / zp.norm();
    }
  }
}
