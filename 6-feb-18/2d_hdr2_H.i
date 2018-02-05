[Mesh]
  type = FileMesh
  file = ex_hdr2.msh
[]

[UserObjects]
  [./water_uo]
    type = TigerFluidConst
  [../]
  [./matrix_uo1]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '???'
  [../]
[]

[Materials]
  [./matrix_h]
    type = TigerRockMaterialH
    fp_UO = water_uo
    kf_UO = matrix_uo1
    porosity = 0.1
    compressibility = 1.0e-10
  [../]
[]

[BCs]
  [./whole_h]
    type = DirichletBC
    variable = pressure
    boundary = circum
    value = ???
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
