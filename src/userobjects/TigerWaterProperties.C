/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/

#include "TigerWaterProperties.h"

template <>
InputParameters
validParams<TigerWaterProperties>()
{
  InputParameters params = validParams<TigerFluidPropertiesTP>();
  params.addClassDescription("Water properties varies with tempreture and pressure (FRACture model)");
  return params;
}

TigerWaterProperties::TigerWaterProperties(const InputParameters & parameters)
  : TigerFluidPropertiesTP(parameters)
{
}

std::string
TigerWaterProperties::fluidName() const
{
  return "Water (FRACture model)";
}

Real
TigerWaterProperties::rho(Real pressure, Real temperature) const
{
  Real _a = -9.9559 + 7.0845*std::exp(-1.638e-4*temperature)+3.909*std::exp(2.551e-10*pressure);
  return (-3.033405 + 10.128163*_a - 8.750567*_a*_a + 2.663107*_a*_a*_a)*1.0e3;
}


Real
TigerWaterProperties::mu(Real temperature) const
{
  return 2.4e-5*std::pow(10.0, 248.37/(temperature+133.15));
}

Real
TigerWaterProperties::beta(Real /*pressure*/, Real /*temperature*/) const
{
  return 4.4e-10;
}

