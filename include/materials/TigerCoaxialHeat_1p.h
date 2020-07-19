/**************************************************************************/
/*  MOSKITO - Multiphysics cOupled Simulator toolKIT for wellbOres        */
/*                                                                        */
/*  Copyright (C) 2019 by Maziar Gholami Korzani                          */
/*  Karlsruhe Institute of Technology, Institute of Applied Geosciences   */
/*  Division of Geothermal Research                                       */
/*                                                                        */
/*  This file is part of MOSKITO App                                      */
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

class TigerCoaxialHeat_1p;

template <>
InputParameters validParams<TigerCoaxialHeat_1p>();

class TigerCoaxialHeat_1p : public Material
{
public:
  TigerCoaxialHeat_1p(const InputParameters & parameters);
  virtual void computeQpProperties() override;

protected:

  // outer radius of drilling pipe
  MaterialProperty<Real> & _rdo;
  // inner radius of drilling pipe
  MaterialProperty<Real> & _rdi;
  // conductivity of drilling pipe
  MaterialProperty<Real> & _kd;

  Real _rdoo;
  Real _rdii;
  Real _kdd;

};
