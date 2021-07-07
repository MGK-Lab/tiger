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

  params.addRequiredParam<Real>("specific_density",
        "specific density of rock for calculating bulk density (kg/m^3)");
  params.addRequiredCoupledVar("porosity", "porosity (temporal and spatial function)");
  params.addParam<bool>("porosity_evolotion", false,"if it evoloves by "
        "deformation, true");
  MooseEnum ET("exponential fractional", "exponential");
  params.addParam<MooseEnum>("evolotion_type", ET,
        "Two approaches to calculate prososity evolution based on total strain"
        "[exponential, fractional]");
  params.addClassDescription("Material for defining porosity and its evolotion");

  return params;
}

TigerPorosityMaterial::TigerPorosityMaterial(const InputParameters & parameters)
  : Material(parameters),
    _rho_b(declareProperty<Real>("bulk_density")),
    _rho_m(declareProperty<Real>("mixture_density")),
    _mass_frac(declareProperty<Real>("void_mass_fraction")),
    _n(declareProperty<Real>("porosity")),
    _n0(coupledValue("porosity")),
    _rho_f(getMaterialProperty<Real>("fluid_density")),
    _p_e(getParam<bool>("porosity_evolotion")),
    _rho_r(getParam<Real>("specific_density"))
{
  MooseEnum ET("exponential", "exponential");
  if(getParam<MooseEnum>("evolotion_type")==ET)
    _ev_type = true;
  else
    _ev_type = false;

  if (_p_e)
  {
    _biot = &getMaterialProperty<Real>("biot_coefficient");
    _vol_total_strain = &getMaterialProperty<Real>("total_volumetric_strain_HM");
  }
  else
  {
    _biot = NULL;
    _vol_total_strain = NULL;
  }
}

void
TigerPorosityMaterial::computeQpProperties()
{
  if (!_p_e)
    _n[_qp] = _n0[_qp];
  else
  {
    Real c = log((*_biot)[_qp] / ((*_biot)[_qp] - _n0[_qp]));
    if (_ev_type)
      _n[_qp] = (*_biot)[_qp] + (_n0[_qp] - (*_biot)[_qp]) * exp(c * (1.0 - exp((*_vol_total_strain)[_qp] / c)));
    else
    {
      _n[_qp] = (_n0[_qp] + (*_vol_total_strain)[_qp]) / (1.0 + (*_vol_total_strain)[_qp]);
      if (_n[_qp]<0.0) mooseError("negative porosity due to very low volumetric strain");
    }
  }

  _rho_b[_qp] = (1.0 - _n[_qp]) * _rho_r;
  _rho_m[_qp] = _n[_qp] * _rho_f[_qp] + _rho_b[_qp];

  if (_n[_qp] ==0.0 || _n[_qp] == 1.0)
    _mass_frac[_qp] =  _n[_qp];
  else
  {
    if ((_rho_r - _rho_f[_qp]) == 0.0 || _rho_m[_qp] == 0.0)
      mooseError("Rock density and fluid density are either equal or zero");
    else
      _mass_frac[_qp] = (_rho_r - _rho_m[_qp]) * _rho_f[_qp] / _rho_m[_qp] / (_rho_r - _rho_f[_qp]);
  }
}
