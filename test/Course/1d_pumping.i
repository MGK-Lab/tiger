[Mesh]
  type = GeneratedMesh
  dim = 1
  xmin = 0
  xmax = 30
  nx = 100
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

[Problem]
  coord_type = RZ
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
  [./right]
    type = PresetBC
    variable = pressure
    boundary = right
    value = 1.818e5
  [../]
[]

[Functions]
  [./analytical_function]
    type = ParsedFunction
    vars = 'Q T R'
    vals = '0.04 2.3e-3 30'
    value = '1.818e5-1e4*Q/(2*3.14*T)*log(R/x)'
  [../]
[]

[AuxVariables]
  [./vx]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./analytical_solution]
    family = LAGRANGE
    order = FIRST
  [../]
[]

[AuxKernels]
  [./vx_ker]
    type = TigerDarcyVelocityComponent
    gradient_variable = pressure
    variable =  vx
    component = x
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
    initial_condition = 1.818e5
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

[Executioner]
  type = Transient
  num_steps = 50
  end_time = 500.0
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
