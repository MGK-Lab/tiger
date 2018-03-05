[Mesh]
  type = FileMesh
  file = 2d_ad.msh
  uniform_refine = 1
[]

[UserObjects]
  [./water_uo]
    type = TigerFluidConst
    density = 1
    specific_heat = 1
    conductivity = 1e-4
  [../]
  [./matrix_uo1]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '1.0e-8'
  [../]
[]

[Materials]
  [./matrix_h]
    type = TigerRockMaterialH
    fp_UO = water_uo
    kf_UO = matrix_uo1
    porosity = 1.0
    compressibility = 1.0e-10
  [../]
  [./matrix_t]
    type = TigerRockMaterialT
    fp_UO = water_uo
    porosity = 1.0
    conductivity_type = isotropic
    mean_calculation_type = arithmetic
    lambda = 1e-4
    density = 1
    specific_heat = 1
  [../]
  [./advect_th]
    type = TigerAdvectionMaterialTH
    pressure =  pressure
    fp_UO = water_uo
    has_supg = true
    is_supg_consistent = true
    supg_eff_length = min
    supg_coeficient = critical
  [../]
[]

[BCs]
  [./lt_h]
    type = NeumannBC
    variable = pressure
    boundary = 'l1 l2 t'
    value = 0.707106781
  [../]
  [./c_h]
    type = DirichletBC
    variable = pressure
    boundary = 'c'
    value = 0
  [../]
  [./rb_h]
    type = NeumannBC
    variable = pressure
    boundary = 'r1 r2 b'
    value = -0.707106781
  [../]
  [./left_t]
    type = DirichletBC
    variable = temperature
    boundary = 'l1 t'
    value = 1
  [../]
  [./top_t]
    type = DirichletBC
    variable = temperature
    boundary = 'l2'
    value = 0
  [../]
[]

[AuxVariables]
  [./vx]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./vy]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./analytical_solution]
    family = LAGRANGE
    order = FIRST
  [../]
[]

[AuxKernels]
  [./vx_ker]
    type = TigerDarcyVelocityComponent
    gradient_variable = pressure
    variable =  vx
    component = x
  [../]
  [./vy_ker]
    type = TigerDarcyVelocityComponent
    gradient_variable = pressure
    variable =  vy
    component = y
  [../]
[]

[Variables]
  [./pressure]
  [../]
  [./temperature]
  [../]
[]

[Kernels]
  [./H_diff]
    type = TigerKernelH
    variable = pressure
  [../]
  [./T_advect]
    type = TigerAdvectionKernelTH
    variable = temperature
  [../]
  [./T_diff]
    type = TigerDiffusionKernelT
    variable = temperature
  [../]
[]

[Executioner]
  type = Steady
  solve_type = NEWTON
  petsc_options = '-snes_ksp_ew'
  petsc_options_iname = '-ksp_type -pc_type -snes_atol -snes_rtol -snes_max_it -ksp_max_it -sub_pc_type -sub_pc_factor_shift_type'
  petsc_options_value = 'gmres asm 1E-10 1E-10 200 500 lu NONZERO'
[]

[Outputs]
  exodus = true
  print_linear_residuals = true
[]
