[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 40
  xmax = 10
  xmin = 0
[]

[UserObjects]
  [./water_uo]
    type = TigerFluidConst
    specific_heat = 840
    density = 2600
  [../]
  [./rock_uo]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '1.0e-10'
  [../]
[]

[Materials]
  [./rock_h]
    type = TigerRockMaterialH
    fp_UO = water_uo
    porosity = 0.0
    compressibility = 1.0e-9
    kf_UO = rock_uo
  [../]
  [./rock_t]
    type = TigerRockMaterialT
    fp_UO = water_uo
    porosity = 0.0
    conductivity_type = isotropic
    mean_calculation_type = arithmetic
    lambda = 2.184
    density = 2600
    specific_heat = 840
  [../]
[]

[BCs]
  [./left_p]
    type =  DirichletBC
    variable = pressure
    boundary = left
    value = 0
  [../]
  [./right_p]
    type =  DirichletBC
    variable = pressure
    boundary = right
    value = 100.0
  [../]
  [./left_t]
    type =  DirichletBC
    variable = temperature
    boundary = left
    value = 0
  [../]
  [./right_t]
    type =  DirichletBC
    variable = temperature
    boundary = right
    value = 100.0
  [../]
[]

[Variables]
  [./temperature]
  [../]
  [./pressure]
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

[Functions]
  [./analytical_function]
    type = ParsedFunction
    vars = 'l T'
    vals = '10 100'
    value = 'T*exp((l-x)/2)*sinh(x/2)/sinh(l/2)'
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

[Kernels]
  [./T_diff]
    type = TigerDiffusionKernelT
    variable = temperature
  [../]
  [./T_advect]
    type = TigerAdvectionKernelT
    variable = temperature
    gradient_variable = pressure
  [../]
  [./H_diff]
    type = TigerKernelH
    variable = pressure
  [../]
[]

[Executioner]
  type = Steady
  l_tol = 1e-10 #difference between first and last linear steps
  nl_rel_step_tol = 1e-15 #machine percision
  nl_rel_tol = 1e-10 #difference between first and last nonlinear steps
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
[]
