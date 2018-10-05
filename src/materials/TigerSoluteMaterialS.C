/**************************************************************************/
/*  TIGER - Hydro-thermal sImulator GEothermal Reservoirs                 */
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

#include "TigerSoluteMaterialS.h"
#include "MooseMesh.h"
#include "libmesh/quadrature.h"

registerMooseObject("TigerApp", TigerSoluteMaterialS);

template <>
InputParameters
validParams<TigerSoluteMaterialS>()
{
  InputParameters params = validParams<Material>();

  MooseEnum Advection
        ("pure_diffusion darcy_velocity user_velocity darcy_user_velocities",
        "darcy_velocity");
  params.addParam<MooseEnum>("advection_type", Advection,
        "Type of the velocity to simulate advection [pure_diffusion "
        "darcy_velocity user_velocity darcy_user_velocities]");
    params.addParam<bool>("output_Pe_Cr_numbers", false ,
        "calcuate Peclet and Courant numbers");
  params.addParam<bool>("has_supg", false ,
        "Is Streameline Upwinding / Petrov Galerkin (SU/PG) activated?");
  params.addParam<Real>("supg_coeficient_scale", 1.0 ,
        "The user defined factor to scale SU/PG coefficent (tau)");
  params.addParam<FunctionName>("user_velocity", 0.0,
        "a vector function to define the velocity field");
  params.addParam<UserObjectName>("supg_uo", "",
        "The name of the userobject for SU/PG");
  params.addRequiredParam<Real>("diffusion", "Molecular diffusion of component in water (m^2/s) - something like 2e-9");
  params.addParam<Real>("dispersion_longitudinal", 0, "Longitudinal dispersivity (m)");
  params.addParam<Real>("dispersion_transverse", 0, "Transverse dispersivity (m)");
  params.addParam<Real>("formation_factor", 1, "The formation factor, 0.1 for clays, 0.7 for sand, depending on the tortuosity of the porous medium");
  params.addClassDescription("Solute material for solute kernels");

  return params;
}

TigerSoluteMaterialS::TigerSoluteMaterialS(const InputParameters & parameters)
  : Material(parameters),
    _at(getParam<MooseEnum>("advection_type")),
    _has_PeCr(getParam<bool>("output_Pe_Cr_numbers")),
    _has_supg(getParam<bool>("has_supg")),
    _supg_scale(getParam<Real>("supg_coeficient_scale")),
    _TimeKernelS(declareProperty<Real>("TimeKernel_S")),
//    _dTimeKernelS_dPhi(declareProperty<Real>("dTimeKernelS_dPhi")), Needed if porosity is changing
    _SUPG_ind(declareProperty<bool>("solute_supg_indicator")),
    _av_ind(declareProperty<bool>("solute_av_dv_indicator")),
    _av(declareProperty<RealVectorValue>("solute_advection_velocity")),
    _SUPG_p(declareProperty<RealVectorValue>("solute_petrov_supg_p_function")),
    _n(getMaterialProperty<Real>("porosity")),
    _rot_mat(getMaterialProperty<RankTwoTensor>("lowerD_rotation_matrix")),
    _diffusion_molecular(getParam<Real>("diffusion")),
    _disp_l(getParam<Real>("dispersion_longitudinal")),
    _disp_t(getParam<Real>("dispersion_transverse")),
    _formation_factor(getParam<Real>("formation_factor")),
  //  _dispersion_tensor(declareProperty<RankTwoTensor>("dispersion_tensor")),
    _diffusion_factor(declareProperty<Real>("diffusion_factor")),
    _diffdisp(declareProperty<RankTwoTensor>("diffusion_dispersion"))
{
  _Pe = (_has_PeCr || _has_supg) ?
              &declareProperty<Real>("thermal_peclet_number") : NULL;
  _Cr = (_has_PeCr || _has_supg) ?
              &declareProperty<Real>("thermal_courant_number") : NULL;
  _vel_func = (_at == AT::user_velocity || _at == AT::darcy_user_velocities) ?
              &getFunction("user_velocity") : NULL;
  _supg_uo = (parameters.isParamSetByUser("supg_uo")) ?
              &getUserObject<TigerSUPG>("supg_uo") : NULL;
  _dv = (_at == AT::darcy_velocity || _at == AT::darcy_user_velocities) ?
              &getMaterialProperty<RealVectorValue>("darcy_velocity") : NULL;
}


void
TigerSoluteMaterialS::computeQpProperties()
{
    // Chemical kernel for calculating the time derivative, n0 is porosity
    _TimeKernelS[_qp] = _n[_qp];

    _diffusion_factor[_qp] = _diffusion_molecular * _n[_qp] * _formation_factor;

    // Push-back the internal tensors to external tensors which are accessable from AuxSystem and Kernel
//    _dispersion_tensor[_qp] = DispersionTensorCalculator(_av[_qp], _disp_l, _disp_t, _current_elem->dim(), _mesh.dimension(), _diffusion_factor[_qp]);

    _diffdisp[_qp] = DispersionTensorCalculator(_av[_qp], _disp_l, _disp_t, _current_elem->dim(), _mesh.dimension(), _diffusion_factor[_qp]);

    // Rotate for lower dimensional cases
//    if (_current_elem->dim() < _mesh.dimension())
//    _dispersion_tensor[_qp].rotate(_rot_mat[_qp]);
  //  if (_current_elem->dim() < _mesh.dimension())
  //  _diffdisp[_qp].rotate(_rot_mat[_qp]);

    switch (_at)
  {
    case AT::pure_diffusion:
      _av[_qp].zero();
      _av_ind[_qp] = false;
    break;
    case AT::darcy_velocity:
      _av[_qp] = (*_dv)[_qp];
      _av_ind[_qp] = true;
      break;
    case AT::user_velocity:
      _av[_qp] = _vel_func->vectorValue(_t, _q_point[_qp]);
      _av_ind[_qp] = false;
      break;
    case AT::darcy_user_velocities:
      _av[_qp] = (*_dv)[_qp] + _vel_func->vectorValue(_t, _q_point[_qp]);
      _av_ind[_qp] = true;
      break;
  }

  Real lambda = _diffdisp[_qp].trace() / (_current_elem->dim() * _TimeKernelS[_qp]);
  if (_has_PeCr && !_has_supg)
    _supg_uo->PeCrNrsCalculator(lambda, _dt, _current_elem, _av[_qp], (*_Pe)[_qp], (*_Cr)[_qp]);

    if (_has_supg)
      {
        // should be multiplied by the gradient of the test function to build the Petrov Galerkin P function
        _supg_uo->SUPGCalculator(lambda, _dt, _current_elem, _av[_qp], _SUPG_p[_qp], (*_Pe)[_qp], (*_Cr)[_qp]);
        _SUPG_p[_qp] *= _supg_scale;

        if (_SUPG_p[_qp].norm() != 0.0)
          _SUPG_ind[_qp] = true;
        else
          _SUPG_ind[_qp] = false;
      }
      else
        _SUPG_ind[_qp] = false;

}

RankTwoTensor
TigerSoluteMaterialS::DispersionTensorCalculator(const RealVectorValue & darcy_v, Real const & disp_l, Real const & disp_t, int dim, int dimMesh, Real diffusion_factor)
{
 if (dim == dimMesh)
 {
  Real vmag = (1/(std::sqrt(darcy_v(0) * darcy_v(0) + darcy_v(1) * darcy_v(1) + darcy_v(2) * darcy_v(2))));
  if (dim == 1)
  {
    if (darcy_v(0) != 0 )
    {
    Real d00 = vmag * (disp_t * (darcy_v(1) * darcy_v(1) + darcy_v(2) * darcy_v(2)) + disp_l * darcy_v(0) * darcy_v(0));
    d00 += diffusion_factor;

    _dispersion_ten = (RankTwoTensor(d00, 0., 0., 0., 0., 0.));
    }
    else
    _dispersion_ten = (RankTwoTensor(diffusion_factor, 0., 0., 0., 0., 0.));
  }

  if (dim == 2 )
  {
    if (darcy_v(0) != 0 || darcy_v(1) != 0)
    {
    Real d00 = vmag * ((disp_t * (darcy_v(1) * darcy_v(1) + darcy_v(2) * darcy_v(2)) + disp_l * darcy_v(0) * darcy_v(0)));
    d00 += diffusion_factor;
    Real d01 = vmag * (((disp_l - disp_t) * darcy_v(0) * darcy_v(1)));
    d01 += diffusion_factor;
    Real d11 = vmag * ((disp_t * (darcy_v(0) * darcy_v(0) + darcy_v(2) * darcy_v(2)) + disp_l * darcy_v(1) * darcy_v(1)));
    d11 += diffusion_factor;

    _dispersion_ten = (RankTwoTensor(d00, d11, 0., 0., 0., d01));
    }
    else
    _dispersion_ten = (RankTwoTensor(diffusion_factor, diffusion_factor, 0., 0., 0., diffusion_factor));
  }

  if (dim == 3 )
  {
    if (darcy_v(0) != 0 || darcy_v(1) != 0 || darcy_v(2) != 0)
    {
    Real d00 = vmag * ((disp_t * (darcy_v(1) * darcy_v(1) + darcy_v(2) * darcy_v(2)) + disp_l * darcy_v(0) * darcy_v(0)));
    d00 += diffusion_factor;
    Real d01 = vmag * (((disp_l - disp_t) * darcy_v(0) * darcy_v(1)));
    d01 += diffusion_factor;
    Real d02 = vmag * (((disp_l - disp_t) * darcy_v(0) * darcy_v(2)));
    d02 += diffusion_factor;
    Real d11 = vmag * ((disp_t * (darcy_v(0) * darcy_v(0) + darcy_v(2) * darcy_v(2)) + disp_l * darcy_v(1) * darcy_v(1)));
    d11 += diffusion_factor;
    Real d12 = vmag * (((disp_l - disp_t) * darcy_v(1) * darcy_v(2)));
    d12 += diffusion_factor;
    Real d22 = vmag * ((disp_t * (darcy_v(0) * darcy_v(0) + darcy_v(1) * darcy_v(1)) + disp_l * darcy_v(2) * darcy_v(2)));
    d22 += diffusion_factor;

    _dispersion_ten = (RankTwoTensor(d00, d11, d22, d12, d02, d01));
    }
  else
  _dispersion_ten = (RankTwoTensor(diffusion_factor, diffusion_factor, diffusion_factor, diffusion_factor, diffusion_factor, diffusion_factor));
  }

  }
  return _dispersion_ten;
}
