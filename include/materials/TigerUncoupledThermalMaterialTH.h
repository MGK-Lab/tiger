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

#ifndef TIGERUNCOUPLEDTHERMALMATERIALTH_H
#define TIGERUNCOUPLEDTHERMALMATERIALTH_H

#include "TigerMaterialGeneral.h"
#include "RankTwoTensor.h"
#include "Function.h"

class TigerUncoupledThermalMaterialTH;

template <>
InputParameters validParams<TigerUncoupledThermalMaterialTH>();

class TigerUncoupledThermalMaterialTH : public TigerMaterialGeneral
{
public:
  TigerUncoupledThermalMaterialTH(const InputParameters & parameters);

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

  Real tau(Real & norm_v, Real & alpha, Real & diff, Real & dt, Real & h_ele, const RealVectorValue & v, const RealVectorValue & h) const;
  RealVectorValue EEL(const Elem * ele) const;
  void ActualEEL(const Elem * ele, RealVectorValue & l, const unsigned & ind) const;
  Real Optimal(Real) const;
  Real Temporal(const RealVectorValue &, const RealVectorValue &, Real, Real) const;
  Real DoublyAsymptotic(Real) const;
  Real Critical(Real) const;

  bool _has_PeCr;
  bool _has_supg;
  MooseEnum _effective_length;
  MooseEnum _method;
  Real _supg_scale;

  void ConductivityTensorCalculator(Real const & n, Real const & lambda_f, std::vector<Real> lambda_s, MooseEnum conductivity_type, MooseEnum mean_type, int dim);

protected:
  virtual void computeQpProperties() override;
  virtual void computeProperties() override;

  Function & _vel_func;
  MaterialProperty<Real> * _Pe;
  MaterialProperty<Real> * _Cr;

  /// equivalent conductivity for mixture
  MaterialProperty<RankTwoTensor> & _lambda_sf;
  /// coefficient for time derivative kernel
  MaterialProperty<Real> & _T_Kernel_dt;
  /// scaling factor for lowerdimensional element
  MaterialProperty<Real> & _scaling_lowerD;

  MaterialProperty<Real> & _rho_cp_f;
  MaterialProperty<bool> & _SUPG_ind;
  MaterialProperty<RealVectorValue> & _dv;
  MaterialProperty<RealVectorValue> & _SUPG_p;
};

#endif /* TIGERCOUPLEDTHERMALMATERIALTH_H */
