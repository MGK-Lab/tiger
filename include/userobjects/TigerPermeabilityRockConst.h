/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/

#ifndef TIGERPERMEABILITYROCKCONST_H
#define TIGERPERMEABILITYROCKCONST_H

#include "TigerPermeability.h"
#include "MooseEnum.h"

class TigerPermeabilityRockConst;

template <>
InputParameters validParams<TigerPermeabilityRockConst>();

class TigerPermeabilityRockConst : public TigerPermeability
{
public:
  TigerPermeabilityRockConst(const InputParameters & parameters);

  /// permeability name
  virtual std::string PermeabilityName() const override;

  /// permeability matrix (m^2)
  virtual RankTwoTensor PermeabilityTensorCalculator(MooseEnum permeability_type, std::vector<Real> k0) const override;
};

#endif /* TIGERPERMEABILITYROCKCONST_H */
