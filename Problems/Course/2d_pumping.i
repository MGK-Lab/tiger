[Mesh]
  type = FileMesh
  file = Circle.msh
[]

[UserObjects]
  [./water_uo]
    type = TigerFluidConst
  [../]
  [./rock_uo1]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '2.3e-10'
  [../]
[]

[Materials]
  [./rock_h1]
    type = TigerRockMaterialH
    fp_UO = water_uo
    porosity = 0.0
    compressibility = 7.5e-8
    kf_UO = rock_uo1
  [../]
[]

[BCs]
  [./circum]
    type = DirichletBC
    variable = pressure
    boundary = circum
    value = 1.368e5
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

[Functions]
  [./analytical_function]
    type = ParsedFunction
    vars = 'Q T R'
    vals = '0.04 2.3e-3 250'
    value = '1.368e5-1e4*Q/(2*3.14*T)*log(R/sqrt(x*x+y*y))'
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
  [./a_ker]
    type = FunctionAux
    function = analytical_function
    variable = analytical_solution
    execute_on = initial
  [../]
[]

[Variables]
  [./pressure]
    initial_condition = 1.368e5
  [../]
[]

[DiracKernels]
  [./pumpout]
    type = TigerPointSourceH
    point = '0.0 0.0 0.0'
    mass_flux = -40.0
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

[Preconditioning]
  [./pre]
    type = SMP
    full = true
    petsc_options_iname = '-ksp_type -snes_type -pc_type -pc_factor_shift_type -pc_factor_shift_amount -snes_atol -snes_rtol -snes_max_it'
    petsc_options_value = '  gmres     newtontr     asm          NONZERO               1E-12               1E-10       1E-15       250     '
  [../]
[]

[Executioner]
  #type = Steady
  type = Transient
  num_steps = 50
  end_time = 5e4
  solve_type = NEWTON
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
[]
