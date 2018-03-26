[Mesh]
  type = FileMesh
  file = 3d_egs.e
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

#[Adaptivity]
#  max_h_level = 2
#  cycles_per_step = 1
#  marker = error_frac
#  [./Markers]
#    [./error_frac]
#      indicator = ind
#      type = ErrorFractionMarker
#      refine = 0.01
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
    k0 = '1.0e-16'
  [../]
  [./ub_uo]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '1.0e-15'
  [../]
  [./fi_uo]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '1.0e-12'
  [../]
  [./fv_uo]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '1.0e-11'
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
    porosity = 0.05
    compressibility = 1.0e-10
    block = unit_top
  [../]
  [./ub_h]
    type = TigerRockMaterialH
    kf_UO = ub_uo
    porosity = 0.1
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
    type = TigerRockMaterialT
    porosity = 0.05
    conductivity_type = isotropic
    mean_calculation_type = geometric
    lambda = 3
    density = 2600
    specific_heat = 950
    block = unit_top
  [../]
  [./ub_t]
    type = TigerRockMaterialT
    porosity = 0.1
    conductivity_type = isotropic
    mean_calculation_type = geometric
    lambda = 2
    density = 2600
    specific_heat = 950
    block = unit_bottom
  [../]
  [./f_t]
    type = TigerRockMaterialT
    porosity = 1.0
    conductivity_type = isotropic
    mean_calculation_type = geometric
    lambda = 3
    scaling_factor = 0.01
    density = 2600
    specific_heat = 950
    block = 'frac_vertical frac_inclined'
  [../]
  [./w_t]
    type = TigerRockMaterialT
    porosity = 1.0
    conductivity_type = isotropic
    mean_calculation_type = geometric
    lambda = 3
    scaling_factor = 0.108
    density = 2600
    specific_heat = 950
    block = 'well_vertical well_inclined'
  [../]
  [./u_th]
    type = TigerAdvectionMaterialTH
    pressure = pressure
    has_supg = true
    is_supg_consistent = true
    supg_eff_length = average
    supg_coeficient = transient_brooks
    block = 'unit_top unit_bottom'
  [../]
  [./f_th]
    type = TigerAdvectionMaterialTH
    pressure = pressure
    has_supg = true
    is_supg_consistent = true
    supg_eff_length = average
    supg_coeficient = transient_brooks
    supg_coeficient_scale = 1
    scaling_factor = 0.01
    block = 'frac_vertical frac_inclined'
  [../]
  [./w_th]
    type = TigerAdvectionMaterialTH
    pressure = pressure
    has_supg = true
    is_supg_consistent = true
    supg_eff_length = min
    supg_coeficient = transient_brooks
    scaling_factor = 0.108
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
    value = 'if(x<-299 & x>-301 & y<101 & y>99 & z<-1899 & z>-1901, 70, 200)'
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
  [../]
  [./temperature]
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
  [./fieldsplit]
    type = FSP
    full = true
    topsplit = pT
    [./pT]
      splitting = 'p T'
      splitting_type = multiplicative
      petsc_options_iname = '-ksp_type -pc_type -ksp_rtol -ksp_max_it -snes_type -snes_linesearch_type -snes_atol -snes_rtol -snes_max_it'
      petsc_options_value = 'fgmres lu 1.0e-12 25 newtonls basic 1.0e-1 1.0e-20 20'
    [../]
    [./p]
      vars = 'pressure'
      petsc_options_iname = '-ksp_type -pc_type -sub_pc_type -sub_pc_factor_levels -ksp_rtol -ksp_max_it'
      petsc_options_value = 'fgmres asm ilu 1 1e-15 25'
    [../]
    [./T]
      vars = 'temperature'
      petsc_options_iname = '-ksp_type -pc_type -pc_hypre_type -ksp_rtol -ksp_max_it'
      petsc_options_value = 'preonly hypre boomeramg 1e-15 25'
    [../]
  [../]
[]

[Executioner]
  type = Transient
  end_time = 946080000
  #dtmax = 2.59e6
  dt = 2.59e6
  solve_type = NEWTON
  #[./TimeStepper]
  #  type = SolutionTimeAdaptiveDT
  #   dt = 60
  #  percent_change = 0.5
  #[../]
  [./TimeIntegrator]
    type = CrankNicolson
  [../]
[]

[Outputs]
  [./maz]
    type = Exodus
    file_base = 3d_egs/3d_egs
  [../]
  print_linear_residuals = true
[]
