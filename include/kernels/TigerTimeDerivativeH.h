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

#ifndef TIGERTIMEDERIVATIVEH_H
#define TIGERTIMEDERIVATIVEH_H

#include "TimeDerivative.h"

class TigerTimeDerivativeH;

template <>
InputParameters validParams<TigerTimeDerivativeH>();

class TigerTimeDerivativeH : public TimeDerivative
{
public:
  TigerTimeDerivativeH(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;

  const MaterialProperty<Real> & _H_Kernel_dt;
};

#endif // TIGERTIMEDERIVATIVEH_H
