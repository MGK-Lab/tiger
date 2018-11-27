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

#include "TigerIdealWater.h"

registerMooseObject("FluidPropertiesApp", TigerIdealWater);

template <>
InputParameters
validParams<TigerIdealWater>()
{
  InputParameters params = validParams<SinglePhaseFluidPropertiesPT>();
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
  params.addParam<Real>("viscosity", 1.0E-3,
        "Constant dynamic viscosity (Pa.s)");
  params.addParam<Real>("reference_density", 998.29,
        "Density at the reference pressure and temperature (kg/m^3)");
  params.addParam<Real>("reference_temperature", 293.15,
        "Reference temperature (K)");
  params.addParam<Real>("reference_pressure", 101325,
        "Reference pressure (Pa)");
  params.addClassDescription("Water properties with an exponential EOS and"
        " a constant viscosity");
  return params;
}

TigerIdealWater::TigerIdealWater(const InputParameters & parameters)
  : SinglePhaseFluidPropertiesPT(parameters),
    _molar_mass(getParam<Real>("molar_mass")),
    _thermal_expansion(getParam<Real>("thermal_expansion")),
    _cv(getParam<Real>("cv")),
    _cp(getParam<Real>("cp")),
    _bulk_modulus(getParam<Real>("bulk_modulus")),
    _thermal_conductivity(getParam<Real>("thermal_conductivity")),
    _specific_entropy(getParam<Real>("specific_entropy")),
    _henry_constant(getParam<Real>("henry_constant")),
    _pp_coeff(getParam<Real>("porepressure_coefficient")),
    _viscosity(getParam<Real>("viscosity")),
    _density_ref(getParam<Real>("reference_density")),
    _T_ref(getParam<Real>("reference_temperature")),
    _P_ref(getParam<Real>("reference_pressure"))
{
}

TigerIdealWater::~TigerIdealWater() {}

std::string
TigerIdealWater::fluidName() const
{
  return "Ideal Water properties";
}

Real
TigerIdealWater::molarMass() const
{
  return _molar_mass;
}

Real TigerIdealWater::beta(Real /*pressure*/, Real /*temperature*/) const
{
  return _thermal_expansion;
}

Real TigerIdealWater::cp(Real /*pressure*/, Real /*temperature*/) const { return _cp; }

Real TigerIdealWater::cv(Real /*pressure*/, Real /*temperature*/) const { return _cv; }

Real
TigerIdealWater::c(Real pressure, Real temperature) const
{
  return std::sqrt(_bulk_modulus / rho(pressure, temperature));
}

Real TigerIdealWater::k(Real /*pressure*/, Real /*temperature*/) const
{
  return _thermal_conductivity;
}

void
TigerIdealWater::k_dpT(
    Real /*pressure*/, Real /*temperature*/, Real & k, Real & dk_dp, Real & dk_dT) const
{
  k = _thermal_conductivity;
  dk_dp = 0;
  dk_dT = 0;
}

Real TigerIdealWater::s(Real /*pressure*/, Real /*temperature*/) const
{
  return _specific_entropy;
}

Real
TigerIdealWater::rho(Real pressure, Real temperature) const
{
  return _density_ref * std::exp((pressure-_P_ref) / _bulk_modulus - _thermal_expansion * (temperature-_T_ref));
}

void
TigerIdealWater::rho_dpT(
    Real pressure, Real temperature, Real & rho, Real & drho_dp, Real & drho_dT) const
{
  rho = this->rho(pressure, temperature);
  drho_dp = rho / _bulk_modulus;
  drho_dT = -_thermal_expansion * rho;
}

Real
TigerIdealWater::e(Real /*pressure*/, Real temperature) const
{
  return _cv * temperature;
}

void
TigerIdealWater::e_dpT(
    Real pressure, Real temperature, Real & e, Real & de_dp, Real & de_dT) const
{
  e = this->e(pressure, temperature);
  de_dp = 0.0;
  de_dT = _cv;
}

void
TigerIdealWater::rho_e_dpT(Real pressure,
                                 Real temperature,
                                 Real & rho,
                                 Real & drho_dp,
                                 Real & drho_dT,
                                 Real & e,
                                 Real & de_dp,
                                 Real & de_dT) const
{
  Real density, ddensity_dp, ddensity_dT;
  rho_dpT(pressure, temperature, density, ddensity_dp, ddensity_dT);
  rho = density;
  drho_dp = ddensity_dp;
  drho_dT = ddensity_dT;

  Real energy, denergy_dp, denergy_dT;
  e_dpT(pressure, temperature, energy, denergy_dp, denergy_dT);
  e = energy;
  de_dp = denergy_dp;
  de_dT = denergy_dT;
}

Real TigerIdealWater::mu(Real /*pressure*/, Real temperature) const
{
  return _viscosity;
}

void
TigerIdealWater::mu_dpT(
    Real pressure, Real temperature, Real & mu, Real & dmu_dp, Real & dmu_dT) const
{
  mu = this->mu(pressure, temperature);
  dmu_dp = 0.0;
  dmu_dT = 0.0;
}

void
TigerIdealWater::rho_mu(Real pressure, Real temperature, Real & rho, Real & mu) const
{
  rho = this->rho(pressure, temperature);
  mu = this->mu(pressure, temperature);
}

void
TigerIdealWater::rho_mu_dpT(Real pressure,
                                  Real temperature,
                                  Real & rho,
                                  Real & drho_dp,
                                  Real & drho_dT,
                                  Real & mu,
                                  Real & dmu_dp,
                                  Real & dmu_dT) const
{
  this->rho_dpT(pressure, temperature, rho, drho_dp, drho_dT);
  this->mu_dpT(pressure, temperature, mu, dmu_dp, dmu_dT);
}

Real
TigerIdealWater::h(Real pressure, Real temperature) const
{
  return e(pressure, temperature) + _pp_coeff * pressure / rho(pressure, temperature);
}

void
TigerIdealWater::h_dpT(
    Real pressure, Real temperature, Real & h, Real & dh_dp, Real & dh_dT) const
{
  h = this->h(pressure, temperature);

  Real density, ddensity_dp, ddensity_dT;
  rho_dpT(pressure, temperature, density, ddensity_dp, ddensity_dT);

  dh_dp = _pp_coeff / density - _pp_coeff * pressure * ddensity_dp / density / density;
  dh_dT = _cv - _pp_coeff * pressure * ddensity_dT / density / density;
}

Real TigerIdealWater::henryConstant(Real /*temperature*/) const { return _henry_constant; }

void
TigerIdealWater::henryConstant_dT(Real /*temperature*/, Real & Kh, Real & dKh_dT) const
{
  Kh = _henry_constant;
  dKh_dT = 0.0;
}
