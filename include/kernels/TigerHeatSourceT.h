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

#ifndef TIGERHEATSOURCET_H
#define TIGERHEATSOURCET_H

#include "Kernel.h"

class TigerHeatSourceT;
class Function;

template <>
InputParameters validParams<TigerHeatSourceT>();

class TigerHeatSourceT : public Kernel
{
public:
  TigerHeatSourceT(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;

  const Real & _scale;

  Function & _function;
};

#endif  //TIGERHEATSOURCET_H
