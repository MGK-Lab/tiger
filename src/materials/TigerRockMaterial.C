/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/

#include "TigerRockMaterial.h"

template <>
InputParameters
validParams<TigerRockMaterial>()
{
  InputParameters params = validParams<Material>();
  params.addRequiredParam<Real>("porosity", "Porosity of rock matrix");
  params.addRequiredParam<Real>("compressibility", "Compressibility of rock matrix (1/Pa)");

  MooseEnum PT("isotropic=1 orthotropic=2 anisotropic=3");
  params.addRequiredParam<MooseEnum>("permeability_type", PT, "The permeability distribution type [isotropic, orthotropic, anisotropic].");

  params.addRequiredParam<std::vector<Real>>("k0", "Initial permeability of rock matrix (m^2)");
  params.addRequiredParam<UserObjectName>("kf_UO", "The name of the user object for permeability calculation");
  params.addClassDescription("Rock matrix properties");
  return params;
}

TigerRockMaterial::TigerRockMaterial(const InputParameters & parameters)
  : Material(parameters),
    _permeability_type(getParam<MooseEnum>("permeability_type")),
    _k0(getParam<std::vector<Real>>("k0")),
    _n0(getParam<Real>("porosity")),
    _beta0(getParam<Real>("compressibility")),
    _n(declareProperty<Real>("porosity")),
    _k(declareProperty<RankTwoTensor>("permeability")),
    _beta(declareProperty<Real>("solid_compressibility")),
    _kf_UO(getUserObject<TigerPermeability>("kf_UO"))
{
}

void
TigerRockMaterial::computeQpProperties()
{
  _n   [_qp] = _n0;
  _k   [_qp] = _kf_UO.PermeabilityTensorCalculator(_permeability_type,_k0);
  _beta[_qp] = _beta0;
}
