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

#ifndef TIGERADVECTIONKERNELT_H
#define TIGERADVECTIONKERNELT_H

#include "Kernel.h"
#include "RankTwoTensor.h"

class TigerAdvectionKernelT;

template <>
InputParameters validParams<TigerAdvectionKernelT>();

class TigerAdvectionKernelT : public Kernel
{
public:
  TigerAdvectionKernelT(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;

  const MaterialProperty<Real> & _rho_cp_f;
  const VariableGradient & _gradient_pore_press;
  const MaterialProperty<Real> & _T_Kernel_dt;
  const MaterialProperty<RankTwoTensor> & _k_vis;
  const MaterialProperty<RealVectorValue> & _rhof_g;

private:
  Real _dt_coeff;
};

#endif // TIGERADVECTIONKERNELT_H
