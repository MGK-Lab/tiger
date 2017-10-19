/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/

#ifndef TIGERROCKMATERIAL_H
#define TIGERROCKMATERIAL_H

#include "Material.h"
#include "RankTwoTensor.h"
#include "TigerPermeability.h"

class TigerRockMaterial;

template <>
InputParameters validParams<TigerRockMaterial>();

class TigerRockMaterial : public Material
{
public:
  TigerRockMaterial(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// enum for selecting permeability distribution
  MooseEnum _permeability_type;
  /// initial permeability
  std::vector<Real> _k0;
  /// initial porosity
  Real _n0;
  /// initial compressibility
  Real _beta0;


  /// Porosity ()
  MaterialProperty<Real> & _n;
  /// permeability tensor (m^2)
  MaterialProperty<RankTwoTensor> & _k;
  /// compressibility (1/Pa)
  MaterialProperty<Real> & _beta;

  /// Tiger permeability calculater UserObject
  const TigerPermeability & _kf_UO;
};

#endif /* TIGERROCKMATERIALH_H */
