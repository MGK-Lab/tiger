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
    _kf(getMaterialProperty<RankTwoTensor>("permeability_tensor")),
    _rho_f(getMaterialProperty<Real>("fluid_density")),
    _viscosity(getMaterialProperty<Real>("viscosity")),
    _gravity(getMaterialProperty<RealVectorValue>("gravity_vector")),
    _component(getParam<MooseEnum>("component"))
{
}

Real
TigerDarcyVelocityComponent::computeValue()
{
  RealVectorValue _Darcy_Vel = (_kf[_qp]/_viscosity[_qp]) * (_gradient_pore_pressure[_qp]-_rho_f[_qp]*_gravity[_qp]);
  return -_Darcy_Vel(_component);
}
