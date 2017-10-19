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
