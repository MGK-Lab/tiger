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

#include "TigerRockMaterialT.h"
#include "MooseMesh.h"
#include "libmesh/quadrature.h"

template <>
InputParameters
validParams<TigerRockMaterialT>()
{
  InputParameters params = validParams<TigerMaterialGeneral>();
  params.addRequiredParam<Real>("specific_heat", "Specific heat of rock matrix (J/(kg K))");
  params.addRequiredParam<Real>("density", "density of rock matrix (kg/m^3)");
  params.addRequiredParam<Real>("porosity", "Porosity of rock matrix");
  MooseEnum CT("isotropic=1 orthotropic=2 anisotropic=3");
  params.addRequiredParam<MooseEnum>("conductivity_type", CT, "Thermal conductivity distribution type [isotropic, orthotropic, anisotropic].");
  MooseEnum Mean("arithmetic=1 geometric=2");
  params.addRequiredParam<MooseEnum>("mean_calculation_type", Mean, "Solid-liquid mixture thermal conductivity calculation method [arithmetic, geometric]");
  params.addRequiredParam<std::vector<Real>>("lambda", "Initial thermal conductivity of rock matrix (W/(m K))");
  params.addClassDescription("Thermal properties");
  return params;
}

TigerRockMaterialT::TigerRockMaterialT(const InputParameters & parameters)
  : TigerMaterialGeneral(parameters),
    _ct(getParam<MooseEnum>("conductivity_type")),
    _mean(getParam<MooseEnum>("mean_calculation_type")),
    _lambda0(getParam<std::vector<Real>>("lambda")),
    _cp0(getParam<Real>("specific_heat")),
    _rho0(getParam<Real>("density")),
    _n0(getParam<Real>("porosity")),
    _lambda_sf_eq(declareProperty<Real>("conductivity_mixture_equivalent")),
    _lambda_sf(declareProperty<RankTwoTensor>("conductivity_mixture")),
    _T_Kernel_dt(declareProperty<Real>("T_Kernel_dt_coefficient")),
    _scaling_lowerD(declareProperty<Real>("lowerD_scale_factor_t"))
{
}

void
TigerRockMaterialT::computeProperties()
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
TigerRockMaterialT::computeQpProperties()
{
  _T_Kernel_dt[_qp] = (1.0-_n0)*_rho0*_cp0 + _fp_UO.rho(_P[_qp], _T[_qp])*_fp_UO.cp(_P[_qp], _T[_qp])*_n0;

  ConductivityTensorCalculator(_n0, _fp_UO.lambda(_P[_qp], _T[_qp]), _lambda0, _ct, _mean, _current_elem->dim());
  _lambda_sf     [_qp] = _lambda_sf_tensor;
  _lambda_sf_eq  [_qp] = _lambda_sf_real;

  _scaling_lowerD[_qp] = LowerDScaling();

  if (_current_elem->dim() < _mesh.dimension())
    _lambda_sf[_qp].rotate(_rot_mat);
}

void
TigerRockMaterialT::ConductivityTensorCalculator(Real const & n, Real const & lambda_f, std::vector<Real> lambda_s, MooseEnum conductivity_type, MooseEnum mean_type, int dim)
{
  RealVectorValue lambda_x;
  RealVectorValue lambda_y;
  RealVectorValue lambda_z;

  if (mean_type==1)
  {
    if (dim == 1)
    {
      switch (conductivity_type)
      {
        case 1:
          if (lambda_s.size() != 1)
            mooseError("One input value is needed for isotropic distribution of thermal conductivity! You provided ", lambda_s.size(), " values.\n");
          _lambda_sf_tensor = (1.-n)*RankTwoTensor(lambda_s[0], 0., 0., 0., 0., 0.) + n*RankTwoTensor(lambda_f, 0., 0., 0., 0., 0.);
          break;
        case 2:
        case 3:
          mooseError("One dimensional elements cannot have non-isotropic thermal conductivity values.\n");
          break;
      }
    }
    else if (dim == 2)
    {
      switch (conductivity_type)
      {
        case 1:
          if (lambda_s.size() != 1)
            mooseError("One input value is needed for isotropic distribution of thermal conductivity! You provided ", lambda_s.size(), " values.\n");
          _lambda_sf_tensor = ((1.-n)*lambda_s[0] + n*lambda_f) * RankTwoTensor(1., 1., 0., 0., 0., 0.);
          break;
        case 2:
          if (lambda_s.size() != 2)
            mooseError("Two input values are needed for orthotropic distribution of thermal conductivity in two dimensional elements! You provided ", lambda_s.size(), " values.\n");
          _lambda_sf_tensor = (1.-n)*RankTwoTensor(lambda_s[0], lambda_s[1], 0., 0., 0., 0.) + n*lambda_f*RankTwoTensor(1., 1., 0., 0., 0., 0.);
          break;
        case 3:
          if (lambda_s.size() != 4)
            mooseError("Four input values are needed for anisotropic distribution of thermal conductivity in two dimensional elements! You provided ", lambda_s.size(), " values.\n");
          lambda_x = RealVectorValue(lambda_s[0], lambda_s[1], 0.0);
          lambda_y = RealVectorValue(lambda_s[2], lambda_s[3], 0.0);
          lambda_z = RealVectorValue(0.0  , 0.0  , 0.0);
          _lambda_sf_tensor = (1.-n)*RankTwoTensor(lambda_x, lambda_y, lambda_z) +  n*lambda_f*RankTwoTensor(1., 1., 0., 0., 0., 0.);
          break;
      }
    }
    else if (dim == 3)
    {
      switch (conductivity_type)
      {
        case 1:
          if (lambda_s.size() != 1)
            mooseError("One input value is needed for isotropic distribution of thermal conductivity! You provided ", lambda_s.size(), " values.\n");
          _lambda_sf_tensor = ((1.-n)*lambda_s[0] + n*lambda_f) * RankTwoTensor(1., 1., 1., 0., 0., 0.);
          break;

        case 2:
          if (lambda_s.size() != 3)
            mooseError("Three input values are needed for orthotropic distribution of thermal conductivity! You provided ", lambda_s.size(), " values.\n");
          _lambda_sf_tensor = (1.-n)*RankTwoTensor(lambda_s[0], lambda_s[1], lambda_s[2], 0., 0., 0.) + n*lambda_f*RankTwoTensor(1., 1., 1., 0., 0., 0.);
          break;

        case 3:
          if (lambda_s.size() != 9)
            mooseError("Nine input values are needed for anisotropic distribution of thermal conductivity! You provided ", lambda_s.size(), " values.\n");
          lambda_x = RealVectorValue(lambda_s[0], lambda_s[1], lambda_s[2]);
          lambda_y = RealVectorValue(lambda_s[3], lambda_s[4], lambda_s[5]);
          lambda_z = RealVectorValue(lambda_s[6], lambda_s[7], lambda_s[8]);
          _lambda_sf_tensor = (1.-n)*RankTwoTensor(lambda_x, lambda_y, lambda_z) +  n*lambda_f*RankTwoTensor(1., 1., 1., 0., 0., 0.);
          break;
      }
    }
  }
  else
  {
    if (dim == 1)
    {
      switch (conductivity_type)
      {
        case 1:
          if (lambda_s.size() != 1)
            mooseError("One input value is needed for isotropic distribution of thermal conductivity! You provided ", lambda_s.size(), " values.\n");
          _lambda_sf_tensor = RankTwoTensor(1., 0., 0., 0., 0., 0.) * std::pow(lambda_f,n)*std::pow(lambda_s[0],(1.-n));
        break;
        case 2:
        case 3:
          mooseError("One dimensional elements cannot have non-isotropic thermal conductivity values.\n");
          break;
      }
    }
    else if (dim == 2)
    {
      switch (conductivity_type)
      {
        case 1:
          if (lambda_s.size() != 1)
            mooseError("One input value is needed for isotropic distribution of thermal conductivity! You provided ", lambda_s.size(), " values.\n");
          _lambda_sf_tensor = RankTwoTensor(1., 1., 0., 0., 0., 0.) * std::pow(lambda_f,n)*std::pow(lambda_s[0],(1.-n));
          break;
        case 2:
          if (lambda_s.size() != 2)
            mooseError("Two input values are needed for orthotropic distribution of thermal conductivity in two dimensional elements! You provided ", lambda_s.size(), " values.\n");
          _lambda_sf_tensor = RankTwoTensor(std::pow(lambda_s[0],(1.-n)), std::pow(lambda_s[1],(1.-n)), 0., 0., 0., 0.) * std::pow(lambda_f,n);
          break;
        case 3:
          mooseError("Geometric mean for thermal conductivity of mixture is not available in anisotropic distribution.\n");
          break;
      }
    }
    else if (dim == 3)
    {
      switch (conductivity_type)
      {
        case 1:
          if (lambda_s.size() != 1)
            mooseError("One input value is needed for isotropic distribution of thermal conductivity! You provided ", lambda_s.size(), " values.\n");
          _lambda_sf_tensor = RankTwoTensor(1., 1., 1., 0., 0., 0.) * std::pow(lambda_f,n)*std::pow(lambda_s[0],(1.-n));
          break;

        case 2:
          if (lambda_s.size() != 3)
            mooseError("Three input values are needed for orthotropic distribution of thermal conductivity! You provided ", lambda_s.size(), " values.\n");
          _lambda_sf_tensor = RankTwoTensor(std::pow(lambda_s[0],(1.-n)), std::pow(lambda_s[1],(1.-n)), std::pow(lambda_s[2],(1.-n)), 0., 0., 0.) * std::pow(lambda_f,n);
          break;

        case 3:
          mooseError("Geometric mean for thermal conductivity of mixture is not available in anisotropic distribution.\n");
          break;
      }
    }
  }
  _lambda_sf_real = _lambda_sf_tensor.trace() / dim;
}
