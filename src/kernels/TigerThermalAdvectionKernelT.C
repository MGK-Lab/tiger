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

#include "TigerThermalAdvectionKernelT.h"
#include "MaterialPropertyInterface.h"

registerMooseObject("TigerApp", TigerThermalAdvectionKernelT);

template <>
InputParameters
validParams<TigerThermalAdvectionKernelT>()
{
  InputParameters params = validParams<Kernel>();

  params.addCoupledVar("pressure_varible", 0 ,"The variable representing the pressure.");

  return params;
}

TigerThermalAdvectionKernelT::TigerThermalAdvectionKernelT(const InputParameters & parameters)
  : Kernel(parameters),
  _scale_factor(getMaterialProperty<Real>("scale_factor")),
  _rho_f(getMaterialProperty<Real>("fluid_density")),
  _cp_f(getMaterialProperty<Real>("fluid_specific_heat")),
  _SUPG_p(getMaterialProperty<RealVectorValue>("petrov_supg_p_function")),
  _SUPG_ind(getMaterialProperty<bool>("supg_indicator")),
  _darcy_v(getMaterialProperty<RealVectorValue>("darcy_velocity")),
  _pressure_var(coupled("pressure_varible"))
{
  if (parameters.isParamSetByUser("pressure_varible"))
    _k_vis = &getMaterialProperty<RankTwoTensor>("permeability_by_viscosity");
}

Real
TigerThermalAdvectionKernelT::computeQpResidual()
{
  Real R;
  if (_SUPG_ind[_qp])
    R = _rho_f[_qp] * _cp_f[_qp] * ((_test[_i][_qp] + _SUPG_p[_qp] * _grad_test[_i][_qp]) * ( _darcy_v[_qp] * _grad_u[_qp]));
  else
    R = _rho_f[_qp] * _cp_f[_qp] * (_test[_i][_qp] * ( _darcy_v[_qp] * _grad_u[_qp]));

  return _scale_factor[_qp] * R;
}

Real
TigerThermalAdvectionKernelT::computeQpJacobian()
{
  Real R;
  if (_SUPG_ind[_qp])
    R = _rho_f[_qp] * _cp_f[_qp] * ((_test[_i][_qp] + _SUPG_p[_qp] * _grad_test[_i][_qp]) * ( _darcy_v[_qp] * _grad_phi[_j][_qp]));
  else
    R = _rho_f[_qp] * _cp_f[_qp] * (_test[_i][_qp] * ( _darcy_v[_qp] * _grad_phi[_j][_qp]));

  return _scale_factor[_qp] * R;
}

Real
TigerThermalAdvectionKernelT::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real R;
  if (jvar == _pressure_var)
  {
    if (_SUPG_ind[_qp])
      R = _rho_f[_qp] * _cp_f[_qp] * ((_test[_i][_qp] + _SUPG_p[_qp] * _grad_test[_i][_qp]) * ( -(*_k_vis)[_qp] * _grad_phi[_j][_qp] * _grad_u[_qp]));
    else
      R = _rho_f[_qp] * _cp_f[_qp] * (_test[_i][_qp] * ( -(*_k_vis)[_qp] * _grad_phi[_j][_qp] * _grad_u[_qp]));

    R *=_scale_factor[_qp];
  }
  else
    R = 0.0;


  return R;
}
