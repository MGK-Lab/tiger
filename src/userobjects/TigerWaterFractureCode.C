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

#include "TigerWaterFractureCode.h"

registerMooseObject("FluidPropertiesApp", TigerWaterFractureCode);

template <>
InputParameters
validParams<TigerWaterFractureCode>()
{
  InputParameters params = validParams<SinglePhaseFluidProperties>();
  params.addParam<Real>("molar_mass", 1.8E-2,
        "Constant molar mass of the fluid (kg/mol)");
  params.addParam<Real>("thermal_expansion", 2.14E-4,
        "Constant coefficient of thermal expansion (1/K)");
  params.addParam<Real>("cv", 4186.0,
        "Constant specific heat capacity at constant volume (J/kg/K)");
  params.addParam<Real>("cp", 4194.0,
        "Constant specific heat capacity at constant pressure (J/kg/K)");
  params.addRangeCheckedParam<Real>("bulk_modulus", 2.0E9,
        "bulk_modulus>0", "Constant bulk modulus (Pa)");
  params.addParam<Real>("thermal_conductivity", 0.6,
        "Constant thermal conductivity (W/m/K)");
  params.addParam<Real>("specific_entropy", 300.0,
        "Constant specific entropy (J/kg/K)");
  params.addParam<Real>("henry_constant", 0.0,
        "Henry constant for dissolution in water");
  params.addParam<Real>("porepressure_coefficient", 1.0,
        "The enthalpy is internal_energy + P / density * "
        "porepressure_coefficient.  Physically this should be 1.0, "
        "but analytic solutions are simplified when it is zero");
  params.addClassDescription("Fluid properties for a simple fluid with "
        " with variable density and viscosity based on FRACture code");
  return params;
}

TigerWaterFractureCode::TigerWaterFractureCode(const InputParameters & parameters)
  : SinglePhaseFluidProperties(parameters),
    _molar_mass(getParam<Real>("molar_mass")),
    _thermal_expansion(getParam<Real>("thermal_expansion")),
    _cv(getParam<Real>("cv")),
    _cp(getParam<Real>("cp")),
    _bulk_modulus(getParam<Real>("bulk_modulus")),
    _thermal_conductivity(getParam<Real>("thermal_conductivity")),
    _specific_entropy(getParam<Real>("specific_entropy")),
    _henry_constant(getParam<Real>("henry_constant")),
    _pp_coeff(getParam<Real>("porepressure_coefficient"))
{
}

TigerWaterFractureCode::~TigerWaterFractureCode() {}

std::string
TigerWaterFractureCode::fluidName() const
{
  return "Water properties based on FRACture code";
}

Real
TigerWaterFractureCode::molarMass() const
{
  return _molar_mass;
}

Real TigerWaterFractureCode::beta_from_p_T(Real /*pressure*/, Real /*temperature*/) const
{
  return _thermal_expansion;
}

Real TigerWaterFractureCode::cp_from_p_T(Real /*pressure*/, Real /*temperature*/) const { return _cp; }

Real TigerWaterFractureCode::cv_from_p_T(Real /*pressure*/, Real /*temperature*/) const { return _cv; }

Real
TigerWaterFractureCode::c_from_p_T(Real pressure, Real temperature) const
{
  return std::sqrt(_bulk_modulus / rho_from_p_T(pressure, temperature));
}

Real TigerWaterFractureCode::k_from_p_T(Real /*pressure*/, Real /*temperature*/) const
{
  return _thermal_conductivity;
}

void
TigerWaterFractureCode::k_from_p_T(
    Real /*pressure*/, Real /*temperature*/, Real & k, Real & dk_dp, Real & dk_dT) const
{
  k = _thermal_conductivity;
  dk_dp = 0;
  dk_dT = 0;
}

Real TigerWaterFractureCode::s_from_p_T(Real /*pressure*/, Real /*temperature*/) const
{
  return _specific_entropy;
}

Real
TigerWaterFractureCode::rho_from_p_T(Real pressure, Real temperature) const
{
  if (pressure <0.0 || temperature <273.15)
    mooseError("The pressure field has a negative value so the EOS is unable to calculate density.\n");
  Real _a = -9.9559 + 7.0845*std::exp(-1.638e-4*(temperature-273.15))+3.909*std::exp(2.551e-10*pressure);
  return (-3.033405 + 10.128163*_a - 8.750567*_a*_a + 2.663107*_a*_a*_a)*1.0e3;
}

void
TigerWaterFractureCode::rho_from_p_T(
    Real pressure, Real temperature, Real & rho, Real & drho_dp, Real & drho_dT) const
{
  rho = this->rho_from_p_T(pressure, temperature);
  Real _a = -9.9559 + 7.0845*std::exp(-1.638e-4*(temperature-273.15))+3.909*std::exp(2.551e-10*pressure);
  drho_dp = (10.128163 - 17.501134*_a + 7.989321*_a*_a)*9.971859e-7*std::exp(2.551e-10*pressure);
  drho_dT = -(10.128163 - 17.501134*_a + 7.989321*_a*_a)*1.1604411*std::exp(-1.638e-4*(temperature-273.15));
}

Real
TigerWaterFractureCode::e_from_p_T(Real /*pressure*/, Real temperature) const
{
  return _cv * temperature;
}

void
TigerWaterFractureCode::e_from_p_T(
    Real pressure, Real temperature, Real & e, Real & de_dp, Real & de_dT) const
{
  e = this->e_from_p_T(pressure, temperature);
  de_dp = 0.0;
  de_dT = _cv;
}

void
TigerWaterFractureCode::rho_e_dpT(Real pressure,
                                 Real temperature,
                                 Real & rho,
                                 Real & drho_dp,
                                 Real & drho_dT,
                                 Real & e,
                                 Real & de_dp,
                                 Real & de_dT) const
{
  Real density, ddensity_dp, ddensity_dT;
  rho_from_p_T(pressure, temperature, density, ddensity_dp, ddensity_dT);
  rho = density;
  drho_dp = ddensity_dp;
  drho_dT = ddensity_dT;

  Real energy, denergy_dp, denergy_dT;
  e_from_p_T(pressure, temperature, energy, denergy_dp, denergy_dT);
  e = energy;
  de_dp = denergy_dp;
  de_dT = denergy_dT;
}

Real TigerWaterFractureCode::mu_from_p_T(Real /*pressure*/, Real temperature) const
{
  return 2.4e-5*std::pow(10.0, 248.37/(temperature-140));
}

void
TigerWaterFractureCode::mu_from_p_T(
    Real pressure, Real temperature, Real & mu, Real & dmu_dp, Real & dmu_dT) const
{
  mu = this->mu_from_p_T(pressure, temperature);
  dmu_dp = 0.0;
  dmu_dT = mu*-571.893/std::pow((temperature-140),2.0);
}

void
TigerWaterFractureCode::rho_mu(Real pressure, Real temperature, Real & rho, Real & mu) const
{
  rho = this->rho_from_p_T(pressure, temperature);
  mu = this->mu_from_p_T(pressure, temperature);
}

void
TigerWaterFractureCode::rho_mu_dpT(Real pressure,
                                  Real temperature,
                                  Real & rho,
                                  Real & drho_dp,
                                  Real & drho_dT,
                                  Real & mu,
                                  Real & dmu_dp,
                                  Real & dmu_dT) const
{
  this->rho_from_p_T(pressure, temperature, rho, drho_dp, drho_dT);
  this->mu_from_p_T(pressure, temperature, mu, dmu_dp, dmu_dT);
}

Real
TigerWaterFractureCode::h_from_p_T(Real pressure, Real temperature) const
{
  return e_from_p_T(pressure, temperature) + _pp_coeff * pressure / rho_from_p_T(pressure, temperature);
}

void
TigerWaterFractureCode::h_from_p_T(
    Real pressure, Real temperature, Real & h, Real & dh_dp, Real & dh_dT) const
{
  h = this->h_from_p_T(pressure, temperature);

  Real density, ddensity_dp, ddensity_dT;
  rho_from_p_T(pressure, temperature, density, ddensity_dp, ddensity_dT);

  dh_dp = _pp_coeff / density - _pp_coeff * pressure * ddensity_dp / density / density;
  dh_dT = _cv - _pp_coeff * pressure * ddensity_dT / density / density;
}

Real TigerWaterFractureCode::henryConstant(Real /*temperature*/) const { return _henry_constant; }

void
TigerWaterFractureCode::henryConstant_dT(Real /*temperature*/, Real & Kh, Real & dKh_dT) const
{
  Kh = _henry_constant;
  dKh_dT = 0.0;
}
