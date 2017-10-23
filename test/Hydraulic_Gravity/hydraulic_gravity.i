[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 10
  ny = 10
  nz = 10
[]

[UserObjects]
  [./water_uo]
    type =  TigerWaterPropertiesConst
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
    #has_gravity = true
    #gravity_acceleration = 9.81
    compressibility = 1.0e-9
    permeability_type = isotropic
    k0 = '1.0e-10'
    porosity = 0.4
    kf_UO = rock_uo
  [../]
[]

[BCs]
  [./left]
    type =  DirichletBC
    variable = pressure
    boundary = 'left'
    value = 100.0
  [../]
  [./right]
    type =  DirichletBC
    variable = pressure
    boundary = 'right'
    value = 150.0
  [../]
[]

#[AuxVariables]
#  [./vx]
#    family = MONOMIAL
#    order = CONSTANT
#  [../]
#  #[./vy]
#  #  family = MONOMIAL
#  #  order = CONSTANT
#  #[../]
#  #[./vz]
#  #  family = MONOMIAL
#  #  order = CONSTANT
#  #[../]
#[]
#
#[AuxKernels]
#  [./vx_ker]
#    type = TigerDarcyVelocityComponent
#    gradient_variable = pressure
#    variable =  vx
#    component = x
#  [../]
#  #[./vy_ker]
#  #  type = TigerDarcyVelocityComponent
#  #  gradient_variable = pressure
#  #  variable =  vy
#  #  component = y
#  #[../]
#  #[./vz_ker]
#  #  type = TigerDarcyVelocityComponent
#  #  gradient_variable = pressure
#  #  variable =  vz
#  #  component = z
#  #[../]
#[]

[Variables]
  [./pressure]
    initial_condition = 200.0
  [../]
[]

[Kernels]
  [./diff]
    type = TigerKernelH
    variable = pressure
  [../]
  [./time]
    type = TimeDerivative
    variable = pressure
  [../]
[]

[Executioner]
  type = Transient
  dt = 0.01
  num_steps = 10
  nl_abs_tol = 1e-10
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
[]
