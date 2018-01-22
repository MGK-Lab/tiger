[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 60
  ny = 15
  xmin = -30
  xmax = 30
  ymin = -15
  ymax = 0.0
[]

[UserObjects]
  [./water_uo]
    type = TigerWaterTP
  [../]
  [./rock_uo]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = 1.0e-9
  [../]
[]

[Materials]
  [./rock_h]
    type = TigerRockMaterialH
    pressure = pore_pressure
    temperature = temperature
    fp_UO = water_uo
    has_gravity = true
    gravity_acceleration = 10.0
    porosity = 0.1
    compressibility = 1.0e-9
    kf_UO = rock_uo
    material_type = matrix
  [../]
  [./rock_t]
    type = TigerRockMaterialT
    pressure = pore_pressure
    temperature = temperature
    fp_UO = water_uo
    porosity = 0.1
    conductivity_type = isotropic
    lambda = 20
    density = 2600
    specific_heat = 840
    material_type = matrix
    mean_calculation_type = arithmetic
  [../]
[]

[BCs]
  [./top_p]
    type =  DirichletBC
    variable = pore_pressure
    boundary = top
    value = 0.0
  [../]
  #[./bott_p]
  #  type =  DirichletBC
  #  variable = pore_pressure
  #  boundary = bottom
  #  value = 150000.0
  #[../]
  [./top_t]
    type =  DirichletBC
    variable = temperature
    boundary = top
    value = 0.0
  [../]
  #[./bott_t]
  #  type =  DirichletBC
  #  variable = temperature
  #  boundary = bottom
  #  value = 100.0
  #[../]
  [./bott_t]
    type =  FunctionDirichletBC
    variable = temperature
    boundary = bottom
    function = temp
  [../]
[]

[Functions]
  [./temp]
    type = ParsedFunction
    value = 'if(t>2000.0 & x<15 & x>-15,50.0,0.0)'
  [../]
[]

[Variables]
  [./temperature]
  [../]
  [./pore_pressure]
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
  [./density]
    family = MONOMIAL
    order = CONSTANT
  [../]
[]

[AuxKernels]
  [./vx_ker]
    type = TigerDarcyVelocityComponent
    gradient_variable = pore_pressure
    variable =  vx
    component = x
    execute_on = timestep_end
  [../]
  [./vy_ker]
    type = TigerDarcyVelocityComponent
    gradient_variable = pore_pressure
    variable =  vy
    component = y
    execute_on = timestep_end
  [../]
  [./density_ker]
    type = MaterialRealAux
    variable =  density
    property = fluid_density
  [../]
[]

[Kernels]
  [./T_diff]
    type = TigerDiffusionKernelT
    variable = temperature
  [../]
  #[./T_advect]
  #  type = TigerAdvectionKernelT
  #  variable = temperature
  #  gradient_variable = pore_pressure
  #[../]
  [./T_dt]
    type = TigerTimeDerivativeT
    variable = temperature
  [../]
  [./H_diff]
    type = TigerKernelH
    variable = pore_pressure
  [../]
  [./H_dt]
    type = TigerTimeDerivativeH
    variable = pore_pressure
  [../]
[]

[Executioner]
  type = Transient
  #dt = 0.1
  num_steps = 50
  end_time = 2.5e8
  l_tol = 1e-10 #difference between first and last linear steps
  nl_rel_step_tol = 1e-15 #machine percision
  nl_rel_tol = 1e-10 #difference between first and last nonlinear steps
  solve_type = 'PJFNK'
  #petsc_options = '-snes_ksp_ew'
  #petsc_options_iname = '-ksp_type -pc_type -snes_atol -snes_rtol -snes_max_it -ksp_max_it -sub_pc_factor_shift_type'
  #petsc_options_value = 'gmres lu 1E-10 1E-15 200 500 NONZERO'
  petsc_options = '-snes_ksp_ew'
  petsc_options_iname = '-ksp_type -pc_type -snes_atol -snes_rtol -snes_max_it -ksp_max_it -sub_pc_type -sub_pc_factor_shift_type'
  petsc_options_value = 'gmres asm 1E-10 1E-15 200 500 lu NONZERO'
[]

[Outputs]
  exodus = true
  execute_on = timestep_end
  print_linear_residuals = false
[]
