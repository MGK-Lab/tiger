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

#include "TigerCoaxialFluxIndicator.h"

registerMooseObject("TigerApp", TigerCoaxialFluxIndicator);

template <>
InputParameters
validParams<TigerCoaxialFluxIndicator>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("coupled_var", "wellbore temperature");
  params.addRequiredCoupledVar("wall_var", "surface temperature of drilling pipe");
  return params;
}

TigerCoaxialFluxIndicator::TigerCoaxialFluxIndicator(const InputParameters & parameters)
  : AuxKernel(parameters),
  _coupled_var(coupledValue("coupled_var")),
  _wall_var(coupledValue("wall_var")),
  _kd(getMaterialProperty<Real>("conductivity_drilling_pipe")),
  _rdo(getMaterialProperty<Real>("drilling_pipe_outer_radius")),
  _rdi(getMaterialProperty<Real>("drilling_pipe_inner_radius")),
  _fac(getMaterialProperty<Real>("factor"))
{
}

Real
TigerCoaxialFluxIndicator::computeValue()
{
  return _fac[_qp] * _kd[_qp] * (_wall_var[_qp] - _coupled_var[_qp]) / log(_rdo[_qp]/_rdi[_qp]);
}
