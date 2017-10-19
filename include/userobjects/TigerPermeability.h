/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/

#ifndef TIGERPERMEABILITY_H
#define TIGERPERMEABILITY_H

#include "GeneralUserObject.h"
#include "RankTwoTensor.h"

// Forward Declarations
class TigerPermeability;

template <>
InputParameters validParams<TigerPermeability>();

class TigerPermeability : public GeneralUserObject
{
public:
  TigerPermeability(const InputParameters & parameters);

  virtual void execute();
  virtual void initialize();
  virtual void finalize();

  /// permeability name
  virtual std::string PermeabilityName() const = 0;

  /// permeability matrix (m^2)
  virtual RankTwoTensor PermeabilityTensorCalculator(MooseEnum permeability_type, std::vector<Real> k0) const = 0;
};

#endif /* TIGERPERMEABILITY_H */
