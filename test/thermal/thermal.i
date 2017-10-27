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
  [./water]
    type = TigerFluidMaterialTP
    pressure = 1.0e6
    temperature = 100.0
    fp_UO = water_uo
  [../]
  [./rock]
    type = TigerRockMaterialGeneral
    porosity = 0.4
  [../]
  [./rockt]
    type = TigerRockMaterialT
    conductivity_type = isotropic
    lambda = 2
    density = 2600
    specific_heat = 0.84e3
  [../]
[]

[BCs]
  [./front]
    type =  DirichletBC
    variable = pressure
    boundary = 'front'
    value = 0
  [../]
  #[./back]
  #  type =  DirichletBC
  #  variable = pressure
  #  boundary = 'back'
  #  value = 9810.0
  #[../]
[]

[Variables]
  [./pressure]
  [../]
[]

[Kernels]
  [./diff]
    type = Diffusion
    variable = pressure
  [../]
[]

[Executioner]
  type = Steady
  l_tol = 1e-10 #difference between first and last linear step
  nl_rel_step_tol = 1e-14 #machine percision
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
[]
