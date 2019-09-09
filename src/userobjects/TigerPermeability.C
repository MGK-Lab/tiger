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

#include "TigerPermeability.h"

template <>
InputParameters
validParams<TigerPermeability>()
{
  InputParameters params = validParams<GeneralUserObject>();
  return params;
}

TigerPermeability::TigerPermeability(const InputParameters & parameters)
  : GeneralUserObject(parameters),
    permeability_type(TigerPermeability::Permeability_Type() = "isotropic")
{
}

void
TigerPermeability::execute()
{
}

void
TigerPermeability::initialize()
{
}

void
TigerPermeability::finalize()
{
}

MooseEnum
TigerPermeability::Permeability_Type()
{
  return MooseEnum("isotropic=1 orthotropic=2 anisotropic=3");
}


// RankTwoTensor
// TigerPermeability::Permeability(int dim, std::vector<Real> k1, Real porosity, Real scale_factor) const
// {
//   RealVectorValue kx;
//   RealVectorValue ky;
//   RealVectorValue kz;

//   if (dim == 1)
//   {
//     switch (permeability_type)
//     {
//       case 1:
//         if (k0.size() != 1)
//           mooseError("One input value is needed for isotropic distribution of permeability! You provided ", k0.size(), " values.\n");
//     //    k0.clear();
//         kx = RealVectorValue(k0[0], 0.0, 0.0);
//         ky = RealVectorValue(0.0  , 0.0, 0.0);
//         kz = RealVectorValue(0.0  , 0.0, 0.0);
//         break;
//       case 2:
//       case 3:
//         mooseError("One dimensional elements cannot have non-isotropic permeability values.\n");
//         break;
//     }
//   }
//   else if (dim == 2)
//   {
//     switch (permeability_type)
//     {
//       case 1:
//         if (k0.size() != 1)
//           mooseError("One input value is needed for isotropic distribution of permeability! You provided ", k0.size(), " values.\n");
//         kx = RealVectorValue(k0[0], 0.0  , 0.0);
//         ky = RealVectorValue(0.0  , k0[0], 0.0);
//         kz = RealVectorValue(0.0  , 0.0  , 0.0);
//         break;
//       case 2:
//         if (k0.size() != 2)
//           mooseError("Two input values are needed for orthotropic distribution of permeability in two dimensional elements! You provided ", k0.size(), " values.\n");
//         kx = RealVectorValue(k0[0], 0.0  , 0.0);
//         ky = RealVectorValue(0.0  , k0[1], 0.0);
//         kz = RealVectorValue(0.0  , 0.0  , 0.0);
//         break;
//       case 3:
//         if (k0.size() != 4)
//           mooseError("Four input values are needed for anisotropic distribution of permeability in two dimensional elements! You provided ", k0.size(), " values.\n");
//         kx = RealVectorValue(k0[0], k0[1], 0.0);
//         ky = RealVectorValue(k0[2], k0[3], 0.0);
//         kz = RealVectorValue(0.0  , 0.0  , 0.0);
//         break;
//     }
//   }
//   else if (dim == 3)
//   {
//     switch (permeability_type)
//     {
//       case 1:
//         if (k0.size() != 1)
//           mooseError("One input value is needed for isotropic distribution of permeability! You provided ", k0.size(), " values.\n");
//         kx = RealVectorValue(k0[0], 0.0, 0.0);
//         ky = RealVectorValue(0.0, k0[0], 0.0);
//         kz = RealVectorValue(0.0, 0.0, k0[0]);
//         break;
//
//       case 2:
//         if (k0.size() != 3)
//           mooseError("Three input values are needed for orthotropic distribution of permeability! You provided ", k0.size(), " values.\n");
//         kx = RealVectorValue(k0[0], 0.0, 0.0);
//         ky = RealVectorValue(0.0, k0[1], 0.0);
//         kz = RealVectorValue(0.0, 0.0, k0[2]);
//         break;
//
//       case 3:
//         if (k0.size() != 9)
//           mooseError("Nine input values are needed for anisotropic distribution of permeability! You provided ", k0.size(), " values.\n");
//         kx = RealVectorValue(k0[0], k0[1], k0[2]);
//         ky = RealVectorValue(k0[3], k0[4], k0[5]);
//         kz = RealVectorValue(k0[6], k0[7], k0[8]);
//         break;
//     }
//   }
//   std::cout << "Output after in IF cases of kx " << kx << std::endl;
//
//   return RankTwoTensor(kx, ky, kz);
// }
