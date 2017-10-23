[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 10
  ny = 10
  nz = 10
[]

[UserObjects]
  [./water_uo]
    type =  TigerWaterProperties
  [../]
  [./rock_uo]
    type =  TigerPermeabilityRockConst
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
    type = TigerRockMaterial
    compressibility = 1.0e-9
    permeability_type = isotropic
    k0 = '1.0e-8'
    porosity = 0.4
    kf_UO = rock_uo
  [../]
[]

[BCs]
  [./left]
    type =  DirichletBC
    variable = pressure
    boundary = 'left'
    value = 0.0
  [../]
  [./right]
    type =  DirichletBC
    variable = pressure
    boundary = 'right'
    value = 100
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
  [./vz]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./vis]
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
  [./viscosity]
    type = MaterialRealAux
    variable = vis
    property = viscosity
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
  #[./time]
  #  type = TimeDerivative
  #  variable = pressure
  #[../]
[]

[Executioner]
  type = Steady
  #dt = 0.001
  #num_steps = 100
  nl_abs_tol = 1e-14
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
[]
