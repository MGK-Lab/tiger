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

  params.set<ExecFlagEnum>("execute_on", true) = EXEC_INITIAL;
  return params;
}

TigerPermeability::TigerPermeability(const InputParameters & parameters)
  : GeneralUserObject(parameters),
    _permeability_type(TigerPermeability::Permeability_Type() = "isotropic")
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
