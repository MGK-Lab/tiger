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
