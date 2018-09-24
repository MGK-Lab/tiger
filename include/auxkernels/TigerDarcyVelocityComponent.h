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

#ifndef TIGERDARCYVELOCITYCOMPONENT_H
#define TIGERDARCYVELOCITYCOMPONENT_H

#include "AuxKernel.h"
#include "RankTwoTensor.h"

class TigerDarcyVelocityComponent;

template <>
InputParameters validParams<TigerDarcyVelocityComponent>();

class TigerDarcyVelocityComponent : public AuxKernel
{
public:
  TigerDarcyVelocityComponent(const InputParameters & parameters);

protected:
  virtual Real computeValue() override;

private:
  const VariableGradient & _gradient_pore_pressure;
  const MaterialProperty<RankTwoTensor> & _k_vis;
  const MaterialProperty<RealVectorValue> & _rhof_g;
  int _component;
};

#endif // TIGERDARCYVELOCITYCOMPONENT_H
