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

#ifndef TIGERPERMEABILITYCUBICLAW_H
#define TIGERPERMEABILITYCUBICLAW_H

#include "TigerPermeability.h"
#include "MooseEnum.h"

class TigerPermeabilityCubicLaw;

template <>
InputParameters validParams<TigerPermeabilityCubicLaw>();

class TigerPermeabilityCubicLaw : public TigerPermeability
{
public:
  TigerPermeabilityCubicLaw(const InputParameters & parameters);
  /// permeability matrix (m^2); called from Material
  RankTwoTensor Permeability(int dim, Real porosity, Real scale_factor) const;
  // Creates the permeability tensor as function of input and dimension
  RankTwoTensor PermeabilityTensorCalculator(int dim, std::vector<Real> k0) const;
  // Unused here, because k0 is constant
  std::vector<Real> PermeabilityVectorCalculator(Real porosity, Real scale_factor) const;
private:
  Real _aperture;

};

#endif /* TIGERPERMEABILITYCUBICLAW_H */
