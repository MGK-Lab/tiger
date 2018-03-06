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

#define PI 3.141592653589793238462643383279502884197169399375105820974944592308

template <>
InputParameters
validParams<TigerMaterialGeneral>()
{
  InputParameters params = validParams<Material>();
  params.addCoupledVar("pressure", 0.0, "Fluid pressure (Pa)");
  params.addCoupledVar("temperature", 0.0, "Fluid temperature (C)");
  params.addRequiredParam<UserObjectName>("fp_UO", "The name of the userobject for fluid properties");
  params.addParam<Real>("scaling_factor", 1.0, "The scaling factor for lower dimensional elements (2D & 1D elements in 2D & 3D simulations)");
  params.addClassDescription("General properties");
  return params;
}

TigerMaterialGeneral::TigerMaterialGeneral(const InputParameters & parameters)
  : Material(parameters),
    _P(coupledValue("pressure")),
    _T(coupledValue("temperature")),
    _scaling_factor0(getParam<Real>("scaling_factor")),
    _fp_UO(getUserObject<TigerFluidPropertiesTP>("fp_UO"))
{
}

Real
TigerMaterialGeneral::LowerDScaling()
{
  Real _scaling_factor = 1.0;
  if (_mesh.dimension()>1)
    switch (_mesh.dimension())
    {
      case 2:
        _scaling_factor = _scaling_factor0; //aperture * height for fracture & height for unit
        break;
      case 3:
        if (_current_elem->dim() == 2)
          _scaling_factor = _scaling_factor0; //fracture aperture
        else if (_current_elem->dim() == 1)
          _scaling_factor = PI * _scaling_factor0 * _scaling_factor0; //radius of well
        break;
    }
  return _scaling_factor;
}

void
TigerMaterialGeneral::computeRotationMatrix(int dim)
{
  RealVectorValue xp, yp, zp;
  xp = _current_elem->point(1) - _current_elem->point(0);

  switch (dim)
  {
    case 1:
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
      zp = xp.cross(yp);
      yp = zp.cross(xp);
      break;

    case 2:
      yp = _current_elem->point(2) - _current_elem->point(1);
      zp = xp.cross(yp);
      if (!((std::fabs(zp(0)) + std::fabs(zp(1)))/zp.norm() < DBL_MIN)) //horizontal fracture check
        xp = RealVectorValue(0.,0.,1.).cross(zp);
      else
        xp = RealVectorValue(1.,0.,0.);
      yp = zp.cross(xp);
      break;
  }

  for (unsigned int i = 0; i < 3; ++i)
  {
    (_rot_mat)(i, 0) = xp(i) / xp.norm();
    (_rot_mat)(i, 1) = yp(i) / yp.norm();
    (_rot_mat)(i, 2) = zp(i) / zp.norm();
  }
}
