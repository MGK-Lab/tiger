[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 1
  ny = 1
  nz = 100
  zmax = 0
  zmin = -1000
[]

[Modules]
  [./FluidProperties]
    [./water_uo]
      type = SimpleFluidProperties
    [../]
  [../]
[]

[Materials]
  [./rock_t]
    type = TigerUncoupledThermalMaterialTH
    fp_UO = water_uo
    porosity = 0.0
    conductivity_type = isotropic
    lambda = 3
    density = 0.1 #artificial value to reach steady in the transient simulation
    specific_heat = 0.1 #artificial value to reach steady in the transient simulation
  [../]
[]

[BCs]
  [./front]
    type =  DirichletBC
    variable = temperature
    boundary = 'front'
    value = 10.0
  [../]
  [./back_n]
    type =  NeumannBC
    variable = temperature
    boundary = 'back'
    value = 8.0e-2
  [../]
[]

[AuxVariables]
  [./analytical_solution]
    family = LAGRANGE
    order = FIRST
  [../]
[]

[Functions]
  [./analytical_function]
    type = ParsedFunction
    vars = 'q T D A l'
    vals = '8.0e-2 10 3 1e-6 1000'
    value = 'T - (q+A*l)/D*z - A*z*z/2/D'
  [../]
[]

[AuxKernels]
  [./a_ker]
    type = FunctionAux
    function = analytical_function
    variable = analytical_solution
    execute_on = initial
  [../]
[]

[Variables]
  [./temperature]
    initial_condition = 10.0
  [../]
[]

[Kernels]
  [./T_diff]
    type = TigerDiffusionKernelT
    variable = temperature
  [../]
  [./T_Source]
    type = TigerHeatSourceT
    variable = temperature
    value = 1.0e-6
  [../]
  [./T_dt]
    type = TigerTimeDerivativeT
    variable = temperature
  [../]
[]

[Executioner]
  type = Transient
  num_steps = 10
  end_time = 2.5e4
  #type = Steady
  l_tol = 1e-10 #difference between first and last linear step
  nl_rel_step_tol = 1e-10 #machine percision
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
   print_linear_residuals = false
[]
