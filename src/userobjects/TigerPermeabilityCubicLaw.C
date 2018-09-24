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

#include "TigerPermeabilityCubicLaw.h"
#include "MooseError.h"

template <>
InputParameters
validParams<TigerPermeabilityCubicLaw>()
{
  InputParameters params = validParams<TigerPermeability>();

  params.addRequiredParam<Real>("apreture", "Apreture of the fracture (m)");

  params.addClassDescription("calculate permeability tensor for fracture based on the Cubic law");
  return params;
}

TigerPermeabilityCubicLaw::TigerPermeabilityCubicLaw(const InputParameters & parameters)
  : TigerPermeability(parameters)
{
  permeability_type = (TigerPermeability::Permeability_Type() = "isotropic");
  Real _apreture = getParam<Real>("apreture");
  k0.clear();
  k0.assign(1,(std::pow(_apreture,2.0) / 12.0));
}
