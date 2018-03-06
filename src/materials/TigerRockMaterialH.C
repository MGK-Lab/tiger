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
#include "libmesh/quadrature.h"

template <>
InputParameters
validParams<TigerRockMaterialH>()
{
  InputParameters params = validParams<TigerMaterialGeneral>();
  params.addParam<bool>("has_gravity", false, "Is the gravity enabled?");
  params.addParam<Real>("gravity_acceleration", 9.81, "The magnitude of the gravity acceleration (m/s^2)");
  params.addRequiredParam<Real>("porosity", "Porosity of rock matrix");
  params.addRequiredParam<Real>("compressibility", "Compressibility of rock matrix (1/Pa)");
  params.addRequiredParam<UserObjectName>("kf_UO", "The name of the userobject for permeability calculation");
  params.addClassDescription("Hydraulic properties");
  return params;
}

TigerRockMaterialH::TigerRockMaterialH(const InputParameters & parameters)
  : TigerMaterialGeneral(parameters),
    _beta_s(getParam<Real>("compressibility")),
    _n0(getParam<Real>("porosity")),
    _k_vis(declareProperty<RankTwoTensor>("permeability_by_viscosity")),
    _H_Kernel_dt(declareProperty<Real>("H_Kernel_dt_coefficient")),
    _rhof(declareProperty<Real>("fluid_density")),
    _rhof_g(declareProperty<RealVectorValue>("rho_times_gravity")),
    _kf_UO(getUserObject<TigerPermeability>("kf_UO")),
    _has_gravity(getParam<bool>("has_gravity")),
    _g(getParam<Real>("gravity_acceleration")),
    _scaling_lowerD(declareProperty<Real>("lowerD_scale_factor_h"))
{
  if (_has_gravity)
  {
    if (_mesh.dimension() == 3)
      _gravity = RealVectorValue(0., 0., -_g);
    else if (_mesh.dimension() == 2)
      _gravity = RealVectorValue(0., -_g, 0.);
    else if (_mesh.dimension() == 1)
      _gravity = RealVectorValue(-_g, 0., 0.);
  }
  else
    _gravity = RealVectorValue(0., 0., 0.);
}


void
TigerRockMaterialH::computeProperties()
{
  if (_constant_option == ConstantTypeEnum::SUBDOMAIN)
    return;

  // If this Material has the _constant_on_elem flag set, we take the
  // value computed for _qp==0 and use it at all the quadrature points
  // in the Elem.
  if (_constant_option == ConstantTypeEnum::ELEMENT)
  {
    // Compute MaterialProperty values at the first qp.
    _qp = 0;
    if (_current_elem->dim() < _mesh.dimension())
      computeRotationMatrix(_current_elem->dim());
    computeQpProperties();

    // Reference to *all* the MaterialProperties in the MaterialData object, not
    // just the ones for this Material.
    MaterialProperties & props = _material_data->props();

    // Now copy the values computed at qp 0 to all the other qps.
    for (const auto & prop_id : _supplied_prop_ids)
    {
      auto nqp = _qrule->n_points();
      for (decltype(nqp) qp = 1; qp < nqp; ++qp)
        props[prop_id]->qpCopy(qp, props[prop_id], 0);
    }
  }
  else
  {
    if (_current_elem->dim() < _mesh.dimension())
      computeRotationMatrix(_current_elem->dim());
    for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
      computeQpProperties();
  }
}

void
TigerRockMaterialH::computeQpProperties()
{
  _k_vis[_qp] = _kf_UO.PermeabilityTensorCalculator(_current_elem->dim()) / _fp_UO.mu(_P[_qp], _T[_qp]);
  _H_Kernel_dt[_qp] = _beta_s + _fp_UO.beta(_P[_qp], _T[_qp]) * _n0;
  _rhof_g[_qp] = _fp_UO.rho(_P[_qp], _T[_qp]) * _gravity;
  _rhof[_qp] = _fp_UO.rho(_P[_qp], _T[_qp]);

  _scaling_lowerD[_qp] = LowerDScaling();

  if (_current_elem->dim() < _mesh.dimension())
    _k_vis[_qp].rotate(_rot_mat);
}
