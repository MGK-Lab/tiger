[Mesh]
  type = GeneratedMesh
  dim = 2
  xmin = 0
  xmax = 300
  ymin = 0
  ymax = 150
  nx = 30
  ny = 15
[]

# [Adaptivity]
#   max_h_level = 2
#   initial_marker = error_frac
#   marker = error_frac
#   cycles_per_step = 2
#   [./Markers]
#     [./error_frac]
#       indicator = ind
#       type = ErrorFractionMarker
#       refine = 0.8
#       coarsen = 0.01
#     [../]
#   [../]
#   [./Indicators]
#     [./ind]
#       type = GradientJumpIndicator
#       variable = temperature
#     [../]
#   [../]
# []

[Modules]
  [./FluidProperties]
    [./water_uo]
      type = TigerIdealWater
      reference_pressure = 0
      viscosity = 0.001
    [../]
  [../]
[]

[UserObjects]
  [./rock_uo]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = 4.9346e-13
  [../]
  [./supg]
    type = TigerSUPG
    effective_length = max
    supg_coeficient = critical
  [../]
[]

[Materials]
  [./rock_g]
    type = TigerGeometryMaterial
    porosity = 0.1
  [../]
  [./rock_f]
    type = TigerFluidMaterial
    fp_uo = water_uo
    temperature = temperature
  [../]
  [./rock_h]
    type = TigerHydraulicMaterialH
    pressure = pressure
    kf_uo = rock_uo
    gravity_acceleration = 9.81
    has_gravity = true
    compressibility = 1.0e-10
  [../]
  [./matrix_t]
    type = TigerThermalMaterialT
    conductivity_type = isotropic
    mean_calculation_type = arithmetic
    lambda = 1e-4
    density = 1
    specific_heat = 1
    has_supg = true
    supg_uo = supg
  [../]
  [./rock_th]
    type = TigerCoupledThermalMaterialTH
    pressure = pore_pressure
    temperature = temperature
    fp_UO = water_uo
    conductivity_type = isotropic
    lambda = 0
    porosity = 0.1
    density = 0
    specific_heat = 0
    has_supg = true
    supg_eff_length = max
    supg_coeficient = critical
    output_Pe_Cr_numbers = true
  [../]
[]

[BCs]
  [./tp_p]
    type = DirichletBC
    variable = pore_pressure
    boundary = tp
    value = 0
  [../]
  [./top_t]
    type = DirichletBC
    variable = temperature
    boundary = t2
    value =  1
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

[Kernels]
  [./T_diff]
    type = TigerThermalDiffusionKernelT
    variable = temperature
  [../]
  [./T_advect]
    type = TigerThermalAdvectionKernelT
    variable = temperature
    pressure = pressure
  [../]
  [./T_dt]
    type = TigerThermalTimeKernelT
    variable = temperature
  [../]
  [./H_diff]
    type = TigerHydraulicKernelH
    variable = pressure
  [../]
[]

[Preconditioning]
  active = 'p1'
  [./p1]
    type = SMP
    full = true
    petsc_options_iname = '-pc_type -pc_hypre_type'
    petsc_options_value = 'hypre boomeramg'
  [../]
  [./p2]
    type = SMP
    full = true
    petsc_options_iname = '-pc_type -sub_pc_type -sub_pc_factor_shift_type'
    petsc_options_value = 'asm lu NONZERO'
  [../]
  [./p3]
    type = SMP
    full = true
    petsc_options_iname = '-pc_type -ksp_type -sub_pc_type -pc_asm_overlap -sub_pc_factor_shift_type'
    petsc_options_value = 'asm gmres lu 2 NONZERO'
  [../]
[]

[Executioner]
  type = Transient
  num_steps = 2000
  end_time = 6.28992e8
  solve_type = NEWTON
  l_tol = 1e-8
  l_max_its = 25
  nl_rel_tol = 1e-09
  nl_abs_tol = 1e-14
  nl_max_its = 50
  scheme = implicit-euler
[]

[Outputs]
  print_linear_residuals = false
  [./maz]
    type = Exodus
    file_base = elder/elder_half
  [../]
[]
