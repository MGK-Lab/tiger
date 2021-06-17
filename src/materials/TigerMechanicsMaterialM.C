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

#include "TigerMechanicsMaterialM.h"
#include "MooseMesh.h"

registerMooseObject("TigerApp", TigerMechanicsMaterialM);

template <>
InputParameters
validParams<TigerMechanicsMaterialM>()
{
  InputParameters params = validParams<Material>();

  params.addRequiredParam<Real>("specific_density",
        "specific density of rock for calculating bulk density (kg/m^3)");
  params.addParam<Real>("biot_coefficient", 1.0,
        "Biot's coefficient for poromechanics");
  params.addRequiredCoupledVar("displacements",
        "The displacements variables");
  params.addClassDescription("Mechanics material for mechanics kernels");

  return params;
}

TigerMechanicsMaterialM::TigerMechanicsMaterialM(const InputParameters & parameters)
  : Material(parameters),
    _rho_b(declareProperty<Real>("bulk_density")),
    _biot(declareProperty<Real>("biot_coefficient")),
    _n(getMaterialProperty<Real>("porosity")),
    _b(getParam<Real>("biot_coefficient")),
    _density(getParam<Real>("specific_density")),
    _ndisp(coupledComponents("displacements")),
    _grad_disp(coupledGradients("displacements")),
    _vol_strain_rate(declareProperty<Real>("volumetric_strain_rate_HM")),
    _vol_total_strain(declareProperty<Real>("total_volumetric_strain_HM"))
{
  if (_ndisp != _mesh.dimension())
    paramError("displacements", "The number of variables supplied must match the mesh dimension.");

  if (_is_transient)
    _grad_disp_old = coupledGradientsOld("displacements");

  // set unused dimensions to zero
  _grad_disp.resize(3, &_grad_zero);
  _grad_disp_old.resize(3, &_grad_zero);
}

void
TigerMechanicsMaterialM::computeQpProperties()
{
  _rho_b[_qp] = (1.0 - _n[_qp]) * _density;
  _biot[_qp] = _b;

  RankTwoTensor A((*_grad_disp[0])[_qp],
                  (*_grad_disp[1])[_qp],
                  (*_grad_disp[2])[_qp]); // Deformation gradient

  _vol_total_strain[_qp] = A.trace();

  if (_is_transient)
  {
    RankTwoTensor Abar((*_grad_disp_old[0])[_qp],
                       (*_grad_disp_old[1])[_qp],
                       (*_grad_disp_old[2])[_qp]); // Old Deformation gradient
    A -= Abar; // A = grad_disp - grad_disp_old
    RankTwoTensor total_strain_increment = 0.5 * (A + A.transpose());
    _vol_strain_rate[_qp] = total_strain_increment.trace() / _dt;
  }
  else
    _vol_strain_rate[_qp] = 0.0;
}
