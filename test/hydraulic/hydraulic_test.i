[Mesh]
  type = FileMesh
  file =  m5.e
[]

[UserObjects]
  [./water_uo]
    type = TigerFluidConst
  [../]
  [./rock_uo]
    type =  TigerPermeabilityConst
  [../]
[]

[Materials]
  [./rock_h1]
    type = TigerRockMaterialH
    pressure = 1.0e6
    temperature = 100.0
    fp_UO = water_uo
    #has_gravity = true
    #gravity_acceleration = 9.81
    porosity = 0.4
    compressibility = 1.0e-9
    permeability_type = orthotropic
    k0 = '2.0e-10 2.0e-10 2.0e-10'
    kf_UO = rock_uo
    material_type = matrix
    block = 0
  [../]
  [./rock_h2]
    type = TigerRockMaterialH
    pressure = 1.0e6
    temperature = 100.0
    fp_UO = water_uo
    #has_gravity = true
    #gravity_acceleration = 9.81
    porosity = 0.4
    compressibility = 1.0e-9
    permeability_type = orthotropic
    k0 = '10.0e-10 10.0e-10'
    kf_UO = rock_uo
    material_type = fracture
    block = 1
  [../]
  [./rock_h3]
    type = TigerRockMaterialH
    pressure = 1.0e6
    temperature = 100.0
    fp_UO = water_uo
    #has_gravity = true
    #gravity_acceleration = 9.81
    porosity = 1.0
    compressibility = 1.0e-9
    permeability_type = isotropic
    k0 = '20.0e-10'
    kf_UO = rock_uo
    material_type = well
    block = 2
  [../]
[]

[BCs]
  [./front]
    type = DirichletBC
    variable = pressure
    boundary = 6
    value = 0
  [../]
  [./back]
    type = DirichletBC
    variable = pressure
    boundary = 5
    value = 9810.0
  [../]
[]

[AuxVariables]
  [./ax]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./ay]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./az]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./bx]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./by]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./bz]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./cx]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./cy]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./cz]
    family = MONOMIAL
    order = CONSTANT
  [../]
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
  [./ax_ker]
    type = test
    variable = ax
    i = 0
    j = 0
  [../]
  [./ay_ker]
    type = test
    variable = ay
    i = 1
    j = 0
  [../]
  [./az_ker]
    type = test
    variable = az
    i = 2
    j = 0
  [../]
  [./bx_ker]
    type = test
    variable = bx
    i = 0
    j = 1
  [../]
  [./by_ker]
    type = test
    variable = by
    i = 1
    j = 1
  [../]
  [./bz_ker]
    type = test
    variable = bz
    i = 2
    j = 1
  [../]
  [./cx_ker]
    type = test
    variable = cx
    i = 0
    j = 2
  [../]
  [./cy_ker]
    type = test
    variable = cy
    i = 1
    j = 2
  [../]
  [./cz_ker]
    type = test
    variable = cz
    i = 2
    j = 2
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
  dt = 1.0
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
