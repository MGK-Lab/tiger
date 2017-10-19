/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/

#ifndef TIGERFLUIDMATERIALTP_H
#define TIGERFLUIDMATERIALTP_H

#include "Material.h"
#include "TigerFluidPropertiesTP.h"

class TigerFluidMaterialTP;

template <>
InputParameters validParams<TigerFluidMaterialTP>();

/**
 * Computes fluid properties using (pressure, temperature) formulation
 */
class TigerFluidMaterialTP : public Material
{
public:
  TigerFluidMaterialTP(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// Pressure (Pa)
  const VariableValue & _pressure;
  /// Temperature (K)
  const VariableValue & _temperature;

  /// Density (kg/m^3)
  MaterialProperty<Real> & _rho;
  /// Viscosity (Pa.s)
  MaterialProperty<Real> & _mu;
  /// compressibility (1/Pa)
  MaterialProperty<Real> & _beta;

  /// Tiger Fluid properties UserObject
 const TigerFluidPropertiesTP & _fluid_properties_UO;
};

#endif /* TIGERFLUIDMATERIALTP_H */
