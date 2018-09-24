[Mesh]
  type = GeneratedMesh
  dim = 1
  xmin = 0
  xmax = 100
  nx = 10
[]

[Modules]
  [./FluidProperties]
    [./water_uo]
      type = TigerWaterConst
      density = 1
      cp = 1
      thermal_conductivity = 0.01
    [../]
  [../]
[]

[UserObjects]
  [./matrix_uo1]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '1.0e-8'
  [../]
[]

[Materials]
  [./rock_g]
    type = TigerGeometryMaterial
    porosity = 1
  [../]
  [./matrix_h]
    type = TigerRockMaterialH
    fp_UO = water_uo
    kf_UO = matrix_uo1
    compressibility = 1.0e-10
  [../]
  [./matrix_t]
    type = TigerCoupledThermalMaterialTH
    fp_UO = water_uo
    pressure =  pressure
    conductivity_type = isotropic
    mean_calculation_type = arithmetic
    lambda = 0.01
    density = 1
    specific_heat = 1
    has_supg = true
    supg_eff_length = min
    supg_coeficient = optimal
  [../]
[]

[BCs]
  [./left_h]
    type = DirichletBC
    variable = pressure
    boundary = left
    value = 1e6
  [../]
  [./right_h]
    type = DirichletBC
    variable = pressure
    boundary = right
    value = 0
  [../]
  [./left_t]
    type = DirichletBC
    variable = temperature
    boundary = left
    value = 0
  [../]
  [./right_t]
    type = DirichletBC
    variable = temperature
    boundary = right
    value = 1
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

[Functions]
  [./analytical_function]
    type = ParsedFunction
    vars = 'l T v lambda'
    vals = '100 1 0.1 0.01'
    value = 'T*exp((x-l)*v/(2*lambda))*sinh(x*sqrt(v*v/(4*lambda*lambda)))/sinh(l*sqrt(v*v/(4*lambda*lambda)))'
  [../]
[]

[Variables]
  [./pressure]
  [../]
  [./temperature]
  [../]
[]

[Kernels]
  [./H_diff]
    type = TigerKernelH
    variable = pressure
  [../]
  [./T_advect]
    type = TigerAdvectionKernelTH
    variable = temperature
    pressure_varible = pressure
  [../]
  [./T_diff]
    type = TigerDiffusionKernelT
    variable = temperature
  [../]
[]

[Executioner]
  type = Steady
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
  print_linear_residuals = true
[]
