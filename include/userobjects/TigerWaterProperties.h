/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/

#ifndef TIGERWATERPROPERTIES_H
#define TIGERWATERPROPERTIES_H

#include "TigerFluidPropertiesTP.h"

class TigerWaterProperties;

template <>
InputParameters validParams<TigerWaterProperties>();

class TigerWaterProperties : public TigerFluidPropertiesTP
{
public:
  TigerWaterProperties(const InputParameters & parameters);

  /// Fluid name
  virtual std::string fluidName() const override;

  /// Density from pressure and temperature (kg/m^3)
  virtual Real rho(Real pressure, Real temperature) const override;

  /// Dynamic viscosity (Pa.s)
  virtual Real mu(Real temperature) const override;

  /// Compressibility (1/Pa)
  virtual Real beta(Real pressure, Real temperature) const override;
};

#endif /* TIGERWATERPROPERTIES_H */
