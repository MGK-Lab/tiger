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

#include "TigerRockMaterialH.h"
#include "Material.h"
#include "MooseMesh.h"

template <>
InputParameters
validParams<TigerRockMaterialH>()
{
  InputParameters params = validParams<TigerMaterialGeneral>();
  params.addParam<bool>("has_gravity", false, "Is the gravity enabled?");
  params.addParam<Real>("gravity_acceleration", 9.81, "The magnitude of the gravity acceleration (m/s^2)");
  params.addRequiredParam<Real>("porosity", "Porosity of rock matrix");
  params.addRequiredParam<Real>("compressibility", "Compressibility of rock matrix (1/Pa)");

  MooseEnum PT("isotropic=1 orthotropic=2 anisotropic=3");
  params.addRequiredParam<MooseEnum>("permeability_type", PT, "The permeability distribution type [isotropic, orthotropic, anisotropic].");

  params.addRequiredParam<std::vector<Real>>("k0", "Initial permeability of rock matrix (m^2)");
  params.addRequiredParam<UserObjectName>("kf_UO", "The name of the user object for permeability calculation");
  params.addClassDescription("Rock matrix hydraulic properties");
  return params;
}

TigerRockMaterialH::TigerRockMaterialH(const InputParameters & parameters)
  : TigerMaterialGeneral(parameters),
    _pt(getParam<MooseEnum>("permeability_type")),
    _k0(getParam<std::vector<Real>>("k0")),
    _beta_s(getParam<Real>("compressibility")),
    _n0(getParam<Real>("porosity")),
    _k_vis(declareProperty<RankTwoTensor>("permeability_by_viscosity")),
    _H_Kernel_dt(declareProperty<Real>("H_Kernel_dt_coefficient")),
    _rhof(declareProperty<Real>("fluid_density")),
    _rhof_g(declareProperty<RealVectorValue>("rho_times_gravity")),
    _kf_UO(getUserObject<TigerPermeability>("kf_UO")),
    _has_gravity(getParam<bool>("has_gravity")),
    _g(getParam<Real>("gravity_acceleration")),
    _test(declareProperty<RankTwoTensor>("test"))
{
  if (_has_gravity)
    _gravity = RealVectorValue(0.0, 0.0, -_g);
  else
    _gravity = RealVectorValue(0.0, 0.0, 0.0);
}


void
TigerRockMaterialH::computeQpProperties()
{
  _k_vis[_qp] = _kf_UO.PermeabilityTensorCalculator(_pt,_k0) / _fp_UO.mu(_P[_qp], _T[_qp]);
  _H_Kernel_dt[_qp] = _beta_s + _fp_UO.beta(_P[_qp], _T[_qp]) * _n0;
  _rhof_g[_qp] = _fp_UO.rho(_P[_qp], _T[_qp]) * _gravity;
  _rhof[_qp] = _fp_UO.rho(_P[_qp], _T[_qp]);
  if (_current_elem->dim() < _mesh.dimension())
  {
    _rot_mat.zero();
    computeRotationMatrix();
    // _k_vis[_qp](2,2) = 0.0;
    _k_vis[_qp].rotate(_rot_mat);
  }
}
