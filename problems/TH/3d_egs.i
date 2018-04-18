[Mesh]
  type = FileMesh
  file = 3d_egs_mesh.e
  boundary_id = '1 2 3 4 5 6'
  boundary_name = 'back front left right bottom top'
  block_id = '0 1 2 3 4 5'
  block_name = 'unit_top unit_bottom frac_inclined frac_vertical well_vertical well_inclined'
[]

[MeshModifiers]
  [./inject]
    type = AddExtraNodeset
    new_boundary = inject
    coord = '-300.0 100.0 -1900.0'
    tolerance = 1
  [../]
[]

# [Adaptivity]
#  max_h_level = 2
#  initial_marker = error_frac
#  marker = error_frac
#  cycles_per_step = 2
#  [./Markers]
#    [./error_frac]
#      indicator = ind
#      type = ErrorFractionMarker
#      refine = 0.7
#      coarsen = 0.1
#    [../]
#  [../]
#  [./Indicators]
#    [./ind]
#      type = GradientJumpIndicator
#      variable = temperature
#    [../]
#  [../]
# []

[UserObjects]
  [./water_uo]
    type = TigerFluidConst
    density = 1000
    viscosity = 2e-4
    specific_heat = 4200
    conductivity = 0.65
    compressibility = 4.0e-10
  [../]
  [./ut_uo]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '1.0e-15'
  [../]
  [./ub_uo]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '1.0e-16'
  [../]
  [./fi_uo]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '1.0e-11'
  [../]
  [./fv_uo]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '1.0e-10'
  [../]
  [./w_uo]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '1.0e-5'
  [../]
[]

[GlobalParams]
  fp_UO = water_uo
  has_gravity = true
  output_Pe_Cr_numbers = true
[]

[Materials]
  [./ut_h]
    type = TigerRockMaterialH
    kf_UO = ut_uo
    porosity = 0.1
    compressibility = 1.0e-10
    block = unit_top
  [../]
  [./ub_h]
    type = TigerRockMaterialH
    kf_UO = ub_uo
    porosity = 0.05
    compressibility = 1.0e-10
    block = unit_bottom
  [../]
  [./fv_h]
    type = TigerRockMaterialH
    scaling_factor = 0.01
    kf_UO = fv_uo
    porosity = 1
    compressibility = 4.0e-10
    block = frac_vertical
  [../]
  [./fi_h]
    type = TigerRockMaterialH
    scaling_factor = 0.01
    kf_UO = fi_uo
    porosity = 1
    compressibility = 4.0e-10
    block = frac_inclined
  [../]
  [./w_h]
    type = TigerRockMaterialH
    scaling_factor = 0.108
    kf_UO = w_uo
    porosity = 1
    compressibility = 4.0e-10
    block = 'well_vertical well_inclined'
  [../]
  [./ut_t]
    type = TigerCoupledThermalMaterialTH
    porosity = 0.1
    conductivity_type = isotropic
    mean_calculation_type = geometric
    lambda = 2
    density = 2600
    specific_heat = 950
    pressure = pressure
    has_supg = true
    supg_eff_length = directional_average
    supg_coeficient = transient_tezduyar
    block = unit_top
  [../]
  [./ub_t]
    type = TigerCoupledThermalMaterialTH
    porosity = 0.05
    conductivity_type = isotropic
    mean_calculation_type = geometric
    lambda = 3
    density = 2600
    specific_heat = 950
    pressure = pressure
    has_supg = true
    supg_eff_length = directional_average
    supg_coeficient = transient_tezduyar
    block = unit_bottom
  [../]
  [./f_t]
    type = TigerCoupledThermalMaterialTH
    porosity = 1.0
    conductivity_type = isotropic
    mean_calculation_type = geometric
    lambda = 3
    scaling_factor = 0.01
    density = 2600
    specific_heat = 950
    pressure = pressure
    has_supg = true
    supg_eff_length = average
    supg_coeficient = transient_tezduyar
    block = 'frac_vertical frac_inclined'
  [../]
  [./w_t]
    type = TigerCoupledThermalMaterialTH
    porosity = 1.0
    conductivity_type = isotropic
    mean_calculation_type = geometric
    lambda = 3
    scaling_factor = 0.108
    density = 2600
    specific_heat = 950
    pressure = pressure
    has_supg = true
    supg_eff_length = min
    supg_coeficient = transient_tezduyar
    block = 'well_vertical well_inclined'
  [../]
[]

[BCs]
  [./top_h]
    type = DirichletBC
    variable = pressure
    boundary = top
    value = 1.6677e7
  [../]
  [./whole_t]
    type =  DirichletBC
    variable = temperature
    boundary = 'back front left right bottom top'
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
  [./vz]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./pe]
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
  [./vz_ker]
    type = TigerDarcyVelocityComponent
    gradient_variable = pressure
    variable =  vz
    component = z
  [../]
  [./pe_ker]
    type = MaterialRealAux
    property = 'peclet_number'
    variable = pe
  [../]
[]

[Functions]
  [./hydrostatic]
    type = ParsedFunction
    value = '-1000*9.81*z'
  [../]
  [./temp]
    type = ParsedFunction
    value = 'if(x<-299.9 & x>-300.1 & y<100.1 & y>99.9 & z<-1899.9 & z>-1900.1, 70, 200)'
  [../]
[]

[ICs]
  [./hydrostatic_ic]
    type = FunctionIC
    variable = pressure
    function = hydrostatic
  [../]
  [./temp_ic]
    type = FunctionIC
    variable = temperature
    function = temp
  [../]
[]

[Variables]
  [./pressure]
    scaling = 1e7
  [../]
  [./temperature]
    #initial_condition = 200
  [../]
[]

[DiracKernels]
  [./pump_in]
    type = TigerPointSourceH
    point = '-300.0 100.0 -1900.0'
    mass_flux = 50.0
    variable = pressure
  [../]
  [./pump_out]
    type = TigerPointSourceH
    point = '300.0 200.0 -1900.0'
    mass_flux = -50.0
    variable = pressure
  [../]
[]

[Kernels]
  [./H_diff]
    type = TigerKernelH
    variable = pressure
  [../]
  [./H_time]
    type = TigerTimeDerivativeH
    variable = pressure
  [../]
  [./TH_advect]
    type = TigerAdvectionKernelTH
    variable = temperature
    pressure_varible = pressure
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
  active = 'p4'
  [./p1]
    type = SMP
    full = true
    #petsc_options = '-snes_ksp_ew'
    petsc_options_iname = '-pc_type -pc_hypre_type -snes_type -snes_linesearch_type -sub_pc_factor_shift_type'
    petsc_options_value = 'hypre boomeramg newtonls basic NONZERO'
  [../]
  [./p2]
    type = SMP
    full = true
    #petsc_options = '-snes_ksp_ew'
    petsc_options_iname = '-pc_type -sub_pc_type -snes_type -snes_linesearch_type -sub_pc_factor_shift_type -ksp_gmres_restart'
    petsc_options_value = 'asm lu newtonls basic NONZERO 51'
  [../]
  [./p3]
    type = SMP
    full = true
    #petsc_options = '-snes_ksp_ew'
    petsc_options_iname = '-pc_type -ksp_type -sub_pc_type -snes_type -snes_linesearch_type -pc_asm_overlap -sub_pc_factor_shift_type -ksp_gmres_restart'
    petsc_options_value = 'asm gmres lu newtonls basic 2 NONZERO 51'
  [../]
  [./p4]
    type = FSP
    full = true
    topsplit = pT
    [./pT]
      splitting = 'p T'
      splitting_type = multiplicative
      petsc_options_iname = '-ksp_type -pc_type -snes_type -snes_linesearch_type'
      petsc_options_value = 'fgmres lu newtonls basic'
    [../]
    [./p]
      vars = 'pressure'
      petsc_options_iname = '-ksp_type -pc_type -sub_pc_type'
      petsc_options_value = 'fgmres asm ilu'
    [../]
    [./T]
      vars = 'temperature'
      petsc_options_iname = '-ksp_type -pc_type -pc_hypre_type'
      petsc_options_value = 'preonly hypre boomeramg'
    [../]
  [../]
[]

[Executioner]
  type = Transient
  end_time = 946080000
  dtmax = 31449600
  #dt = 2.59e5
  solve_type = NEWTON
  l_tol = 1e-10
  l_max_its = 50
  nl_rel_tol = 1e-7
  #nl_abs_tol = 1
  nl_max_its = 50
  #scheme = crank-nicolson
  [./TimeStepper]
    type = SolutionTimeAdaptiveDT
    dt = 600
    percent_change = 0.5
  [../]
[]

[Outputs]
  [./maz]
    type = Exodus
    file_base = 3d_egs/3d_egs
  [../]
  print_linear_residuals = true

[]
