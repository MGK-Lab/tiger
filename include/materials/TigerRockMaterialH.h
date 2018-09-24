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

#ifndef TIGERROCKMATERIALH_H
#define TIGERROCKMATERIALH_H

#include "TigerMaterialGeneral.h"
#include "RankTwoTensor.h"
#include "TigerPermeability.h"

class TigerRockMaterialH;

template <>
InputParameters validParams<TigerRockMaterialH>();

class TigerRockMaterialH : public TigerMaterialGeneral
{
public:
  TigerRockMaterialH(const InputParameters & parameters);

protected:
  virtual void computeProperties() override;
  virtual void computeQpProperties() override;

  /// initial compressibility
  Real _beta_s;
  /// gravity vector
  RealVectorValue _gravity;
  /// initial porosity
  Real _n0;
  /// permeability tensor
  MaterialProperty<RankTwoTensor> & _k_vis;
  /// compressibility
  MaterialProperty<Real> & _H_Kernel_dt;
  /// density
  MaterialProperty<Real> & _rhof;
  /// compressibility
  MaterialProperty<RealVectorValue> & _rhof_g;
  /// Tiger permeability calculater UserObject
  const TigerPermeability & _kf_UO;
  /// gravity option
  bool _has_gravity;
  /// gravity acceleration (m/s^2)
  Real _g;
  /// scaling factor for lowerdimensional element
  MaterialProperty<Real> & _scaling_lowerD;
};

#endif /* TIGERROCKMATERIALH_H */
