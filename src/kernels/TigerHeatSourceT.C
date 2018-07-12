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

#include "TigerHeatSourceT.h"

// MOOSE
#include "Function.h"

template <>
InputParameters
validParams<TigerHeatSourceT>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<Real>("value", 1.0, "Constant heat source (sink) (W/m^3) (positive is a source, and negative is a sink) or a multiplier for the the provided function");
  params.addParam<FunctionName>("function", "1", "Heat source (sink) as a function (W/m^3) (positive is a source, and negative is a sink)");
  return params;
}

TigerHeatSourceT::TigerHeatSourceT(const InputParameters & parameters)
  : Kernel(parameters),
    _scaling_lowerD(getMaterialProperty<Real>("lowerD_scale_factor_t")),
    _scale(getParam<Real>("value")),
    _function(getFunction("function")),
    _SUPG_p(getMaterialProperty<RealVectorValue>("petrov_supg_p_function")),
    _SUPG_ind(getMaterialProperty<bool>("supg_indicator"))
{
}

Real
TigerHeatSourceT::computeQpResidual()
{
  Real factor = _scale * _function.value(_t, _q_point[_qp]);
  Real R;
  if (_SUPG_ind[_qp])
    R = (_test[_i][_qp] + _SUPG_p[_qp] * _grad_test[_i][_qp]) * -factor;
  else
    R = _test[_i][_qp] * -factor;
  return _scaling_lowerD[_qp] * R;
}
