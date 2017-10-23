/**************************************************************************/
/*  TIGER - Hydro-thermal sImulator GEothermal Reservoirs                 */
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

#include "TigerRockMaterial.h"
#include "MooseMesh.h"

template <>
InputParameters
validParams<TigerRockMaterial>()
{
  InputParameters params = validParams<Material>();
  params.addParam<bool>("has_gravity", false, "Is the gravity enabled?");
  params.addParam<Real>("gravity_acceleration", 9.81, "The magnitude of the gravity acceleration (m/s^2)");
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
    _has_gravity(getParam<bool>("has_gravity")),
    _g(getParam<Real>("gravity_acceleration")),
    _gravity(declareProperty<RealVectorValue>("gravity_vector")),
    _n(declareProperty<Real>("porosity")),
    _k(declareProperty<RankTwoTensor>("permeability_tensor")),
    _beta(declareProperty<Real>("solid_compressibility")),
    _kf_UO(getUserObject<TigerPermeability>("kf_UO"))
{
  if (_has_gravity)
    switch (_mesh.dimension())
    {
      case 1:
        _gravity0 = RealVectorValue(-_g, 0.0, 0.0);
        break;
      case 2:
        _gravity0 = RealVectorValue(0.0, -_g, 0.0);
        break;
      case 3:
        _gravity0 = RealVectorValue(0.0, 0.0, -_g);
        break;
      default:
        _gravity0 = RealVectorValue(0.0, 0.0, 0.0);
        break;
    }
  else
    _gravity0 = RealVectorValue(0.0, 0.0, 0.0);
}

void
TigerRockMaterial::computeQpProperties()
{
  _n   [_qp] = _n0;
  _k   [_qp] = _kf_UO.PermeabilityTensorCalculator(_permeability_type,_k0);
  _beta[_qp] = _beta0;
  _gravity[_qp] = _gravity0;
}
