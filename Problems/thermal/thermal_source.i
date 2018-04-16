[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 1
  ny = 1
  nz = 100
  zmax = 0
  zmin = -10000
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
    porosity = 0.0
    conductivity_type = isotropic
    lambda = 3
    density = 0.1 #artificial value to reach steady in the transient simulation
    specific_heat = 0.1 #artificial value to reach steady in the transient simulation
  [../]
[]

[BCs]
  [./front]
    type =  DirichletBC
    variable = temperature
    boundary = 'front'
    value = 10.0
  [../]
  [./back_n]
    type =  NeumannBC
    variable = temperature
    boundary = 'back'
    value = 8.0e-2
  [../]
[]

[Variables]
  [./temperature]
    initial_condition = 10.0
  [../]
[]

[Kernels]
  [./T_diff]
    type = TigerDiffusionKernelT
    variable = temperature
  [../]
  [./T_Source]
    type = TigerHeatSourceT
    variable = temperature
    value = 1.0e-6
  [../]
  [./T_dt]
    type = TigerTimeDerivativeT
    variable = temperature
  [../]
[]

[Executioner]
  type = Transient
  num_steps = 50
  end_time = 2.0e6
  #type = Steady
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
