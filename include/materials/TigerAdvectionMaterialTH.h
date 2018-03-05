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

#ifndef TIGERADVECTIONMATERIALTH_H
#define TIGERADVECTIONMATERIALTH_H

#include "TigerMaterialGeneral.h"
#include "RankTwoTensor.h"

class TigerAdvectionMaterialTH;

template <>
InputParameters validParams<TigerAdvectionMaterialTH>();

class TigerAdvectionMaterialTH : public TigerMaterialGeneral
{
public:
  TigerAdvectionMaterialTH(const InputParameters & parameters);

private:
  virtual Real tau(RealVectorValue vel, Real diff, Real dt, const Elem * ele) const;
  Real EEL(const Elem * ele) const;
  Real Optimal(Real) const;
  Real Temporal(Real, Real, Real, Real) const;
  Real DoublyAsymptotic(Real) const;
  Real Critical(Real) const;

  bool _has_supg;
  bool _pure_advection;
  bool _is_supg_consistent;

protected:
  virtual void computeQpProperties() override;

  MooseEnum _effective_length;
  MooseEnum _method;

  const MaterialProperty<Real> * _lambda_sf_eq;
  const VariableGradient & _gradient_pore_press;
  const MaterialProperty<RankTwoTensor> & _k_vis;
  const MaterialProperty<RealVectorValue> & _rhof_g;
  MaterialProperty<RealVectorValue> & _dv;
  MaterialProperty<RealVectorValue> * _SUPG_p;
  MaterialProperty<RealVectorValue> * _SUPG_p_consistent;
  MaterialProperty<Real> & _rho_cp_f;
};

#endif /* TIGERADVECTIONMATERIALTH_H */
