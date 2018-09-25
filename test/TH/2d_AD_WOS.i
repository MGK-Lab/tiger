[Mesh]
  type = FileMesh
  file = 2d_ad.msh
  uniform_refine = 1
[]

[Modules]
  [./FluidProperties]
    [./water_uo]
      type = TigerWaterConst
      density = 1
      cp = 1
      thermal_conductivity = 1e-4
    [../]
  [../]
[]

[UserObjects]
  [./matrix_uo1]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '1.0e-8'
  [../]
[]

[Materials]
  [./rock_g]
    type = TigerGeometryMaterial
    porosity = 1
  [../]
  [./matrix_h]
    type = TigerRockMaterialH
    fp_uo = water_uo
    kf_uo = matrix_uo1
    compressibility = 1.0e-10
  [../]
  [./matrix_t]
    type = TigerCoupledThermalMaterialTH
    fp_uo = water_uo
    pressure =  pressure
    conductivity_type = isotropic
    mean_calculation_type = arithmetic
    lambda = 1e-4
    density = 1
    specific_heat = 1
    has_supg = true
    supg_eff_length = average
    supg_coeficient = optimal
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
    pressure_varible = pressure
  [../]
  [./T_diff]
    type = TigerDiffusionKernelT
    variable = temperature
  [../]
[]

[Executioner]
  type = Steady
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
  print_linear_residuals = true
[]
