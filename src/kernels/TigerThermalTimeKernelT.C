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

#include "TigerThermalTimeKernelT.h"

registerMooseObject("TigerApp", TigerThermalTimeKernelT);

template <>
InputParameters
validParams<TigerThermalTimeKernelT>()
{
  InputParameters params = validParams<TimeDerivative>();
  return params;
}

TigerThermalTimeKernelT::TigerThermalTimeKernelT(const InputParameters & parameters)
  : TimeDerivative(parameters),
    _scale_factor(getMaterialProperty<Real>("scale_factor")),
    _T_Kernel_dt(getMaterialProperty<Real>("T_Kernel_dt_coefficient")),
    _SUPG_p(getMaterialProperty<RealVectorValue>("petrov_supg_p_function")),
    _SUPG_ind(getMaterialProperty<bool>("supg_indicator"))
{
}

Real
TigerThermalTimeKernelT::computeQpResidual()
{
  Real test;
  if (_SUPG_ind[_qp])
    test = _test[_i][_qp] + _SUPG_p[_qp] * _grad_test[_i][_qp];
  else
    test = _test[_i][_qp];
  return _scale_factor[_qp] * _T_Kernel_dt[_qp] * test * _u_dot[_qp];
}

Real
TigerThermalTimeKernelT::computeQpJacobian()
{
  Real test;
  if (_SUPG_ind[_qp])
    test = _test[_i][_qp] + _SUPG_p[_qp] * _grad_test[_i][_qp];
  else
    test = _test[_i][_qp];
  return _scale_factor[_qp] * _T_Kernel_dt[_qp] * test * _phi[_j][_qp] * _du_dot_du[_qp];
}
