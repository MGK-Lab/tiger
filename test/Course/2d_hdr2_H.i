[Mesh]
  type = FileMesh
  file = ex_hdr2.msh
[]

[UserObjects]
  [./water_uo]
    type = TigerFluidConst
    density = 1000
    viscosity = 1.8e-4
    specific_heat = 4200
    conductivity = 0.65
    compressibility = 4.0e-10
  [../]
  [./matrix_uo1]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '7.0e-14'
  [../]
  [./fracture_uo1]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '8.3333e-10'
  [../]
[]

[Materials]
  [./matrix_h]
    type = TigerRockMaterialH
    fp_UO = water_uo
    kf_UO = matrix_uo1
    porosity = 0.1
    compressibility = 1.0e-10
    block = 'unit'
  [../]
  [./fracure_h]
    type = TigerRockMaterialH
    fp_UO = water_uo
    kf_UO = fracture_uo1
    porosity = 1.0
    compressibility = 4.0e-10
    block = 'frac1 frac2'
  [../]
[]

[BCs]
  [./whole_h]
    type = DirichletBC
    variable = pressure
    boundary = circum
    value = 1e7
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
    initial_condition = 1e7
  [../]
[]

[DiracKernels]
  [./pump_in]
    type = TigerPointSourceH
    point = '150.0 250.0 0.0'
    mass_flux = 3.0
    variable = pressure
  [../]
  [./pump_out]
    type = TigerPointSourceH
    point = '350.0 250.0 0.0'
    mass_flux = -3.0
    variable = pressure
  [../]
[]

[Kernels]
  [./H_diff]
    type = TigerKernelH
    variable = pressure
  [../]
  [./H_time]
    type = TigerTimeDerivativeH
    variable = pressure
  [../]
[]

[Preconditioning]
  [./pre]
    type = SMP
    full = true
    petsc_options_iname = '-pc_type -pc_hypre_type -snes_atol -snes_rtol -snes_max_it'
    petsc_options_value = ' hypre    boomeramg      1E-10      1E-15      250     '
  [../]
[]

[Executioner]
  #type = Steady
  type = Transient
  num_steps = 100
  end_time = 1e4
  solve_type = NEWTON
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
[]
