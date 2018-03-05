[Mesh]
  type = GeneratedMesh
  dim = 1
  xmin = 0
  xmax = 1.4
  nx = 100
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
  [./advect_th]
    type = TigerAdvectionMaterialTH
    fp_UO = water_uo
    pressure = pressure
    has_supg = false
    is_supg_consistent = true
    supg_eff_length = min
    supg_coeficient = transient_tezduyar
  [../]
[]

[BCs]
  [./left_h]
    type = DirichletBC
    variable = pressure
    boundary = left
    value = 1e5
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

[Functions]
  [./initial]
    type = ParsedFunction
    value = 'if(x<0.4, 0.5*(1+cos(5*3.14*(x-0.2))),0.0)'
  [../]
[]

[Variables]
  [./pressure]
  [../]
  [./temperature]
  [../]
[]

[ICs]
  [./init_temp]
    type = FunctionIC
    variable = temperature
    function = initial
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
  [../]
  [./T_time]
    type =  TigerTimeDerivativeT
    variable = temperature
  [../]
[]

[Executioner]
  type = Transient
  end_time = 1
  num_steps = 10
  solve_type = NEWTON
  #[./TimeIntegrator]
  #   type = CrankNicolson
  #[../]
  petsc_options = '-snes_ksp_ew'
  petsc_options_iname = '-ksp_type -pc_type -snes_atol -snes_rtol -snes_max_it -ksp_max_it -sub_pc_type -sub_pc_factor_shift_type'
  petsc_options_value = 'gmres asm 1E-10 1E-10 200 500 lu NONZERO'
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
[]
