/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/

#ifndef TIGERFLUIDPROPERTIESTP_H
#define TIGERFLUIDPROPERTIESTP_H

#include "GeneralUserObject.h"

// Forward Declarations
class TigerFluidPropertiesTP;

template <>
InputParameters validParams<TigerFluidPropertiesTP>();

class TigerFluidPropertiesTP : public GeneralUserObject
{
public:
  TigerFluidPropertiesTP(const InputParameters & parameters);

  virtual void execute();
  virtual void initialize();
  virtual void finalize();

  /// Fluid name
  virtual std::string fluidName() const = 0;

  /// Density from pressure and temperature (kg/m^3)
  virtual Real rho(Real pressure, Real temperature) const = 0;

  /// Dynamic viscosity (Pa.s)
  virtual Real mu(Real temperature) const = 0;

  /// Compressibility (1/Pa)
  virtual Real beta(Real pressure, Real temperature) const = 0;
};

#endif /* TIGERFLUIDPROPERTIESTP_H */
