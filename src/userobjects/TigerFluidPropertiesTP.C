/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/

#include "TigerFluidPropertiesTP.h"

template <>
InputParameters
validParams<TigerFluidPropertiesTP>()
{
  InputParameters params = validParams<GeneralUserObject>();
  return params;
}

TigerFluidPropertiesTP::TigerFluidPropertiesTP(const InputParameters & parameters)
  : GeneralUserObject(parameters)
{
}

void
TigerFluidPropertiesTP::execute()
{
}

void
TigerFluidPropertiesTP::initialize()
{
}

void
TigerFluidPropertiesTP::finalize()
{
}
