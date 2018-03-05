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

#ifndef TIGERROCKMATERIALT_H
#define TIGERROCKMATERIALT_H

#include "TigerMaterialGeneral.h"
#include "RankTwoTensor.h"

class TigerRockMaterialT;

template <>
InputParameters validParams<TigerRockMaterialT>();

class TigerRockMaterialT : public TigerMaterialGeneral
{
public:
  TigerRockMaterialT(const InputParameters & parameters);

private:
  /// enum for selecting thermal conductivity distribution for solid phase
  MooseEnum _ct;
  /// enum for selecting calculation method for mixture
  MooseEnum _mean;
  /// initial thermal conductivity for solid phase
  std::vector<Real> _lambda0;
  /// initial specific heat for solid phase
  Real _cp0;
  /// initial density for solid phase
  Real _rho0;
  /// initial porosity
  Real _n0;

  RankTwoTensor _lambda_sf_tensor = RankTwoTensor();
  Real _lambda_sf_real = 0.0;

  void ConductivityTensorCalculator(Real const & n, Real const & lambda_f, std::vector<Real> lambda_s, MooseEnum conductivity_type, MooseEnum mean_type, int dim);

protected:
  virtual void computeQpProperties() override;
  virtual void computeProperties() override;

  /// conductivity tensor for mixture
  MaterialProperty<Real> & _lambda_sf_eq;
  /// equivalent conductivity for mixture
  MaterialProperty<RankTwoTensor> & _lambda_sf;
  /// coefficient for time derivative kernel
  MaterialProperty<Real> & _T_Kernel_dt;
};

#endif /* TIGERROCKMATERIALT_H */
