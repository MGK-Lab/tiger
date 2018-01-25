[Mesh]
  type = FileMesh
  file = ex_hdr.msh
[]

[UserObjects]
  [./water_uo]
    type = TigerFluidConst
    density = 1000
    viscosity = 1e-4
    specific_heat = 4200
    conductivity = 0.65
    compressibility = 4.0e-10
  [../]
[]

[Materials]
  [./matrix_t]
    type = TigerRockMaterialT
    fp_UO = water_uo
    porosity = 0.0
    conductivity_type = isotropic
    mean_calculation_type = geometric
    lambda = 3
    density = 2600
    specific_heat = 950
    #block = 'matrix'
  [../]
  #[./fracture_t]
  #  type = TigerRockMaterialT
  #  fp_UO = water_uo
  #  porosity = 1.0
  #  conductivity_type = isotropic
  #  mean_calculation_type = geometric
  #  lambda = 3
  #  density = 2600
  #  specific_heat = 950
  #  block = 'frac'
  #[../]
[]

[BCs]
  [./whole_t]
    type =  DirichletBC
    variable = temperature
    boundary = circum
    value = 300
  [../]
  [./well_t]
    type =  DirichletBC
    variable = temperature
    boundary = inject
    value = 70
  [../]
[]

[Variables]
  [./temperature]
    initial_condition = 300
  [../]
[]

[Kernels]
  [./T_diff]
    type = TigerDiffusionKernelT
    variable = temperature
  [../]
  [./T_time]
    type = TigerTimeDerivativeT
    variable = temperature
  [../]
[]
[Executioner]
  type = Transient
  num_steps = 50
  end_time = 1e10
  l_tol = 1e-10 #difference between first and last linear step
  nl_rel_step_tol = 1e-14 #machine percision
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
  #solve_type = PJFNK
  #petsc_options_iname = '-ksp_type -snes_type -pc_type -pc_factor_shift_type -pc_factor_shift_amount -snes_atol -snes_rtol -snes_max_it'
  #petsc_options_value = '  gmres     newtontr     lu          NONZERO               1E-12               1E-10       1E-15       250     '
[]

[Outputs]
  exodus = true
  print_linear_residuals = true
[]
