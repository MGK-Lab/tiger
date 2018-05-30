//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "TigerWaterConst.h"

registerMooseObject("FluidPropertiesApp", TigerWaterConst);

template <>
InputParameters
validParams<TigerWaterConst>()
{
  InputParameters params = validParams<SinglePhaseFluidPropertiesPT>();
  params.addParam<Real>("molar_mass", 1.8E-2, "Constant molar mass of the fluid (kg/mol)");
  params.addParam<Real>(
      "thermal_expansion", 2.14E-4, "Constant coefficient of thermal expansion (1/K)");
  params.addParam<Real>(
      "cv", 4186.0, "Constant specific heat capacity at constant volume (J/kg/K)");
  params.addParam<Real>(
      "cp", 4194.0, "Constant specific heat capacity at constant pressure (J/kg/K)");
  params.addRangeCheckedParam<Real>(
      "bulk_modulus", 2.0E9, "bulk_modulus>0", "Constant bulk modulus (Pa)");
  params.addParam<Real>("thermal_conductivity", 0.6, "Constant thermal conductivity (W/m/K)");
  params.addParam<Real>("specific_entropy", 300.0, "Constant specific entropy (J/kg/K)");
  params.addParam<Real>("henry_constant", 0.0, "Henry constant for dissolution in water");
  params.addParam<Real>("porepressure_coefficient",
                        1.0,
                        "The enthalpy is internal_energy + P / density * "
                        "porepressure_coefficient.  Physically this should be 1.0, "
                        "but analytic solutions are simplified when it is zero");
  params.addParam<Real>("viscosity", 1.0E-3, "Constant dynamic viscosity (Pa.s)");
  params.addParam<Real>("density", 1000.0, "Constant density");
  params.addClassDescription("Fluid properties for water");
  return params;
}

TigerWaterConst::TigerWaterConst(const InputParameters & parameters)
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
    _density(getParam<Real>("density"))
{
}

TigerWaterConst::~TigerWaterConst() {}

std::string
TigerWaterConst::fluidName() const
{
  return "Constant Water properties";
}

Real
TigerWaterConst::molarMass() const
{
  return _molar_mass;
}

Real TigerWaterConst::beta(Real /*pressure*/, Real /*temperature*/) const
{
  return _thermal_expansion;
}

Real TigerWaterConst::cp(Real /*pressure*/, Real /*temperature*/) const { return _cp; }

Real TigerWaterConst::cv(Real /*pressure*/, Real /*temperature*/) const { return _cv; }

Real
TigerWaterConst::c(Real pressure, Real temperature) const
{
  return std::sqrt(_bulk_modulus / rho(pressure, temperature));
}

Real TigerWaterConst::k(Real /*pressure*/, Real /*temperature*/) const
{
  return _thermal_conductivity;
}

void
TigerWaterConst::k_dpT(
    Real /*pressure*/, Real /*temperature*/, Real & k, Real & dk_dp, Real & dk_dT) const
{
  k = _thermal_conductivity;
  dk_dp = 0;
  dk_dT = 0;
}

Real TigerWaterConst::s(Real /*pressure*/, Real /*temperature*/) const
{
  return _specific_entropy;
}

Real
TigerWaterConst::rho(Real /*pressure*/, Real /*temperature*/) const
{
  return _density;
}

void
TigerWaterConst::rho_dpT(
    Real pressure, Real temperature, Real & rho, Real & drho_dp, Real & drho_dT) const
{
  rho = this->rho(pressure, temperature);
  drho_dp = 0.0;
  drho_dT = 0.0;
}

Real
TigerWaterConst::e(Real /*pressure*/, Real temperature) const
{
  return _cv * temperature;
}

void
TigerWaterConst::e_dpT(
    Real pressure, Real temperature, Real & e, Real & de_dp, Real & de_dT) const
{
  e = this->e(pressure, temperature);
  de_dp = 0.0;
  de_dT = _cv;
}

void
TigerWaterConst::rho_e_dpT(Real pressure,
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

Real TigerWaterConst::mu(Real /*pressure*/, Real temperature) const
{
  return _viscosity;
}

void
TigerWaterConst::mu_dpT(
    Real pressure, Real temperature, Real & mu, Real & dmu_dp, Real & dmu_dT) const
{
  mu = this->mu(pressure, temperature);
  dmu_dp = 0.0;
  dmu_dT = 0.0;
}

void
TigerWaterConst::rho_mu(Real pressure, Real temperature, Real & rho, Real & mu) const
{
  rho = this->rho(pressure, temperature);
  mu = this->mu(pressure, temperature);
}

void
TigerWaterConst::rho_mu_dpT(Real pressure,
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
TigerWaterConst::h(Real pressure, Real temperature) const
{
  return e(pressure, temperature) + _pp_coeff * pressure / rho(pressure, temperature);
}

void
TigerWaterConst::h_dpT(
    Real pressure, Real temperature, Real & h, Real & dh_dp, Real & dh_dT) const
{
  h = this->h(pressure, temperature);

  Real density, ddensity_dp, ddensity_dT;
  rho_dpT(pressure, temperature, density, ddensity_dp, ddensity_dT);

  dh_dp = _pp_coeff / density - _pp_coeff * pressure * ddensity_dp / density / density;
  dh_dT = _cv - _pp_coeff * pressure * ddensity_dT / density / density;
}

Real TigerWaterConst::henryConstant(Real /*temperature*/) const { return _henry_constant; }

void
TigerWaterConst::henryConstant_dT(Real /*temperature*/, Real & Kh, Real & dKh_dT) const
{
  Kh = _henry_constant;
  dKh_dT = 0.0;
}
