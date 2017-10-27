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

#ifndef TIGERROCKMATERIALT_H
#define TIGERROCKMATERIALT_H

#include "Material.h"

class TigerRockMaterialT;

template <>
InputParameters validParams<TigerRockMaterialT>();

class TigerRockMaterialT : public Material
{
public:
  TigerRockMaterialT(const InputParameters & parameters);

private:
  /// enum for selecting thermal conductivity distribution
  MooseEnum _conductivity_type;
  /// initial thermal conductivity
  std::vector<Real> _lambda0;
  /// initial specific heat
  Real _cp0;
  /// initial density
  Real _rho0;

protected:
  virtual void computeQpProperties() override;

  /// thermal conductivity vector (W/(m K))
  MaterialProperty<std::vector<Real>> & _lambda;
  /// specific heat (J/(kg K))
  MaterialProperty<Real> & _cp;
  /// density (kg/m^3)
  MaterialProperty<Real> & _rho;
};

#endif /* TIGERROCKMATERIALT_H */
