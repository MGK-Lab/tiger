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

#include "TigerRockMaterialGeneral.h"
#include "MooseMesh.h"
#include "MooseError.h"

template <>
InputParameters
validParams<TigerRockMaterialGeneral>()
{
  InputParameters params = validParams<Material>();
  params.addParam<bool>("has_gravity", false, "Is the gravity enabled?");
  params.addParam<Real>("gravity_acceleration", 9.81, "The magnitude of the gravity acceleration (m/s^2)");
  params.addRequiredParam<Real>("porosity", "Porosity of rock matrix");
  params.addClassDescription("Rock matrix general properties");
  return params;
}

TigerRockMaterialGeneral::TigerRockMaterialGeneral(const InputParameters & parameters)
  : Material(parameters),
    _n0(getParam<Real>("porosity")),
    _has_gravity(getParam<bool>("has_gravity")),
    _g(getParam<Real>("gravity_acceleration")),
    _gravity(declareProperty<RealVectorValue>("gravity_vector")),
    _n(declareProperty<Real>("porosity"))
{
  if (_has_gravity)
    _gravity0 = RealVectorValue(0.0, 0.0, -_g);
  else
    _gravity0 = RealVectorValue(0.0, 0.0, 0.0);
}

void
TigerRockMaterialGeneral::computeQpProperties()
{
  // if (_mesh.dimension() == 3)
  //   mooseError("Rock matrix material should be defined in 3D\n");
  //
  _n[_qp] = _n0;
  _gravity[_qp] = _gravity0;
}
