[Mesh]
  type = FileMesh
  file = 3d_curvedfrac.msh
[]

[UserObjects]
  [./water_uo]
    type = TigerFluidConst
  [../]
[]

[GlobalParams]
  porosity = 0.0
  conductivity_type = isotropic
  density = 2600
  specific_heat = 840
  fp_UO = water_uo
[]

[Materials]
  [./rock_t1]
    type = TigerUncoupledThermalMaterialTH
    lambda = 2
    block = 'b1 b2'
  [../]
  [./rock_t2]
    type = TigerUncoupledThermalMaterialTH
    lambda = 200
    block = frac
  [../]
[]

[BCs]
  [./front]
    type =  DirichletBC
    variable = temperature
    boundary = top
    value = 0
  [../]
  [./back]
    type =  DirichletBC
    variable = temperature
    boundary = bottom
    value = 100.0
  [../]
[]

[Variables]
  [./temperature]
    initial_condition = 0.0
  [../]
[]

[Kernels]
  [./T_diff]
    type = TigerDiffusionKernelT
    variable = temperature
  [../]
  [./T_dt]
    type = TigerTimeDerivativeT
    variable = temperature
  [../]
[]

[Executioner]
  type = Transient
  num_steps = 10
  end_time = 1.0e5
  l_tol = 1e-10 #difference between first and last linear step
  nl_rel_step_tol = 1e-14 #machine percision
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
[]
