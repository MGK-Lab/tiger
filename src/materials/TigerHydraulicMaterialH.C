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

#include "TigerHydraulicMaterialH.h"
#include "MooseMesh.h"

registerMooseObject("TigerApp", TigerHydraulicMaterialH);

template <>
InputParameters
validParams<TigerHydraulicMaterialH>()
{
  InputParameters params = validParams<Material>();

  params.addRequiredCoupledVar("pressure",
          "Pore pressure nonlinear variable (Pa)");
  params.addParam<bool>("has_gravity", false, "Is the gravity enabled?");
  params.addParam<Real>("gravity_acceleration", 9.81,
        "The magnitude of the gravity acceleration (m/s^2)");
  params.addRequiredParam<Real>("compressibility",
        "The compressibility of the solid porous media (1/Pa)");
  params.addRequiredParam<UserObjectName>("kf_uo",
        "The name of the userobject for the permeability calculation");
  params.addClassDescription("Hydraulic material for hydraulic kernels");

  return params;
}

TigerHydraulicMaterialH::TigerHydraulicMaterialH(const InputParameters & parameters)
  : Material(parameters),
    _grad_p(coupledGradient("pressure")),
    _k_vis(declareProperty<RankTwoTensor>("permeability_by_viscosity")),
    _H_Kernel_dt(declareProperty<Real>("H_Kernel_dt_coefficient")),
    _gravity(declareProperty<RealVectorValue>("gravity_vector")),
    _kf_uo(getUserObject<TigerPermeability>("kf_uo")),
    _dv(declareProperty<RealVectorValue>("darcy_velocity_vector")),
    _ddv_dT(declareProperty<RealVectorValue>("d_darcy_velocity_dT")),
    _n(getMaterialProperty<Real>("porosity")),
    _rot_mat(getMaterialProperty<RankTwoTensor>("lowerD_rotation_matrix")),
    _rho_f(getMaterialProperty<Real>("fluid_density")),
    _mu_f(getMaterialProperty<Real>("fluid_viscosity")),
    _beta_f(getMaterialProperty<Real>("fluid_compressibility")),
    _drho_dT_f(getMaterialProperty<Real>("fluid_drho_dT")),
    _dmu_dT_f(getMaterialProperty<Real>("fluid_dmu_dT")),
    _has_gravity(getParam<bool>("has_gravity")),
    _beta_s(getParam<Real>("compressibility"))
{
  Real _g0 = getParam<Real>("gravity_acceleration");
  if (_has_gravity)
  {
    if (_mesh.dimension() == 3)
      _g = RealVectorValue(0., 0., -_g0);
    else if (_mesh.dimension() == 2)
      _g = RealVectorValue(0., -_g0, 0.);
    else if (_mesh.dimension() == 1)
      _g = RealVectorValue(-_g0, 0., 0.);
  }
  else
    _g = RealVectorValue(0., 0., 0.);
}

void
TigerHydraulicMaterialH::computeQpProperties()
{
  _k_vis[_qp] = _kf_uo.PermeabilityTensorCalculator(_current_elem->dim()) / _mu_f[_qp];
  _H_Kernel_dt[_qp] = _beta_s + _beta_f[_qp] * _n[_qp];
  _gravity[_qp] = _g;

  if (_current_elem->dim() < _mesh.dimension())
    _k_vis[_qp].rotate(_rot_mat[_qp]);

  _dv[_qp] = - _k_vis[_qp] * (_grad_p[_qp] - _rho_f[_qp] * _gravity[_qp]);
  _ddv_dT[_qp] = _k_vis[_qp] * _gravity[_qp] * (_drho_dT_f[_qp] - _dmu_dT_f[_qp] * _rho_f[_qp] / _mu_f[_qp]);
}
