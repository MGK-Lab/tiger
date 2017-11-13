[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 20
  ny = 20
  nz = 20
  zmax = 0.5
  zmin = -0.5
  ymax = 0.5
  ymin = -0.5
  xmax = 0.5
  xmin = -0.5
[]

[UserObjects]
  [./water_uo]
    type = TigerFluidConst
  [../]
  [./rock_uo]
    type =  TigerPermeabilityRockConst
  [../]
[]

[Materials]
  [./rock_h]
    type = TigerRockMaterialH
    pressure = 1.0e6
    temperature = 100.0
    fp_UO = water_uo
    #has_gravity = true
    #gravity_acceleration = 9.81
    porosity = 0.4
    compressibility = 1.0e-9
    permeability_type = isotropic
    k0 = '1.0e-10'
    kf_UO = rock_uo
  [../]
[]

[BCs]
  [./all]
    type =  PresetBC
    variable = pressure
    boundary = '0 1 2 3 4 5'
    value = 0.0
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

[DiracKernels]
  [./well_in]
    type = TigerPointSourceH
    point = '0.0 0.0 0.0'
    mass_flux = 1.0
    variable = pressure
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
  dt = 0.01
  end_time = 0.05
  l_tol = 1e-5 #difference between first and last linear step
  nl_rel_step_tol = 1e-14 #machine percision
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
  #print_linear_residuals = false
[]
