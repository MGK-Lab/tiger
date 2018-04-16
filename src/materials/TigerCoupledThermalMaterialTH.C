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

#include "TigerCoupledThermalMaterialTH.h"
#include "MooseMesh.h"
#include "libmesh/quadrature.h"

registerMooseObject("TigerApp", TigerCoupledThermalMaterialTH);

template <>
InputParameters
validParams<TigerCoupledThermalMaterialTH>()
{
  InputParameters params = validParams<TigerMaterialGeneral>();
  params.addRequiredParam<Real>("specific_heat", "Specific heat of rock matrix (J/(kg K))");
  params.addRequiredParam<Real>("density", "density of rock matrix (kg/m^3)");
  params.addRequiredParam<Real>("porosity", "Porosity of rock matrix");
  params.addRequiredParam<std::vector<Real>>("lambda", "Initial thermal conductivity of rock matrix (W/(m K))");
  MooseEnum CT("isotropic=1 orthotropic=2 anisotropic=3");
  params.addRequiredParam<MooseEnum>("conductivity_type", CT, "Thermal conductivity distribution type [isotropic, orthotropic, anisotropic].");
  MooseEnum Mean("arithmetic=1 geometric=2");
  params.addParam<MooseEnum>("mean_calculation_type", Mean = "arithmetic", "Solid-liquid mixture thermal conductivity calculation method [arithmetic, geometric]");
  params.addParam<bool>("output_Pe_Cr_numbers", false , "calcuate Peclet and Courant numbers");
  params.addParam<bool>("has_supg", false , "Is Streameline Upwinding / Petrov Galerkin (SU/PG) activated?");
  MooseEnum EleLen("min=1 max=2 average=3 directional_min=4 directional_max=5 directional_average=6");
  params.addParam<MooseEnum>("supg_eff_length", EleLen = "min", "The characteristic element length for SU/PG.");
  MooseEnum Method("optimal=1 doubly_asymptotic=2 critical=3 transient_brooks=4 transient_tezduyar=5");
  params.addParam<MooseEnum>("supg_coeficient", Method = "optimal" , "The method for calculating SU/PG coefficent (tau)");
  params.addParam<Real>("supg_coeficient_scale", 1.0 , "The user defined factor to scale SU/PG coefficent (tau)");
  params.addClassDescription("Thermal properties for a fully coupled TH simulation");
  return params;
}

TigerCoupledThermalMaterialTH::TigerCoupledThermalMaterialTH(const InputParameters & parameters)
  : TigerMaterialGeneral(parameters),
    _ct(getParam<MooseEnum>("conductivity_type")),
    _mean(getParam<MooseEnum>("mean_calculation_type")),
    _lambda0(getParam<std::vector<Real>>("lambda")),
    _cp0(getParam<Real>("specific_heat")),
    _rho0(getParam<Real>("density")),
    _n0(getParam<Real>("porosity")),
    _has_PeCr(getParam<bool>("output_Pe_Cr_numbers")),
    _has_supg(getParam<bool>("has_supg")),
    _effective_length(getParam<MooseEnum>("supg_eff_length")),
    _method(getParam<MooseEnum>("supg_coeficient")),
    _supg_scale(getParam<Real>("supg_coeficient_scale")),
    _gradient_pore_press(coupledGradient("pressure")),
    _k_vis(getMaterialProperty<RankTwoTensor>("permeability_by_viscosity")),
    _rhof_g(getMaterialProperty<RealVectorValue>("rho_times_gravity")),
    _Pe(_has_PeCr ? &declareProperty<Real>("peclet_number") : NULL),
    _Cr(_has_PeCr ? &declareProperty<Real>("courant_number") : NULL),
    _lambda_sf(declareProperty<RankTwoTensor>("conductivity_mixture")),
    _T_Kernel_dt(declareProperty<Real>("T_Kernel_dt_coefficient")),
    _scaling_lowerD(declareProperty<Real>("lowerD_scale_factor_t")),
    _rho_cp_f(declareProperty<Real>("fluid_thermal_capacity")),
    _SUPG_ind(declareProperty<bool>("supg_indicator")),
    _dv(declareProperty<RealVectorValue>("darcy_velocity")),
    _SUPG_p(declareProperty<RealVectorValue>("petrov_supg_p_function"))
{
}

void
TigerCoupledThermalMaterialTH::computeProperties()
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
TigerCoupledThermalMaterialTH::computeQpProperties()
{
  _T_Kernel_dt[_qp] = (1.0-_n0)*_rho0*_cp0 + _fp_UO.rho(_P[_qp], _T[_qp])*_fp_UO.cp(_P[_qp], _T[_qp])*_n0;

  ConductivityTensorCalculator(_n0, _fp_UO.lambda(_P[_qp], _T[_qp]), _lambda0, _ct, _mean, _current_elem->dim());
  _lambda_sf     [_qp] = _lambda_sf_tensor;

  _scaling_lowerD[_qp] = LowerDScaling();

  if (_current_elem->dim() < _mesh.dimension())
    _lambda_sf[_qp].rotate(_rot_mat);

  _rho_cp_f[_qp] = _fp_UO.rho(_P[_qp], _T[_qp])*_fp_UO.cp(_P[_qp], _T[_qp]);

  _dv[_qp] = -_k_vis[_qp] * (_gradient_pore_press[_qp] - _rhof_g[_qp]);

  Real norm_v, h_ele, alpha, lambda;

  if (_has_supg || _has_PeCr)
  {
    norm_v = _dv[_qp].norm();
    h_ele = EEL(_current_elem).norm();

    if (_lambda_sf_real == 0)
    {
      lambda = 0.0;
      alpha = 1.0e10;
    }
    else
    {
      lambda = _lambda_sf_real / _T_Kernel_dt[_qp];
      alpha = 0.5 * norm_v * h_ele / lambda;
    }

    if (_has_PeCr)
    {
      (*_Pe)[_qp] = alpha;
      (*_Cr)[_qp] = norm_v * _dt / h_ele;
    }
  }

  if (_has_supg && _dv[_qp].norm()!=0.0) // should be multiplied by gradient of the test function to build the Petrov P function
  {
    _SUPG_p[_qp] = _supg_scale * tau(norm_v, alpha, lambda, _dt, h_ele, _dv[_qp], EEL(_current_elem)) * _dv[_qp];
    _SUPG_ind[_qp] = true;
  }
  else
    _SUPG_ind[_qp] = false;
}

void
TigerCoupledThermalMaterialTH::ConductivityTensorCalculator(Real const & n, Real const & lambda_f, std::vector<Real> lambda_s, MooseEnum conductivity_type, MooseEnum mean_type, int dim)
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


Real
TigerCoupledThermalMaterialTH::tau(Real & norm_v, Real & alpha, Real & diff, Real & dt, Real & h_ele, const RealVectorValue & v, const RealVectorValue & h) const
{
  Real tau = 0.0;

  if (_effective_length<4)
    switch (_method)
    {
      case 1:
        tau += Optimal(alpha) * h_ele / (2.0 * norm_v);
        break;
      case 2:
        tau += DoublyAsymptotic(alpha) * h_ele / (2.0 * norm_v);
        break;
      case 3:
        tau += Critical(alpha) * h_ele / (2.0 * norm_v);
        break;
      case 4: // Brooks & Hughes 1982
        tau += Optimal(alpha) * h_ele / (sqrt(15.0) * norm_v);
        break;
      case 5: // Tezduyar & Osawa 2000
        tau += Temporal(v, h, diff, dt);
        break;
    }
  else
  {
    RealVectorValue a;
    a = 0.5 * RealVectorValue( std::fabs(v(0)*h(0)), std::fabs(v(1)*h(1)), std::fabs(v(2)*h(2)) ) / diff;
    //
    // std::cout<<a(0)<<" , "<<a(1)<<" , "<<a(2)<<std::endl;

    switch (_method)
    {
      case 1:
        tau = Optimal(a(0)) * std::fabs(h(0) * v(0)) +
              Optimal(a(1)) * std::fabs(h(1) * v(1)) +
              Optimal(a(2)) * std::fabs(h(2) * v(2));
        tau *= 0.5 * v.norm_sq();
        break;
      case 2:
        tau = DoublyAsymptotic(a(0)) * std::fabs(h(0) * v(0)) +
              DoublyAsymptotic(a(1)) * std::fabs(h(1) * v(1)) +
              DoublyAsymptotic(a(2)) * std::fabs(h(2) * v(2));
        tau *= 0.5 * v.norm_sq();
        break;
      case 3:
        tau = Critical(a(0)) * std::fabs(h(0) * v(0)) +
              Critical(a(1)) * std::fabs(h(1) * v(1)) +
              Critical(a(2)) * std::fabs(h(2) * v(2));
        tau *= 0.5 * v.norm_sq();
        break;
      case 4: // Brooks & Hughes 1982
        tau = Optimal(a(0)) * std::fabs(h(0) * v(0)) +
              Optimal(a(1)) * std::fabs(h(1) * v(1)) +
              Optimal(a(2)) * std::fabs(h(2) * v(2));
        tau /= sqrt(15.0) * v.norm_sq();
        break;
      case 5: // Tezduyar & Osawa 2000
        tau += Temporal(v, h, diff, dt);
        break;
    }
  }

  return tau;
}

Real
TigerCoupledThermalMaterialTH::Optimal(Real alpha) const
{
  Real s = 0.0;
  if (alpha < 0.01)
    s = alpha * (1.0 / 3.0 + alpha * alpha * (-1.0 / 45.0 + 18.0 / 8505.0 * alpha * alpha)); //taylor expansion
  else
    s = 1.0 / std::tanh(alpha) - 1.0 / alpha;
  return s;
}

Real
TigerCoupledThermalMaterialTH::Temporal(const RealVectorValue & v, const RealVectorValue & h, Real diff, Real dt) const
{
  Real s1, s2, s3;
  if (_effective_length<4)
  {
    s1 = 2.0 * v.norm() / h.norm();
    s2 = (dt!=0.0 ? 2.0 / dt : 0.0);
    s3 = 4.0 * diff / (h.norm() * h.norm());
  }
  else
  {
    s1 = 2.0 * ( std::fabs(v(0) / h(0)) + std::fabs(v(1) / h(1)) + std::fabs(v(2) / h(2)));
    s2 = (dt!=0.0 ? 2.0 / dt : 0.0);
    s3 = 4.0 * diff * (1.0 / (h(0) * h(0)) + 1.0 / (h(1) * h(1)) + 1.0 / (h(2) * h(2)));
  }

  return (1.0 / sqrt(s1 * s1 + s2 * s2 + s3 * s3));
}

Real
TigerCoupledThermalMaterialTH::DoublyAsymptotic(Real alpha) const
{
  Real s = 0.0;
  if (alpha <= 3.0)
    s = alpha / 3.0;
  else
    s = 1.0;
  return s;
}

Real
TigerCoupledThermalMaterialTH::Critical(Real alpha) const
{
  Real s = 0.0;
  if (alpha <= 1.0)
    s = 0.0;
  else
    s = 1.0 - 1.0 / alpha;
  return s;
}

RealVectorValue
TigerCoupledThermalMaterialTH::EEL(const Elem * ele) const
{
  RealVectorValue L;
  L.zero();
  if (ele->dim() == 1)
    L(0) += ele->volume();
  else
  {
    switch (_effective_length)
    {
      case 1: // min
        L(0) += ele->hmin();
        break;
      case 2: // max
        L(0) += ele->hmax();
        break;
      case 3: // average
        L(0) += 0.5 * (ele->hmin() + ele->hmax());
        break;
      case 4: // actual min
        ActualEEL(ele, L, _effective_length);
        break;
      case 5: // actual max
        ActualEEL(ele, L, _effective_length);
        break;
      case 6: // actual average
        ActualEEL(ele, L, _effective_length);
        break;
    }
  }
  return L;
}

void
TigerCoupledThermalMaterialTH::ActualEEL(const Elem * ele, RealVectorValue & l, const unsigned & ind) const
{
  if (ind == 4)
    l = RealVectorValue(std::numeric_limits<Real>::max());

  Real counter = 0;

  for (unsigned int n_outer=0; n_outer<ele->n_vertices(); n_outer++)
    for (unsigned int n_inner=n_outer+1; n_inner<ele->n_vertices(); n_inner++)
    {
      const Point diff = (ele->point(n_outer) - ele->point(n_inner));
      switch (ind)
      {
        case 4: // min
          l(0) = std::min(l(0), std::fabs(diff(0)));
          l(1) = std::min(l(1), std::fabs(diff(1)));
          l(2) = std::min(l(2), std::fabs(diff(2)));
          break;
        case 5: // max
          l(0) = std::max(l(0), std::fabs(diff(0)));
          l(1) = std::max(l(1), std::fabs(diff(1)));
          l(2) = std::max(l(2), std::fabs(diff(2)));
          break;
        case 6: // average
          l(0) += std::fabs(diff(0));
          l(1) += std::fabs(diff(1));
          l(2) += std::fabs(diff(2));
          break;
      }
      counter++;
    }
    if (ind == 6) l /= counter;
}
