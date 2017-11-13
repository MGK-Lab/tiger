[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 1
  ny = 1
  nz = 20
  zmax = 0
  zmin = -1.0
[]

[UserObjects]
  [./water_uo]
    type = TigerFluidConst
  [../]
  [./rock_uo]
    type =  TigerPermeabilityRockConst
  [../]
[]

[Materials]
  [./rock_h]
    type = TigerRockMaterialH
    pressure = 1.0e6
    temperature = 100.0
    fp_UO = water_uo
    #has_gravity = true
    #gravity_acceleration = 10.0
    porosity = 0.4
    compressibility = 1.0e-9
    permeability_type = isotropic
    k0 = '1.0e-10'
    kf_UO = rock_uo
  [../]
  [./rock_t]
    type = TigerRockMaterialT
    pressure = 1.0e6
    temperature = 100.0
    fp_UO = water_uo
    porosity = 0.4
    conductivity_type = isotropic
    lambda = 2
    density = 2600
    specific_heat = 840
  [../]
[]

[BCs]
  [./front_p]
    type =  DirichletBC
    variable = pressure
    boundary = 'front'
    value = 0.0
  [../]
  [./back_p]
    type =  DirichletBC
    variable = pressure
    boundary = 'back'
    value = 50.0
  [../]
  [./front_t]
    type =  DirichletBC
    variable = temperature
    boundary = 'front'
    value = 0.0
  [../]
  [./back_t]
    type =  DirichletBC
    variable = temperature
    boundary = 'back'
    value = 100.0
  [../]
[]

[Variables]
  [./temperature]
    initial_condition = 50.0
  [../]
  [./pressure]
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
  [./vz]
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
    execute_on = timestep_end
  [../]
  [./vy_ker]
    type = TigerDarcyVelocityComponent
    gradient_variable = pressure
    variable =  vy
    component = y
    execute_on = timestep_end
  [../]
  [./vz_ker]
    type = TigerDarcyVelocityComponent
    gradient_variable = pressure
    variable =  vz
    component = z
    execute_on = timestep_end
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
  [./T_dt]
    type = TigerTimeDerivativeT
    variable = temperature
  [../]
  [./H_diff]
    type = TigerKernelH
    variable = pressure
  [../]
  [./H_dt]
    type = TigerTimeDerivativeH
    variable = pressure
  [../]
[]

[Executioner]
  type = Transient
  #dt = 0.1
  num_steps = 200
  end_time = 4.0e5
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
[]
