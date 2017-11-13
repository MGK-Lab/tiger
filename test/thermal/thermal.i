[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 10
  ny = 10
  nz = 10
[]

[UserObjects]
  [./water_uo]
    type = TigerFluidConst
  [../]
[]

[Materials]
  [./rock_t]
    type = TigerRockMaterialT
    pressure = 1.0e6
    temperature = 100.0
    fp_UO = water_uo
    porosity = 0.4
    conductivity_type = isotropic
    lambda = 2
    density = 2600
    specific_heat = 840
  [../]
[]

[BCs]
  [./front]
    type =  DirichletBC
    variable = temperature
    boundary = 'front'
    value = 0
  [../]
  [./back]
    type =  DirichletBC
    variable = temperature
    boundary = 'back'
    value = 100.0
  [../]
[]

[Variables]
  [./temperature]
    initial_condition = 50.0
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
  num_steps = 200
  end_time = 4.0e5
  l_tol = 1e-10 #difference between first and last linear step
  nl_rel_step_tol = 1e-14 #machine percision
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
[]
