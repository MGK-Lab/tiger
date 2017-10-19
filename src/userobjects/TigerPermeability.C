/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/

#include "TigerPermeability.h"

template <>
InputParameters
validParams<TigerPermeability>()
{
  InputParameters params = validParams<GeneralUserObject>();
  return params;
}

TigerPermeability::TigerPermeability(const InputParameters & parameters)
  : GeneralUserObject(parameters)
{
}

void
TigerPermeability::execute()
{
}

void
TigerPermeability::initialize()
{
}

void
TigerPermeability::finalize()
{
}
