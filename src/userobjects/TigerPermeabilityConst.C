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

#include "TigerPermeabilityConst.h"
#include "MooseError.h"

template <>
InputParameters
validParams<TigerPermeabilityConst>()
{
  InputParameters params = validParams<TigerPermeability>();
  params.addClassDescription("calculate permeability tensor based on provided initial permeability");
  return params;
}

TigerPermeabilityConst::TigerPermeabilityConst(const InputParameters & parameters)
  : TigerPermeability(parameters)
{
}

RankTwoTensor
TigerPermeabilityConst::PermeabilityTensorCalculator(MooseEnum permeability_type, std::vector<Real> k0, int dim) const
{
  RealVectorValue kx;
  RealVectorValue ky;
  RealVectorValue kz;

  if (dim == 1)
  {
    switch (permeability_type)
    {
      case 1:
        if (k0.size() != 1)
          mooseError("One input value is needed for isotropic distribution of permeability! You provided ", k0.size(), " values.\n");
        kx = RealVectorValue(k0[0], 0.0, 0.0);
        ky = RealVectorValue(0.0  , 0.0, 0.0);
        kz = RealVectorValue(0.0  , 0.0, 0.0);
        break;
      case 2:
      case 3:
        mooseError("One dimensional elements cannot have non-isotropic permeability values.\n");
        break;
    }
  }
  else if (dim == 2)
  {
    switch (permeability_type)
    {
      case 1:
        if (k0.size() != 1)
          mooseError("One input value is needed for isotropic distribution of permeability! You provided ", k0.size(), " values.\n");
        kx = RealVectorValue(k0[0], 0.0  , 0.0);
        ky = RealVectorValue(0.0  , k0[0], 0.0);
        kz = RealVectorValue(0.0  , 0.0  , 0.0);
        break;
      case 2:
        if (k0.size() != 2)
          mooseError("Two input values are needed for orthotropic distribution of permeability in two dimensional elements! You provided ", k0.size(), " values.\n");
        kx = RealVectorValue(k0[0], 0.0  , 0.0);
        ky = RealVectorValue(0.0  , k0[1], 0.0);
        kz = RealVectorValue(0.0  , 0.0  , 0.0);
        break;
      case 3:
        if (k0.size() != 4)
          mooseError("Four input values are needed for anisotropic distribution of permeability in two dimensional elements! You provided ", k0.size(), " values.\n");
        kx = RealVectorValue(k0[0], k0[1], 0.0);
        ky = RealVectorValue(k0[2], k0[3], 0.0);
        kz = RealVectorValue(0.0  , 0.0  , 0.0);
        break;
    }
  }
  else if (dim == 3)
  {
    switch (permeability_type)
    {
      case 1:
        if (k0.size() != 1)
          mooseError("One input value is needed for isotropic distribution of permeability! You provided ", k0.size(), " values.\n");
        kx = RealVectorValue(k0[0], 0.0, 0.0);
        ky = RealVectorValue(0.0, k0[0], 0.0);
        kz = RealVectorValue(0.0, 0.0, k0[0]);
        break;

      case 2:
        if (k0.size() != 3)
          mooseError("Three input values are needed for orthotropic distribution of permeability! You provided ", k0.size(), " values.\n");
        kx = RealVectorValue(k0[0], 0.0, 0.0);
        ky = RealVectorValue(0.0, k0[1], 0.0);
        kz = RealVectorValue(0.0, 0.0, k0[2]);
        break;

      case 3:
        if (k0.size() != 9)
          mooseError("Nine input values are needed for anisotropic distribution of permeability! You provided ", k0.size(), " values.\n");
        kx = RealVectorValue(k0[0], k0[1], k0[2]);
        ky = RealVectorValue(k0[3], k0[4], k0[5]);
        kz = RealVectorValue(k0[6], k0[7], k0[8]);
        break;
    }
  }

  return RankTwoTensor(kx, ky, kz);
}
