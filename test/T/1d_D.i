[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 10
[]

[Modules]
  [./FluidProperties]
    [./water_uo]
      type = TigerWaterConst
    [../]
  [../]
[]

[Materials]
  [./rock_t]
    type = TigerUncoupledThermalMaterialTH
    fp_UO = water_uo
    porosity = 0
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
    boundary = left
    value = 0
  [../]
  [./back]
    type =  DirichletBC
    variable = temperature
    boundary = right
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
  num_steps = 50
  end_time = 2.0e5
  l_tol = 1e-10 #difference between first and last linear step
  nl_rel_step_tol = 1e-14 #machine percision
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
[]
