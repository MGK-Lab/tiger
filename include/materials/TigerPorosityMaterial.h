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

#pragma once

#include "Material.h"

class TigerPorosityMaterial;

template <>
InputParameters validParams<TigerPorosityMaterial>();

class TigerPorosityMaterial : public Material
{
public:
  TigerPorosityMaterial(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  // bulk density
  MaterialProperty<Real> & _rho_b;
  // mixture density
  MaterialProperty<Real> & _rho_m;
  // void mass fraction
  MaterialProperty<Real> & _mass_frac;
  // calculated porosity
  MaterialProperty<Real> & _n;
  // Initial porosity
  const VariableValue & _n0;

  // taken from TigerFluidMaterial
  const MaterialProperty<Real> & _rho_f;

  // taken from TigerMechanicsMaterialM
  const MaterialProperty<Real> * _biot;
  const MaterialProperty<Real> * _vol_total_strain;

private:
  const bool _p_e;
  const Real _rho_r;
  bool _ev_type;
};
