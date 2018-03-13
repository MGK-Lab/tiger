[Mesh]
  type = FileMesh
  file = elder.msh
  uniform_refine = 2
[]

#[Adaptivity]
#  max_h_level = 2
#  cycles_per_step = 1
#  marker = error_frac
#  [./Markers]
#    [./error_frac]
#      indicator = ind
#      type = ErrorFractionMarker
#      refine = 0.05
#    [../]
#  [../]
#  [./Indicators]
#    [./ind]
#      type = GradientJumpIndicator
#      variable = temperature
#    [../]
#  [../]
#[]

[UserObjects]
  [./water_uo]
    type = TigerWaterElder
  [../]
  [./rock_uo]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = 4.935e-13
  [../]
[]

[Materials]
  [./rock_h]
    type = TigerRockMaterialH
    temperature = temperature
    fp_UO = water_uo
    kf_UO = rock_uo
    has_gravity = true
    gravity_acceleration = 9.81
    porosity = 1.0
    compressibility = 0
  [../]
  [./rock_t]
    type = TigerRockMaterialT
    temperature = temperature
    fp_UO = water_uo
    mean_calculation_type = arithmetic
    conductivity_type = isotropic
    lambda = 0
    porosity = 0.1
    density = 0
    specific_heat = 0
  [../]
[./matrix_th]
    type = TigerAdvectionMaterialTH
    pressure = pore_pressure
    temperature = temperature
    fp_UO = water_uo
    has_supg = true
    is_supg_consistent = true
    supg_eff_length = max
    supg_coeficient = transient_brooks
    output_Pe_Cr_numbers = true
  [../]
[]

[BCs]
  [./tp_p]
    type = DirichletBC
    variable = pore_pressure
    boundary = 'tp2 tp3'
    value = 0
  [../]
  [./top_t]
    type = DirichletBC
    variable = temperature
    boundary = t2
    value = 1
  [../]
  [./bottom_t]
    type = DirichletBC
    variable = temperature
    boundary = bottom
    value = 0
  [../]
[]

[Functions]
  [./pore]
    type = ParsedFunction
    value = '1000.0*9.81*abs(y)'
  [../]
[]

[ICs]
  [./pore]
    type = FunctionIC
    function = pore
    variable = pore_pressure
  [../]
[]

[Variables]
  [./temperature]
    initial_condition = 0.0
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
  [./pe]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./cr]
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
  [../]
  [./vy_ker]
    type = TigerDarcyVelocityComponent
    gradient_variable = pore_pressure
    variable =  vy
    component = y
  [../]
  [./pe_ker]
    type = MaterialRealAux
    property = 'peclet_number'
    variable = pe
  [../]
  [./cr_ker]
    type = MaterialRealAux
    property = 'courant_number'
    variable = cr
  [../]
  [./density_ker]
    type = MaterialRealAux
    variable =  density
    property = fluid_density
  [../]
[]

[Kernels]
  [./H_diff]
    type = TigerKernelH
    variable = pore_pressure
  [../]
  [./H_time]
    type = TigerTimeDerivativeH
    variable = pore_pressure
  [../]
  [./T_diff]
    type = TigerDiffusionKernelT
    variable = temperature
  [../]
  [./T_advect]
    type = TigerAdvectionKernelTH
    variable = temperature
  [../]
  [./T_dt]
    type = TigerTimeDerivativeT
    variable = temperature
  [../]
[]

[Executioner]
  type = Transient
  num_steps = 2000
  end_time = 6.28992e8
  solve_type = NEWTON
  petsc_options = '-snes_ksp_ew'
  petsc_options_iname = '-ksp_type -pc_type -snes_atol -snes_rtol -snes_max_it -ksp_max_it -sub_pc_factor_shift_type'
  petsc_options_value = 'gmres lu 1E-10 1E-15 200 500 NONZERO'
  #[./TimeIntegrator]
  #  type = BDF2
  #[../]
[]

[Outputs]
  print_linear_residuals = false
  [./maz]
    type = Exodus
    file_base = elder/elder
    interval = 10
  [../]
[]
