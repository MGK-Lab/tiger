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

#ifndef TIGERCOUPLEDTHERMALMATERIALTH_H
#define TIGERCOUPLEDTHERMALMATERIALTH_H

#include "TigerMaterialGeneral.h"
#include "RankTwoTensor.h"

class TigerCoupledThermalMaterialTH;

template <>
InputParameters validParams<TigerCoupledThermalMaterialTH>();

class TigerCoupledThermalMaterialTH : public TigerMaterialGeneral
{
public:
  TigerCoupledThermalMaterialTH(const InputParameters & parameters);

private:
  // enum for selecting thermal conductivity distribution for solid phase
  MooseEnum _ct;
  // enum for selecting calculation method for mixture
  MooseEnum _mean;
  // initial thermal conductivity for solid phase
  std::vector<Real> _lambda0;
  // initial specific heat for solid phase
  Real _cp0;
  // initial density for solid phase
  Real _rho0;

  RankTwoTensor _lambda_sf_tensor = RankTwoTensor();
  Real _lambda_sf_real = 0.0;

  bool _has_PeCr;
  bool _has_supg;
  Real _supg_scale;

  void ConductivityTensorCalculator(Real const & n, Real const & lambda_f, std::vector<Real> lambda_s, MooseEnum conductivity_type, MooseEnum mean_type, int dim);

protected:
  virtual void computeQpProperties() override;

  const VariableGradient & _gradient_pore_press;
  const MaterialProperty<RankTwoTensor> & _k_vis;
  const MaterialProperty<Real> & _rho_f;
  const MaterialProperty<RealVectorValue> & _g;
  MaterialProperty<Real> * _Pe;
  MaterialProperty<Real> * _Cr;

  // equivalent conductivity for mixture
  MaterialProperty<RankTwoTensor> & _lambda_sf;
  // coefficient for time derivative kernel
  MaterialProperty<Real> & _T_Kernel_dt;

  MaterialProperty<Real> & _rho_cp_f;
  MaterialProperty<bool> & _SUPG_ind;
  MaterialProperty<RealVectorValue> & _dv;
  MaterialProperty<RealVectorValue> & _SUPG_p;

  // imported props from TigerGeometryMaterial
  const MaterialProperty<Real> & _n;
  const MaterialProperty<RankTwoTensor> & _rot_mat;
};

#endif /* TIGERCOUPLEDTHERMALMATERIALTH_H */
