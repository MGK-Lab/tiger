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

#include "TigerDarcyVelocityComponent.h"

template <>
InputParameters
validParams<TigerDarcyVelocityComponent>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("gradient_variable", "Variable name for pressure field");
  MooseEnum component("x=0 y=1 z=2");
  params.addRequiredParam<MooseEnum>("component", component, "The Darcy velocity component to compute");
  return params;
}

TigerDarcyVelocityComponent::TigerDarcyVelocityComponent(const InputParameters & parameters)
  : AuxKernel(parameters),
    _gradient_pore_pressure(coupledGradient("gradient_variable")),
    _k_vis(getMaterialProperty<RankTwoTensor>("permeability_by_viscosity")),
    _rhof_g(getMaterialProperty<RealVectorValue>("rho_times_gravity")),
    _component(getParam<MooseEnum>("component"))
{
}

Real
TigerDarcyVelocityComponent::computeValue()
{
  return -(_k_vis[_qp] * (_gradient_pore_pressure[_qp] - _rhof_g[_qp]))(_component);
}
