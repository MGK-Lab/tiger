/******************************************************************************/
/*           GOLEM - Multiphysics of faulted geothermal reservoirs            */
/*                                                                            */
/*          Copyright (C) 2017 by Antoine B. Jacquey and Mauro Cacace         */
/*             GFZ Potsdam, German Research Centre for Geosciences            */
/*                                                                            */
/*    This program is free software: you can redistribute it and/or modify    */
/*    it under the terms of the GNU General Public License as published by    */
/*      the Free Software Foundation, either version 3 of the License, or     */
/*                     (at your option) any later version.                    */
/*                                                                            */
/*       This program is distributed in the hope that it will be useful,      */
/*       but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/*        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the       */
/*                GNU General Public License for more details.                */
/*                                                                            */
/*      You should have received a copy of the GNU General Public License     */
/*    along with this program.  If not, see <http://www.gnu.org/licenses/>    */
/******************************************************************************/

#include "TigerSU_PG.h"

template <>
InputParameters
validParams<TigerSU_PG>()
{
  InputParameters params = validParams<GeneralUserObject>();
  MooseEnum EleLen("min=1 max=2 average=3");
  params.addRequiredParam<MooseEnum>("effective_length", EleLen, "The characteristic element length for SU/PG.");
  MooseEnum Method("optimal=1 temporal=2 doubly_asymptotic=3 critical=4");
  params.addRequiredParam<MooseEnum>("method", Method , "The SUPG method.");
  params.addClassDescription("User Object class to implement Streamline Upwinding using Petrov-Galerkin.");
  return params;
}

TigerSU_PG::TigerSU_PG(const InputParameters & parameters)
  : GeneralUserObject(parameters),
    _effective_length(getParam<MooseEnum>("effective_length")),
    _method(getParam<MooseEnum>("method"))
{
}

Real
TigerSU_PG::k_bar(RealVectorValue vel, Real diff, Real dt, const Elem * ele) const
{
  Real norm_v = vel.norm();
  Real h_ele = EEL(vel, ele);
  Real beta = 0.0;
  Real alpha = 0.5 * norm_v * h_ele / diff;

  switch (_method)
  {
    case 1: // optimal
      beta += Optimal(alpha);
      break;
    case 2: // temporal (not found the equation yet)
      beta += Temporal(norm_v, h_ele, diff, dt);
      break;
    case 3:
      beta += DoublyAsymptotic(alpha);
      break;
    case 4:
      beta += Critical(alpha);
      break;
  }
  return beta * norm_v * h_ele / 2.0;
}

Real
TigerSU_PG::Optimal(Real alpha) const
{
  Real s = 0.0;
  if (alpha < 0.01)
    s = alpha * (1.0 / 3.0 + alpha * alpha * (-1.0 / 45.0 + 18.0 / 8505.0 * alpha * alpha)); //taylor expansion
  else if (0.01 <= alpha && alpha < 50)
    s = 1.0 / std::tanh(alpha) - 1.0 / alpha;
  else if (50 <= alpha)
    s = 1.0;
  return s;
}

Real
TigerSU_PG::Temporal(Real norm_v, Real h_ele, Real diff, Real dt) const
{
  return (1.0 / sqrt((2.0 / dt) * (2.0 / dt) + (2.0 * norm_v / h_ele) * (2.0 * norm_v / h_ele) +
                     (4.0 * diff / (h_ele * h_ele)) * (4.0 * diff / (h_ele * h_ele))));
}

Real
TigerSU_PG::DoublyAsymptotic(Real alpha) const
{
  Real s = 0.0;
  if (alpha <= 3.0)
    s = alpha / 3.0;
  else
    s = 1.0;
  return s;
}

Real
TigerSU_PG::Critical(Real alpha) const
{
  Real s = 0.0;
  if (alpha <= 1.0)
    s = 0.0;
  else
    s = 1.0 - 1.0 / alpha;
  return s;
}

Real
TigerSU_PG::EEL(RealVectorValue vel, const Elem * ele) const
{
  Real L = 0.0;
  RealVectorValue my_vel = vel;
  if (ele->dim() == 1)
    L += ele->volume();
  else
  {
    switch (_effective_length)
    {
      case 1: // min
        L += ele->hmin();
        break;
      case 2: // max
        L += ele->hmax();
        break;
      case 3: // average
        L += 0.5 * (ele->hmin() + ele->hmax());
        break;
    }
  }
  return L;
}
