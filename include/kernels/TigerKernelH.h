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
  const MaterialProperty<Real> & _fluid_viscosity;
  const MaterialProperty<Real> & _solid_porosity;
  const MaterialProperty<Real> & _fluid_compressibility;
  const MaterialProperty<Real> & _solid_compressibility;
};

#endif // TIGERKERNELH_H
