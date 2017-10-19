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

#ifndef TIGERPERMEABILITYROCKCONST_H
#define TIGERPERMEABILITYROCKCONST_H

#include "TigerPermeability.h"
#include "MooseEnum.h"

class TigerPermeabilityRockConst;

template <>
InputParameters validParams<TigerPermeabilityRockConst>();

class TigerPermeabilityRockConst : public TigerPermeability
{
public:
  TigerPermeabilityRockConst(const InputParameters & parameters);

  /// permeability name
  virtual std::string PermeabilityName() const override;

  /// permeability matrix (m^2)
  virtual RankTwoTensor PermeabilityTensorCalculator(MooseEnum permeability_type, std::vector<Real> k0) const override;
};

#endif /* TIGERPERMEABILITYROCKCONST_H */
