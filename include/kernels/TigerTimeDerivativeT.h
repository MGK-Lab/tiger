/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef TIGERTIMEDERIVATIVET_H
#define TIGERTIMEDERIVATIVET_H

#include "TimeDerivative.h"

class TigerTimeDerivativeT;

template <>
InputParameters validParams<TigerTimeDerivativeT>();

class TigerTimeDerivativeT : public TimeDerivative
{
public:
  TigerTimeDerivativeT(const InputParameters & parameters);

private:
  bool _pure_advection;

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;

  const MaterialProperty<Real> & _T_Kernel_dt;
};

#endif // TIGERTIMEDERIVATIVET_H
