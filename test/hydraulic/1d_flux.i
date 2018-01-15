[Mesh]
  type = GeneratedMesh
  dim = 1
  xmin = 0
  xmax = 20
  nx = 100
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
[]

[Materials]
  [./rock_h1]
    type = TigerRockMaterialH
    pressure = 1.0e6
    temperature = 100.0
    fp_UO = water_uo
    #has_gravity = true
    #gravity_acceleration = 9.81
    porosity = 0.0
    compressibility = 7.5e-8
    kf_UO = rock_uo1
    material_type = matrix
  [../]
[]

[BCs]
  [./left]
    type = NeumannBC
    variable = pressure
    boundary = left
    value = -0.04
  [../]
  [./right]
    type = DirichletBC
    variable = pressure
    boundary = right
    value = 0.0
  [../]
[]

[AuxVariables]
  [./vx]
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
  num_steps = 50
  end_time = 1000.0
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
