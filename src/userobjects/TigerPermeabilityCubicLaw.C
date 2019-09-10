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

registerMooseObject("TigerApp", TigerPermeabilityCubicLaw);

template <>
InputParameters
validParams<TigerPermeabilityCubicLaw>()
{
  InputParameters params = validParams<TigerPermeability>();
  params.addParam<Real>("aperture", 0, "Aperture of the fracture (m), just necessary if different from the scaling factor");
  params.set<ExecFlagEnum>("execute_on", true) = EXEC_TIMESTEP_BEGIN;


  params.addClassDescription("Permeability tensor for fractures"
        " based on the Cubic law");
  return params;
}

TigerPermeabilityCubicLaw::TigerPermeabilityCubicLaw(const InputParameters & parameters)
  : TigerPermeability(parameters),
  _aperture(getParam<Real>("aperture"))
{
  _permeability_type = (TigerPermeability::Permeability_Type() = "isotropic");
}

RankTwoTensor
TigerPermeabilityCubicLaw::Permeability(int dim, Real porosity, Real scale_factor) const
{
  Real effAperture = 0;
  if(_aperture == 0)
    effAperture = scale_factor;
  else
    effAperture = _aperture;
  std::vector<Real> k0 = PermeabilityVectorCalculator(porosity, effAperture);
  return PermeabilityTensorCalculator(dim, k0);

}

RankTwoTensor
TigerPermeabilityCubicLaw::PermeabilityTensorCalculator(int dim, std::vector<Real> k0) const
{

      RealVectorValue kx;
      RealVectorValue ky;
      RealVectorValue kz;

      if (dim == 1)
      {
            if (k0.size() != 1)
              mooseError(name(),": One input value is needed for isotropic distribution of permeability! You provided ", k0.size(), " values.\n");
            kx = RealVectorValue(k0[0], 0.0, 0.0);
            ky = RealVectorValue(0.0  , 0.0, 0.0);
            kz = RealVectorValue(0.0  , 0.0, 0.0);

      }
      else if (dim == 2)
      {
            if (k0.size() != 1)
              mooseError(name(),": One input value is needed for isotropic distribution of permeability! You provided ", k0.size(), " values.\n");
            kx = RealVectorValue(k0[0], 0.0  , 0.0);
            ky = RealVectorValue(0.0  , k0[0], 0.0);
            kz = RealVectorValue(0.0  , 0.0  , 0.0);

      }
      else if (dim == 3)
      {
            if (k0.size() != 1)
              mooseError(name(),": One input value is needed for isotropic distribution of permeability! You provided ", k0.size(), " values.\n");
            kx = RealVectorValue(k0[0], 0.0, 0.0);
            ky = RealVectorValue(0.0, k0[0], 0.0);
            kz = RealVectorValue(0.0, 0.0, k0[0]);

      }
      return RankTwoTensor(kx, ky, kz);

}

std::vector<Real>
TigerPermeabilityCubicLaw::PermeabilityVectorCalculator(Real porosity, Real effAperture) const
{
  std::vector<Real> k0(1,(std::pow(effAperture,2.0) / 12.0));
  return k0;
}
