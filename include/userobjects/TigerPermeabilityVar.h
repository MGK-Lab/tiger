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

#pragma once

#include "TigerPermeability.h"
#include "MooseEnum.h"

class TigerPermeabilityVar;

template <>
InputParameters validParams<TigerPermeabilityVar>();

class TigerPermeabilityVar : public TigerPermeability
{
public:
  TigerPermeabilityVar(const InputParameters & parameters);

  RankTwoTensor Permeability(const int & dim, const Real & porosity, const Real & scale_factor) const;

protected:
  // Initial permeability from user input
  std::vector<Real> _kinit;
  // Initial porosity from user input
  const Real _ninit;
  // parameters for Kozeny-Carman Eq
  const Real _n;
  const Real _m;

  MooseEnum _permeability_type;
};
