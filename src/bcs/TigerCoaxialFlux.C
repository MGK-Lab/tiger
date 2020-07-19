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

#include "TigerCoaxialFlux.h"

registerMooseObject("TigerApp", TigerCoaxialFlux);

template <>
InputParameters
validParams<TigerCoaxialFlux>()
{
  InputParameters params = validParams<IntegratedBC>();
  params.addRequiredCoupledVar("coupled_var", "wellbore temperature");
  return params;
}

TigerCoaxialFlux::TigerCoaxialFlux(const InputParameters & parameters)
  : IntegratedBC(parameters),
  _coupled_var(coupledValue("coupled_var")),
  _coupled_var_number(coupled("coupled_var")),
  _kd(getMaterialProperty<Real>("conductivity_drilling_pipe")),
  _rdo(getMaterialProperty<Real>("drilling_pipe_outer_radius")),
  _rdi(getMaterialProperty<Real>("drilling_pipe_inner_radius")),
  _fac(getMaterialProperty<Real>("factor"))
{
}

Real
TigerCoaxialFlux::computeQpResidual()
{

  return _test[_i][_qp] * _fac[_qp] * _kd[_qp] * (_u[_qp] - _coupled_var[_qp]) / log(_rdo[_qp]/_rdi[_qp]);

}

Real
TigerCoaxialFlux::computeQpJacobian()
{
  return _test[_i][_qp] * _fac[_qp] * _kd[_qp] * _phi[_j][_qp] / log(_rdo[_qp]/_rdi[_qp]);
}

Real
TigerCoaxialFlux::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real s = 0.0;

  if (jvar == _coupled_var_number)
  {

     s -=  _phi[_j][_qp];

  }

  return _test[_i][_qp] * _fac[_qp] * _kd[_qp] * s / log(_rdo[_qp]/_rdi[_qp]);

}
