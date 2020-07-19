/**************************************************************************/
/*  MOSKITO - Multiphysics cOupled Simulator toolKIT for wellbOres        */
/*                                                                        */
/*  Copyright (C) 2017 by Maziar Gholami Korzani                          */
/*  Karlsruhe Institute of Technology, Institute of Applied Geosciences   */
/*  Division of Geothermal Research                                       */
/*                                                                        */
/*  This file is part of MOSKITO App                                      */
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

#include "TigerCoaxialHeat_1p.h"

registerMooseObject("TigerApp", TigerCoaxialHeat_1p);

template <>
InputParameters
validParams<TigerCoaxialHeat_1p>()
{
    InputParameters params = validParams<Material>();

    params.addClassDescription("Materials for the Lateral heat transfer between "
          "wellbore and drilling pipe");

    params.addParam<Real>("drilling_pipe_outer_radius", 0.06985,
          "outer diameter of the drilling pipe (m)");
    params.addParam<Real>("drilling_pipe_inner_radius", 0.04445,
          "outer diameter of the drilling pipe (m)");
    params.addParam<Real>("conductivity_drilling_pipe", 80.0 ,
          "Thermal conductivity of the drilling pipe (W/(m*K))");

    return params;
}

TigerCoaxialHeat_1p::TigerCoaxialHeat_1p(const InputParameters & parameters)
  : Material(parameters),

    _rdo(declareProperty<Real>("drilling_pipe_outer_radius")),
    _rdi(declareProperty<Real>("drilling_pipe_inner_radius")),
    _kd(declareProperty<Real>("conductivity_drilling_pipe")),

    _rdoo(getParam<Real>("drilling_pipe_outer_radius")),
    _rdii(getParam<Real>("drilling_pipe_inner_radius")),
    _kdd(getParam<Real>("conductivity_drilling_pipe"))
{
}

void
TigerCoaxialHeat_1p::computeQpProperties()
{

  _rdo[_qp] = _rdoo;
  _rdi[_qp] = _rdii;
  _kd[_qp] = _kdd;

}
