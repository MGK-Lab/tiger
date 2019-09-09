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

#ifndef TIGERPERMEABILITY_H
#define TIGERPERMEABILITY_H

#include "GeneralUserObject.h"
#include "RankTwoTensor.h"

// Forward Declarations
class TigerPermeability;

template <>
InputParameters validParams<TigerPermeability>();

class TigerPermeability : public GeneralUserObject
{
public:
  TigerPermeability(const InputParameters & parameters);

  virtual void execute();
  virtual void initialize();
  virtual void finalize();

  /// permeability matrix (m^2); called from Material
  virtual RankTwoTensor Permeability(int dim, Real porosity, Real scale_factor) const = 0;

  virtual RankTwoTensor PermeabilityTensorCalculator(int dim, std::vector<Real> k0) const = 0;
  virtual std::vector<Real> PermeabilityVectorCalculator(Real porosity, Real scale_factor) const = 0;
  static MooseEnum Permeability_Type();

protected:
  /// permeability vector (m^2)
  MooseEnum _permeability_type;
};

#endif /* TIGERPERMEABILITY_H */
