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

#ifndef TIGERROCKMATERIALH_H
#define TIGERROCKMATERIALH_H

#include "Material.h"
#include "RankTwoTensor.h"
#include "TigerPermeability.h"

class TigerRockMaterialH;

template <>
InputParameters validParams<TigerRockMaterialH>();

class TigerRockMaterialH : public Material
{
public:
  TigerRockMaterialH(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// enum for selecting permeability distribution
  MooseEnum _permeability_type;
  /// initial permeability
  std::vector<Real> _k0;
  /// initial compressibility
  Real _beta0;

  /// permeability tensor (m^2)
  MaterialProperty<RankTwoTensor> & _k;
  /// compressibility (1/Pa)
  MaterialProperty<Real> & _beta;

  /// Tiger permeability calculater UserObject
  const TigerPermeability & _kf_UO;
};

#endif /* TIGERROCKMATERIALH_H */
