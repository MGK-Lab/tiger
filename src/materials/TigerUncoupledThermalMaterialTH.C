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

#include "TigerUncoupledThermalMaterialTH.h"
#include "MooseMesh.h"
#include "libmesh/quadrature.h"

// registerMooseObject("TigerApp", TigerUncoupledThermalMaterialTH);

template <>
InputParameters
validParams<TigerUncoupledThermalMaterialTH>()
{
  InputParameters params = validParams<TigerMaterialGeneral>();
  params.addRequiredParam<Real>("specific_heat", "Specific heat of rock matrix (J/(kg K))");
  params.addRequiredParam<Real>("density", "density of rock matrix (kg/m^3)");
  params.addRequiredParam<std::vector<Real>>("lambda", "Initial thermal conductivity of rock matrix (W/(m K))");
  MooseEnum CT("isotropic=1 orthotropic=2 anisotropic=3");
  params.addRequiredParam<MooseEnum>("conductivity_type", CT, "Thermal conductivity distribution type [isotropic, orthotropic, anisotropic].");
  MooseEnum Mean("arithmetic=1 geometric=2");
  params.addParam<MooseEnum>("mean_calculation_type", Mean = "arithmetic", "Solid-liquid mixture thermal conductivity calculation method [arithmetic, geometric]");
  params.addParam<bool>("output_Pe_Cr_numbers", false , "calcuate Peclet and Courant numbers");
  params.addParam<bool>("has_supg", false , "Is Streameline Upwinding / Petrov Galerkin (SU/PG) activated?");
  params.addParam<Real>("supg_coeficient_scale", 1.0 , "The user defined factor to scale SU/PG coefficent (tau)");
  params.addClassDescription("Thermal properties for TH simulation using a provided velocity vector");
  params.addParam<FunctionName>("user_velocity", 0.0, "a vector function to define the velocity field");
  return params;
}

TigerUncoupledThermalMaterialTH::TigerUncoupledThermalMaterialTH(const InputParameters & parameters)
  : TigerMaterialGeneral(parameters),
    _ct(getParam<MooseEnum>("conductivity_type")),
    _mean(getParam<MooseEnum>("mean_calculation_type")),
    _lambda0(getParam<std::vector<Real>>("lambda")),
    _cp0(getParam<Real>("specific_heat")),
    _rho0(getParam<Real>("density")),
    _has_PeCr(getParam<bool>("output_Pe_Cr_numbers")),
    _has_supg(getParam<bool>("has_supg")),
    _supg_scale(getParam<Real>("supg_coeficient_scale")),
    _vel_func(getFunction("user_velocity")),
    _Pe((_has_PeCr || _has_supg) ? &declareProperty<Real>("peclet_number") : NULL),
    _Cr((_has_PeCr || _has_supg) ? &declareProperty<Real>("courant_number") : NULL),
    _lambda_sf(declareProperty<RankTwoTensor>("conductivity_mixture")),
    _T_Kernel_dt(declareProperty<Real>("T_Kernel_dt_coefficient")),
    _rho_cp_f(declareProperty<Real>("fluid_thermal_capacity")),
    _SUPG_ind(declareProperty<bool>("supg_indicator")),
    _dv(declareProperty<RealVectorValue>("darcy_velocity")),
    _SUPG_p(declareProperty<RealVectorValue>("petrov_supg_p_function")),
    _n(getMaterialProperty<Real>("porosity")),
    _rot_mat(getMaterialProperty<RankTwoTensor>("lowerD_rotation_matrix"))
{
}

void
TigerUncoupledThermalMaterialTH::computeQpProperties()
{
  _T_Kernel_dt[_qp] = (1.0-_n[_qp])*_rho0*_cp0 + _fp_uo.rho(_P[_qp], _T[_qp])*_fp_uo.cp(_P[_qp], _T[_qp])*_n[_qp];

  ConductivityTensorCalculator(_n[_qp], _fp_uo.k(_P[_qp], _T[_qp]), _lambda0, _ct, _mean, _current_elem->dim());
  _lambda_sf[_qp] = _lambda_sf_tensor;

  if (_current_elem->dim() < _mesh.dimension())
    _lambda_sf[_qp].rotate(_rot_mat[_qp]);

  _rho_cp_f[_qp] = _fp_uo.rho(_P[_qp], _T[_qp])*_fp_uo.cp(_P[_qp], _T[_qp]);

  _dv[_qp] = _vel_func.vectorValue(_t, _q_point[_qp]);

  Real lambda = _lambda_sf_real / _T_Kernel_dt[_qp];

  if (_has_PeCr && !_has_supg)
    _supg_uo->PeCrNrsCalculator(lambda, _dt, _current_elem, _dv[_qp], (*_Pe)[_qp], (*_Cr)[_qp]);

  if (_has_supg) // should be multiplied by gradient of the test function to build the Petrov P function
  {
    _supg_uo->SUPGCalculator(lambda, _dt, _current_elem, _dv[_qp], _SUPG_p[_qp], (*_Pe)[_qp], (*_Cr)[_qp]);
    _SUPG_p[_qp] *= _supg_scale;
    _SUPG_ind[_qp] = true;
  }
  else
    _SUPG_ind[_qp] = false;
}

void
TigerUncoupledThermalMaterialTH::ConductivityTensorCalculator(Real const & n, Real const & lambda_f, std::vector<Real> lambda_s, MooseEnum conductivity_type, MooseEnum mean_type, int dim)
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
