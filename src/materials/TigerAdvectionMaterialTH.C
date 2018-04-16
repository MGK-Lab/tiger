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

#include "TigerAdvectionMaterialTH.h"
#include "MooseMesh.h"
#include "libmesh/quadrature.h"
#include "MooseParsedVectorFunction.h"

template <>
InputParameters
validParams<TigerAdvectionMaterialTH>()
{
  InputParameters params = validParams<TigerMaterialGeneral>();
  params.addParam<bool>("output_Pe_Cr_numbers", false , "calcuate Peclet and Courant numbers");
  params.addParam<bool>("has_supg", false , "Is Streameline Upwinding / Petrov Galerkin (SU/PG) activated?");
  params.addParam<bool>("is_supg_consistent", true , "Is it a Consistent SU/PG?");
  MooseEnum EleLen("min=1 max=2 average=3 directional_min=4 directional_max=5 directional_average=6");
  params.addParam<MooseEnum>("supg_eff_length", EleLen = "min", "The characteristic element length for SU/PG.");
  MooseEnum Method("optimal=1 doubly_asymptotic=2 critical=3 transient_brooks=4 transient_tezduyar=5");
  params.addParam<MooseEnum>("supg_coeficient", Method = "optimal" , "The method for calculating SU/PG coefficent (tau)");
  params.addParam<Real>("supg_coeficient_scale", 1.0 , "The user defined factor to scale SU/PG coefficent (tau)");
  params.addParam<FunctionName>("user_velocity", "a vector function to define the velocity manually (decoupled)");
  params.addClassDescription("Advection related properties");
  return params;
}

TigerAdvectionMaterialTH::TigerAdvectionMaterialTH(const InputParameters & parameters)
  : TigerMaterialGeneral(parameters),
  _has_user_vel(parameters.isParamSetByUser("user_velocity")),
  _has_PeCr(getParam<bool>("output_Pe_Cr_numbers")),
  _has_supg(getParam<bool>("has_supg")),
  _pure_advection(!hasMaterialProperty<Real>("conductivity_mixture_equivalent")),
  _is_supg_consistent(getParam<bool>("is_supg_consistent")),
  _effective_length(getParam<MooseEnum>("supg_eff_length")),
  _method(getParam<MooseEnum>("supg_coeficient")),
  _gradient_pore_press(coupledGradient("pressure")),
  _dv(declareProperty<RealVectorValue>("darcy_velocity")),
  _SUPG_p(declareProperty<RealVectorValue>("petrov_supg_p_function")),
  _Pe(_has_PeCr ? &declareProperty<Real>("peclet_number") : NULL),
  _Cr(_has_PeCr ? &declareProperty<Real>("courant_number") : NULL),
  _rho_cp_f(declareProperty<Real>("fluid_thermal_capacity")),
  _scaling_lowerD(declareProperty<Real>("lowerD_scale_factor_t")),
  _vel_func(_has_user_vel ? &getFunction("user_velocity") : NULL),
  _supg_scale(getParam<Real>("supg_coeficient_scale")),
  _SUPG_ind(declareProperty<bool>("supg_indicator")),
  _SUPG_consistency_ind(declareProperty<bool>("supg_consistent_indicator"))
{
  if (!_pure_advection)
  {
    _lambda_sf_eq = &getMaterialProperty<Real>("conductivity_mixture_equivalent");
    _T_Kernel_dt = &getMaterialProperty<Real>("T_Kernel_dt_coefficient");
  }

  if (!_has_user_vel)
  {
    _k_vis = &getMaterialProperty<RankTwoTensor>("permeability_by_viscosity");
    _rhof_g = &getMaterialProperty<RealVectorValue>("rho_times_gravity");
  }
}

void
TigerAdvectionMaterialTH::computeQpProperties()
{
  _scaling_lowerD[_qp] = LowerDScaling();

  if (!_pure_advection)
    _rho_cp_f[_qp] = _fp_UO.rho(_P[_qp], _T[_qp])*_fp_UO.cp(_P[_qp], _T[_qp]);
  else
    _rho_cp_f[_qp] = 1.0;

  if (_has_user_vel)
    _dv[_qp] = _vel_func->vectorValue(_t, _q_point[_qp]);
  else
    _dv[_qp] = -(*_k_vis)[_qp] * (_gradient_pore_press[_qp] - (*_rhof_g)[_qp]);

  Real norm_v, h_ele, alpha, lambda;

  if (_has_supg || _has_PeCr)
  {
    norm_v = _dv[_qp].norm();
    h_ele = EEL(_current_elem).norm();
    if (_pure_advection)
    {
      alpha = 1.0e10;
      lambda = 0.0;
    }
    else
    {
      lambda = (*_lambda_sf_eq)[_qp] / (*_T_Kernel_dt)[_qp];
      alpha = 0.5 * norm_v * h_ele / lambda;
    }
  }

  if (_has_PeCr)
  {
    (*_Pe)[_qp] = alpha;
    (*_Cr)[_qp] = norm_v * _dt / h_ele;
  }


  if (_has_supg && _dv[_qp].norm()!=0.0) // should be multiplied by gradient of the test function to build the Petrov P function
  {
    _SUPG_p[_qp] = _supg_scale * tau(norm_v, alpha, lambda, _dt, h_ele, _dv[_qp], EEL(_current_elem)) * _dv[_qp];

    if (_is_supg_consistent)
      _SUPG_consistency_ind[_qp] = true;

    _SUPG_ind[_qp] = true;
  }
  else
  {
    _SUPG_consistency_ind[_qp] = false;
    _SUPG_ind[_qp] = false;
  }
}

Real
TigerAdvectionMaterialTH::tau(Real & norm_v, Real & alpha, Real & diff, Real & dt, Real & h_ele, const RealVectorValue & v, const RealVectorValue & h) const
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
TigerAdvectionMaterialTH::Optimal(Real alpha) const
{
  Real s = 0.0;
  if (alpha < 0.01)
    s = alpha * (1.0 / 3.0 + alpha * alpha * (-1.0 / 45.0 + 18.0 / 8505.0 * alpha * alpha)); //taylor expansion
  else
    s = 1.0 / std::tanh(alpha) - 1.0 / alpha;
  return s;
}

Real
TigerAdvectionMaterialTH::Temporal(const RealVectorValue & v, const RealVectorValue & h, Real diff, Real dt) const
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
TigerAdvectionMaterialTH::DoublyAsymptotic(Real alpha) const
{
  Real s = 0.0;
  if (alpha <= 3.0)
    s = alpha / 3.0;
  else
    s = 1.0;
  return s;
}

Real
TigerAdvectionMaterialTH::Critical(Real alpha) const
{
  Real s = 0.0;
  if (alpha <= 1.0)
    s = 0.0;
  else
    s = 1.0 - 1.0 / alpha;
  return s;
}

RealVectorValue
TigerAdvectionMaterialTH::EEL(const Elem * ele) const
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
TigerAdvectionMaterialTH::ActualEEL(const Elem * ele, RealVectorValue & l, const unsigned & ind) const
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
