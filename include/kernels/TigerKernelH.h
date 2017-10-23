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

#ifndef TIGERKERNELH_H
#define TIGERKERNELH_H

#include "Kernel.h"
#include "RankTwoTensor.h"

class TigerKernelH;

template <>
InputParameters validParams<TigerKernelH>();

class TigerKernelH : public Kernel
{
public:
  TigerKernelH(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;

  const MaterialProperty<RankTwoTensor> & _k;
  const MaterialProperty<Real> & _mu;
  const MaterialProperty<Real> & _rho_f;
  const MaterialProperty<Real> & _n;
  const MaterialProperty<Real> & _beta_f;
  const MaterialProperty<Real> & _beta_s;
  const MaterialProperty<RealVectorValue> & _gravity;
};

#endif // TIGERKERNELH_H
