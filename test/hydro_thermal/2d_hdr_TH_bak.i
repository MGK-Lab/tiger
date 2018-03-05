[Mesh]
  type = FileMesh
  file = ex_hdr.msh
[]

[MeshModifiers]
  [./modify]
     type = BlockDeleter
     block_id = 2
  [../]
[]

[UserObjects]
  [./water_uo]
    type = TigerFluidConst
    density = 1000
    viscosity = 1.8e-4
    specific_heat = 4200
    conductivity = 0.65
    compressibility = 4.0e-10
  [../]
  [./matrix_uo1]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '1.0e-10'
  [../]
  [./fracture_uo1]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '8.3333e-10'
  [../]
[]

[Materials]
  [./matrix_h]
    type = TigerRockMaterialH
    fp_UO = water_uo
    kf_UO = matrix_uo1
    porosity = 0.01
    compressibility = 1.0e-10
    block = 'matrix'
  [../]
  #[./fracure_h]
  #  type = TigerRockMaterialH
  #  fp_UO = water_uo
  #  kf_UO = fracture_uo1
  #  porosity = 1.0
  #  compressibility = 4.0e-10
  #  block = 'frac'
  #[../]
  [./matrix_t]
    type = TigerRockMaterialT
    fp_UO = water_uo
    porosity = 0.01
    conductivity_type = isotropic
    mean_calculation_type = arithmetic
    lambda = 3
    density = 2600
    specific_heat = 950
    block = 'matrix'
  [../]
  #[./fracture_t]
  #  type = TigerRockMaterialT
  #  fp_UO = water_uo
  #  porosity = 1.0
  #  conductivity_type = isotropic
  #  mean_calculation_type = arithmetic
  #  lambda = 3
  #  density = 2600
  #  specific_heat = 950
  #  block = 'frac'
  #[../]
[]

[BCs]
  [./whole_h]
    type = DirichletBC
    variable = pressure
    boundary = circum
    value = 1e7
  [../]
  [./whole_t]
    type =  DirichletBC
    variable = temperature
    boundary = circum
    value = 200
  [../]
  [./well_t]
    type =  DirichletBC
    variable = temperature
    boundary = inject
    value = 70
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
  [./lmin]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./lmax]
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
  [./vy_ker]
    type = TigerDarcyVelocityComponent
    gradient_variable = pressure
    variable =  vy
    component = y
  [../]
  [./lmin_ker]
    type = ElementLengthAux
    method = min
    variable = lmin
  [../]
  [./lmax_ker]
    type = ElementLengthAux
    method = max
    variable = lmax
  [../]
[]

[Variables]
  [./pressure]
    initial_condition = 1e7
  [../]
  [./temperature]
    initial_condition = 200
  [../]
[]

[DiracKernels]
  [./pump_in]
    type = TigerPointSourceH
    point = '175.0 250.0 0.0'
    mass_flux = 0.01
    variable = pressure
  [../]
  [./pump_out]
    type = TigerPointSourceH
    point = '325.0 250.0 0.0'
    mass_flux = -0.01
    variable = pressure
  [../]
[]

[Kernels]
  [./H_diff]
    type = TigerKernelH
    variable = pressure
  [../]
  [./H_Time]
    type = TigerTimeDerivativeH
    variable = pressure
  [../]
  [./T_advect]
    type = TigerAdvectionKernelT
    variable = temperature
    gradient_variable = pressure
  [../]
  [./T_diff]
    type = TigerDiffusionKernelT
    variable = temperature
  [../]
  [./T_time]
    type = TigerTimeDerivativeT
    variable = temperature
  [../]
[]

[Preconditioning]
  [./precond]
    type = SMP
    full = true
    #petsc_options_iname = '-pc_type -pc_hypre_type'
    #petsc_options_value = 'hypre boomeramg'
    petsc_options_iname = '-ksp_type -snes_type -pc_type -pc_factor_shift_type -pc_factor_shift_amount -snes_atol -snes_rtol -snes_max_it'
    petsc_options_value = '  gmres     newtontr     asm          NONZERO               1E-12               1E-10       1E-15       250     '
    #petsc_options = '-snes_ksp_ew'
    #petsc_options_iname = '-ksp_type -pc_type -snes_atol -snes_rtol -snes_max_it -ksp_max_it -sub_pc_type -sub_pc_factor_shift_type'
    #petsc_options_value = 'gmres asm 1E-10 1E-10 200 500 lu NONZERO'
  [../]
[]

[Executioner]
  type = Transient
  num_steps = 100
  end_time = 1e9
  solve_type = NEWTON
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
[]
