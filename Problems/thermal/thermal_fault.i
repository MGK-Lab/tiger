[Mesh]
  type = FileMesh
  file = Inclined_Frac.msh
[]

[UserObjects]
  [./water_uo]
    type = TigerFluidConst
  [../]
[]

[Materials]
  [./rock_t1]
    type = TigerRockMaterialT
    pressure = 1.0e6
    temperature = 100.0
    fp_UO = water_uo
    porosity = 0.0
    conductivity_type = isotropic
    lambda = 2
    density = 2600
    specific_heat = 840
    block = 'b1 b2'
    material_type = matrix
  [../]
  [./rock_t2]
    type = TigerRockMaterialT
    pressure = 1.0e6
    temperature = 100.0
    fp_UO = water_uo
    porosity = 0.0
    conductivity_type = isotropic
    lambda = 200
    density = 2600
    specific_heat = 840
    block = frac
    material_type = fracture
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
  num_steps = 50
  end_time = 6.0e5
  l_tol = 1e-10 #difference between first and last linear step
  nl_rel_step_tol = 1e-14 #machine percision
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
[]
