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

#include "TigerPorosityMaterial.h"
#include "MooseMesh.h"
#include <cfloat>
#include "Function.h"

registerMooseObject("TigerApp", TigerPorosityMaterial);

template <>
InputParameters
validParams<TigerPorosityMaterial>()
{
  InputParameters params = validParams<Material>();

  params.addRequiredCoupledVar("porosity", "porosity (temporal and spatial function)");
  params.addParam<bool>("porosity_evolotion", false,"if it evoloves by "
        "deformation, true. Attention, if true, the given porosity should not be"
        " temporal and displacement variable should be given");
  params.addClassDescription("Material for defining porosity and its evolotion");

  return params;
}

TigerPorosityMaterial::TigerPorosityMaterial(const InputParameters & parameters)
  : Material(parameters),
    _n(declareProperty<Real>("porosity")),
    _n0(coupledValue("porosity")),
    _p_e(getParam<bool>("porosity_evolotion"))
{
}

void
TigerPorosityMaterial::computeQpProperties()
{
  if (!_p_e)
    _n[_qp] = _n0[_qp];
}
