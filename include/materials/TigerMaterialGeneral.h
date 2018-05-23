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

#ifndef TIGERMATERIALGENERAL_H
#define TIGERMATERIALGENERAL_H

#include "Material.h"
#include "RankTwoTensor.h"
#include "MooseMesh.h"
#include <cfloat>
#include "SinglePhaseFluidPropertiesPT.h"

class TigerMaterialGeneral;

template <>
InputParameters validParams<TigerMaterialGeneral>();

class TigerMaterialGeneral : public Material
{
public:
  TigerMaterialGeneral(const InputParameters & parameters);

protected:
  /// Pressure (Pa)
  const VariableValue & _P;
  /// Temperature (K)
  const VariableValue & _T;
  /// rotation matrix for local cordinates
  RankTwoTensor _rot_mat = RankTwoTensor();
  /// initial scaling factor
  Real _scaling_factor0;

  /// compute rotation matrix
  void computeRotationMatrix(int dim);
  /// compute scaling factor for lower dimensional elements
  Real LowerDScaling();
  /// Tiger Fluid properties UserObject
 const SinglePhaseFluidPropertiesPT & _fp_UO;
};

#endif /* TIGERMATERIALGENERAL_H */
