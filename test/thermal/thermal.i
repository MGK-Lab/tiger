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
    specific_heat = 4.2e-2 #unreal value to check the transient stage
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
    specific_heat = 0.84e-2 #unreal value to check the transient stage
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
    type = TimeDerivative
    variable = temperature
  [../]
[]

[Executioner]
  type = Transient
  dt = 2.0
  end_time = 10.0
  l_tol = 1e-10 #difference between first and last linear step
  nl_rel_step_tol = 1e-14 #machine percision
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
[]
