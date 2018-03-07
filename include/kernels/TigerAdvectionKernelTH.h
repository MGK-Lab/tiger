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

#ifndef TIGERADVECTIONKERNELTH_H
#define TIGERADVECTIONKERNELTH_H

#include "Kernel.h"

class TigerAdvectionKernelTH;

template <>
InputParameters validParams<TigerAdvectionKernelTH>();

class TigerAdvectionKernelTH : public Kernel
{
public:
  TigerAdvectionKernelTH(const InputParameters & parameters);

private:
  bool _has_supg;

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;

  const MaterialProperty<Real> & _scaling_lowerD;
  const MaterialProperty<Real> & _rho_cp_f;
  const MaterialProperty<RealVectorValue> * _SUPG_p;
  const MaterialProperty<RealVectorValue> & _darcy_v;
};

#endif // TIGERADVECTIONKERNELTH_H
