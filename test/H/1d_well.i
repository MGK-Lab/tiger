[Mesh]
  type = GeneratedMesh
  dim = 1
  xmin = 0
  xmax = 10
  nx = 10
[]

[UserObjects]
  [./water_uo]
    type = TigerFluidConst
  [../]
  [./rock_uo1]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '1.0e-10'
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
  [./right]
    type = DirichletBC
    variable = pressure
    boundary = right
    value = 0.0
  [../]
[]

[Functions]
  [./analytical_function]
    type = ParsedFunction
    value = '2e5*x-2e6'
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
  [../]
[]

[DiracKernels]
  [./pumpout]
    type = TigerPointSourceH
    point = '0.0 0.0 0.0'
    mass_flux = -20.0
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
  end_time = 1000.0
  l_tol = 1e-10 #difference between first and last linear step
  nl_rel_step_tol = 1e-14 #machine percision
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Postprocessors]
  [./error]
    type = NodalL2Error
    variable = pressure
    function = analytical_function
  [../]
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
[]
