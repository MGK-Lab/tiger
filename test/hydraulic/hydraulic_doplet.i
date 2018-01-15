[Mesh]
  type = FileMesh
  file = doplet_sinked_well.e
[]

[UserObjects]
  [./water_uo]
    type = TigerFluidConst
  [../]
  [./rock_uo1]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '2.0e-10'
  [../]
  [./rock_uo2]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '20.0e-10'
  [../]
  [./rock_uo3]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '80.0e-10'
  [../]
[]

[Materials]
  [./rock_h1]
    type = TigerRockMaterialH
    pressure = 1.0e6
    temperature = 100.0
    fp_UO = water_uo
    has_gravity = true
    gravity_acceleration = 9.81
    porosity = 0.2
    compressibility = 1.0e-9
    kf_UO = rock_uo1
    material_type = matrix
    block = 0
  [../]
  [./rock_h2]
    type = TigerRockMaterialH
    pressure = 1.0e6
    temperature = 100.0
    fp_UO = water_uo
    has_gravity = true
    gravity_acceleration = 9.81
    porosity = 0.5
    compressibility = 1.0e-9
    kf_UO = rock_uo2
    material_type = fracture
    block = 1
  [../]
  [./rock_h3]
    type = TigerRockMaterialH
    pressure = 1.0e6
    temperature = 100.0
    fp_UO = water_uo
    has_gravity = true
    gravity_acceleration = 9.81
    porosity = 1.0
    compressibility = 1.0e-9
    kf_UO = rock_uo3
    material_type = well
    block = 2
  [../]
  [./rock_h4]
    type = TigerRockMaterialH
    pressure = 1.0e6
    temperature = 100.0
    fp_UO = water_uo
    has_gravity = true
    gravity_acceleration = 9.81
    porosity = 1.0
    compressibility = 1.0e-9
    kf_UO = rock_uo3
    material_type = well
    block = 3
  [../]
[]

[DiracKernels]
  [./well_out]
    type = TigerPointSourceH
    point = '19.0 19.0 22.0'
    mass_flux = +30.0
    variable = pressure
  [../]
  [./well_in]
    type = TigerPointSourceH
    point = '-19.0 -19.0 22.0'
    mass_flux = -30.0
    variable = pressure
  [../]
[]

[BCs]
  [./front]
    type = DirichletBC
    variable = pressure
    boundary = 6
    value = 1000
  [../]
  #[./back]
  #  type = DirichletBC
  #  variable = pressure
  #  boundary = 5
  #  value = 9810.0
  #[../]
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
  [./vz]
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
  [./vz_ker]
    type = TigerDarcyVelocityComponent
    gradient_variable = pressure
    variable =  vz
    component = z
  [../]
[]

[Variables]
  [./pressure]
  [../]
[]

[Kernels]
  [./diff]
    type = TigerKernelH
    variable = pressure
  [../]
  [./time]
    type = TigerTimeDerivativeH
    variable = pressure
  [../]
[]

[Executioner]
  type = Transient
  num_steps = 10
  end_time = 2000.0
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
