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

#ifndef TIGERSU_PG_H
#define TIGERSU_PG_H

#include "GeneralUserObject.h"

class TigerSU_PG;

template <>
InputParameters validParams<TigerSU_PG>();

class TigerSU_PG : public GeneralUserObject
{
public:
  TigerSU_PG(const InputParameters & parameters);
  virtual void initialize() {}
  virtual void execute() {}
  virtual void finalize() {}
  virtual Real k_bar(RealVectorValue vel, Real diff, Real dt, const Elem * ele) const;

protected:
  MooseEnum _effective_length;
  MooseEnum _method;

private:
  Real EEL(RealVectorValue vel, const Elem * ele) const;
  Real Optimal(Real) const;
  Real Temporal(Real, Real, Real, Real) const;
  Real DoublyAsymptotic(Real) const;
  Real Critical(Real) const;
};

#endif // TIGERSU_PG_H
