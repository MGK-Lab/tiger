/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/

#include "TigerPermeabilityRockConst.h"
#include "MooseError.h"

template <>
InputParameters
validParams<TigerPermeabilityRockConst>()
{
  InputParameters params = validParams<TigerPermeability>();
  params.addClassDescription("calculate permeability tensor of rock matrix based on provided initial value");
  return params;
}

TigerPermeabilityRockConst::TigerPermeabilityRockConst(const InputParameters & parameters)
  : TigerPermeability(parameters)
{
}

std::string
TigerPermeabilityRockConst::PermeabilityName() const
{
  return "rock matrix";
}

RankTwoTensor
TigerPermeabilityRockConst::PermeabilityTensorCalculator(MooseEnum permeability_type, std::vector<Real> k0) const
{
  RealVectorValue kx;
  RealVectorValue ky;
  RealVectorValue kz;

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

  return RankTwoTensor(kx, ky, kz);
}
