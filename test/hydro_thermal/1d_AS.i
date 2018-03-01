[Mesh]
  type = GeneratedMesh
  dim = 1
  xmin = 0
  xmax = 100
  nx = 20
[]

[UserObjects]
  [./water_uo]
    type = TigerFluidConst
    density = 1
    specific_heat = 1
    conductivity = 0.01
  [../]
  [./matrix_uo1]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '1.0e-8'
  [../]
[]

[Materials]
  [./matrix_h]
    type = TigerRockMaterialH
    fp_UO = water_uo
    kf_UO = matrix_uo1
    porosity = 1.0
    compressibility = 1.0e-10
  [../]
  [./matrix_t]
    type = TigerRockMaterialT
    fp_UO = water_uo
    porosity = 1.0
    conductivity_type = isotropic
    mean_calculation_type = arithmetic
    lambda = 0.01
    density = 1
    specific_heat = 1
  [../]
  [./advect_th]
    type = TigerAdvectionMaterialTH
    pore_pressure =  pressure
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
    type = TigerAdvectionKernelT
    variable = temperature
  [../]
  [./T_diff]
    type = TigerDiffusionKernelT
    variable = temperature
  [../]
  #[./T_body]
  #  type = TigerHeatSourceT
  #  variable = temperature
  #  value = 1
  #[../]
[]

#[Preconditioning]
#  [./precond]
#    type = SMP
#    full = true
#    petsc_options_iname = '-pc_type -pc_hypre_type'
#    petsc_options_value = 'hypre boomeramg'
#    #petsc_options_iname = '-ksp_type -snes_type -pc_type -pc_factor_shift_type -pc_factor_shift_amount -snes_atol -snes_rtol -snes_max_it'
#    #petsc_options_value = '  gmres     newtontr     asm          NONZERO               1E-12               1E-10       1E-15       250     '
#    #petsc_options = '-snes_ksp_ew'
#    #petsc_options_iname = '-ksp_type -pc_type -snes_atol -snes_rtol -snes_max_it -ksp_max_it -sub_pc_type -sub_pc_factor_shift_type'
#    #petsc_options_value = 'gmres asm 1E-10 1E-10 200 500 lu NONZERO'
#  [../]
#[]

[Executioner]
  type = Steady
  solve_type = NEWTON
  petsc_options = '-snes_ksp_ew'
  petsc_options_iname = '-ksp_type -pc_type -snes_atol -snes_rtol -snes_max_it -ksp_max_it -sub_pc_type -sub_pc_factor_shift_type'
  petsc_options_value = 'gmres asm 1E-10 1E-10 200 500 lu NONZERO'
[]

[Outputs]
  exodus = true
  print_linear_residuals = true
[]
