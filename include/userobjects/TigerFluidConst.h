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

#ifndef TIGERFLUIDCONST_H
#define TIGERFLUIDCONST_H

#include "TigerFluidPropertiesTP.h"

class TigerFluidConst;

template <>
InputParameters validParams<TigerFluidConst>();

class TigerFluidConst : public TigerFluidPropertiesTP
{
public:
  TigerFluidConst(const InputParameters & parameters);

  /// Fluid name
  virtual std::string fluidName() const override;

  /// Density from pressure and temperature (kg/m^3)
  virtual Real rho(Real pressure, Real temperature) const override;

  /// Dynamic viscosity (Pa.s)
  virtual Real mu(Real pressure, Real temperature) const override;

  /// Compressibility (1/Pa)
  virtual Real beta(Real pressure, Real temperature) const override;

  /// specific heat (J/(kg K))
  virtual Real cp(Real pressure, Real temperature) const override;

  /// thermal conductivity (W/(m K))
  virtual Real lambda(Real pressure, Real temperature) const override;

private:
  Real _rho;
  Real _mu;
  Real _beta;
  Real _cp;
  Real _lambda;
};

#endif /* TIGERFLUIDCONST_H */
