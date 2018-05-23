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

#ifndef TIGERWATERFRACTURECODE_H
#define TIGERWATERFRACTURECODE_H

#include "SinglePhaseFluidPropertiesPT.h"

class TigerWaterFractureCode;

template <>
InputParameters validParams<TigerWaterFractureCode>();

class TigerWaterFractureCode : public SinglePhaseFluidPropertiesPT
{
public:
  TigerWaterFractureCode(const InputParameters & parameters);
  virtual ~TigerWaterFractureCode();

  /// Fluid name
  virtual std::string fluidName() const override;

  /// Molar mass (kg/mol)
  virtual Real molarMass() const override;

  /// Thermal expansion coefficient (1/K)
  virtual Real beta(Real pressure, Real temperature) const override;

  /// Isobaric specific heat capacity (J/kg/K)
  virtual Real cp(Real pressure, Real temperature) const override;

  /// Isochoric specific heat capacity (J/kg/K)
  virtual Real cv(Real pressure, Real temperature) const override;

  /// Speed of sound (m/s)
  virtual Real c(Real pressure, Real temperature) const override;

  /// Thermal conductivity (W/m/K)
  virtual Real k(Real pressure, Real temperature) const override;

  /// Thermal conductivity and its derivatives wrt pressure and temperature
  virtual void
  k_dpT(Real pressure, Real temperature, Real & k, Real & dk_dp, Real & dk_dT) const override;

  /// Specific entropy (J/kg/K)
  virtual Real s(Real pressure, Real temperature) const override;

  /// Density from pressure and temperature (kg/m^3)
  virtual Real rho(Real pressure, Real temperature) const override;

  /// Density from pressure and temperature and its derivatives wrt pressure and temperature
  virtual void rho_dpT(
      Real pressure, Real temperature, Real & rho, Real & drho_dp, Real & drho_dT) const override;

  /// Internal energy from pressure and temperature (J/kg)
  virtual Real e(Real pressure, Real temperature) const override;

  /// Internal energy and its derivatives wrt pressure and temperature
  virtual void
  e_dpT(Real pressure, Real temperature, Real & e, Real & de_dp, Real & de_dT) const override;

  /// Density and internal energy from pressure and temperature and derivatives wrt pressure and temperature
  virtual void rho_e_dpT(Real pressure,
                         Real temperature,
                         Real & rho,
                         Real & drho_dp,
                         Real & drho_dT,
                         Real & e,
                         Real & de_dp,
                         Real & de_dT) const override;

  virtual Real mu(Real pressure, Real temperature) const override;

  virtual void
  mu_dpT(Real pressure, Real temperature, Real & mu, Real & dmu_dp, Real & dmu_dT) const override;

  virtual void rho_mu(Real pressure, Real temperature, Real & rho, Real & mu) const override;

  virtual void rho_mu_dpT(Real pressure,
                          Real temperature,
                          Real & rho,
                          Real & drho_dp,
                          Real & drho_dT,
                          Real & mu,
                          Real & dmu_dp,
                          Real & dmu_dT) const override;

  /// Specific enthalpy (J/kg)
  virtual Real h(Real p, Real T) const override;

  /// Specific enthalpy and its derivatives
  virtual void
  h_dpT(Real pressure, Real temperature, Real & h, Real & dh_dp, Real & dh_dT) const override;

  /// Henry's law constant for dissolution in water
  virtual Real henryConstant(Real temperature) const override;

  /// Henry's law constant for dissolution in water and derivative wrt temperature
  virtual void henryConstant_dT(Real temperature, Real & Kh, Real & dKh_dT) const override;

protected:
  /// molar mass
  const Real _molar_mass;

  /// thermal expansion coefficient
  const Real _thermal_expansion;

  /// specific heat at constant volume
  const Real _cv;

  /// specific heat at constant pressure
  const Real _cp;

  /// bulk modulus
  const Real _bulk_modulus;

  /// thermal conductivity
  const Real _thermal_conductivity;

  /// specific entropy
  const Real _specific_entropy;

  /// Henry constant
  const Real _henry_constant;

  /// Porepressure coefficient: enthalpy = internal_energy + porepressure / density * _pp_coeff
  const Real _pp_coeff;
};

#endif /* TIGERWATERFRACTURECODE_H */
