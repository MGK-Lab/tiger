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

  params.addParam<Real>("biot_coefficient", 1.0,
        "Biot's coefficient for poromechanics");
  params.addCoupledVar("disps",
        "The displacements variables (they are required if the incrimental is false)");
  params.addRequiredParam<bool>("incremental",
        "Incremental or total strain approach similar to TensorMechanics Action");
  params.addParam<std::string>("base_name", "the identical base name provided "
        "in TensorMechanics Action");
  params.addClassDescription("Mechanics material for mechanics kernels");

  return params;
}

TigerMechanicsMaterialM::TigerMechanicsMaterialM(const InputParameters & parameters)
  : Material(parameters),
    _biot(declareProperty<Real>("biot_coefficient")),
    _ndisp(coupledComponents("disps")),
    _vol_strain_rate(declareProperty<Real>("volumetric_strain_rate_HM")),
    _vol_total_strain(declareProperty<Real>("total_volumetric_strain_HM")),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _TenMech_total_strain(getMaterialProperty<RankTwoTensor>(_base_name + "total_strain")),
    _b(getParam<Real>("biot_coefficient")),
    _incremental(getParam<bool>("incremental"))
{
  _TenMech_strain_rate = _incremental ?
              &getMaterialProperty<RankTwoTensor>(_base_name + "strain_rate") : NULL;

  if (!_incremental && _is_transient)
  {
    if (_ndisp != _mesh.dimension())
      paramError("disps", "The number of displacement variables supplied must "
                  "match the mesh dimension or they are not supplied at all"
                  " in the case of total strain approach");

    _grad_disp = coupledGradients("disps");
    _grad_disp_old = coupledGradientsOld("disps");
    // set unused dimensions to zero
    _grad_disp.resize(3, &_grad_zero);
    _grad_disp_old.resize(3, &_grad_zero);
  }
}

void
TigerMechanicsMaterialM::computeQpProperties()
{
  _biot[_qp] = _b;
  _vol_total_strain[_qp] = _TenMech_total_strain[_qp].trace();

  if (_incremental && _is_transient)
    _vol_strain_rate[_qp] = (*_TenMech_strain_rate)[_qp].trace();


  if (!_incremental && _is_transient)
  {
    RankTwoTensor A   ((*_grad_disp[0])[_qp],
                       (*_grad_disp[1])[_qp],
                       (*_grad_disp[2])[_qp]); // Deformation gradient
    RankTwoTensor Abar((*_grad_disp_old[0])[_qp],
                       (*_grad_disp_old[1])[_qp],
                       (*_grad_disp_old[2])[_qp]); // Old Deformation gradient
    A -= Abar; // A = grad_disp - grad_disp_old
    RankTwoTensor total_strain_increment = 0.5 * (A + A.transpose());
    _vol_strain_rate[_qp] = total_strain_increment.trace() / _dt;
  }
}
