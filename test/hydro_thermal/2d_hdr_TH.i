[Mesh]
  type = FileMesh
  file = ex_hdr.msh
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
    k0 = '1.0e-17'
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
  [./matrix_t]
    type = TigerRockMaterialT
    fp_UO = water_uo
    porosity = 0.01
    conductivity_type = isotropic
    mean_calculation_type = geometric
    lambda = 3
    density = 2600
    specific_heat = 950
    block = 'matrix'
  [../]
  [./fracure_h]
    type = TigerRockMaterialH
    fp_UO = water_uo
    kf_UO = fracture_uo1
    porosity = 1.0
    compressibility = 4.0e-10
    block = 'frac'
  [../]
  [./fracture_t]
    type = TigerRockMaterialT
    fp_UO = water_uo
    porosity = 1.0
    conductivity_type = isotropic
    mean_calculation_type = geometric
    lambda = 3
    density = 2600
    specific_heat = 950
    block = 'frac'
  [../]
  [./advect_th]
    type = TigerAdvectionMaterialTH
    fp_UO = water_uo
    pressure = pressure
    has_supg = true
    is_supg_consistent = true
    supg_eff_length = max
    supg_coeficient = optimal
  [../]
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
  [./pe]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./cr]
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
[]

[Variables]
  [./pressure]
    initial_condition = 1e7
    scaling = 1e7
  [../]
  [./temperature]
    initial_condition = 200
    #scaling = 200
  [../]
[]

[DiracKernels]
  [./pump_in]
    type = TigerPointSourceH
    point = '175.0 250.0 0.0'
    mass_flux = 1.0
    variable = pressure
  [../]
  [./pump_out]
    type = TigerPointSourceH
    point = '325.0 250.0 0.0'
    mass_flux = -1.0
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
  [./T_advect]
    type = TigerAdvectionKernelTH
    variable = temperature
    #block = 'matrix'
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
  #[./precond]
  #  type = SMP
  #  full = true
  #  petsc_options_iname = '-pc_type -pc_hypre_type'
  #  petsc_options_value = 'hypre boomeramg'
  #  #petsc_options_iname = '-ksp_type -snes_type -pc_type -pc_factor_shift_type -pc_factor_shift_amount -snes_atol -snes_rtol -snes_max_it'
  #  #petsc_options_value = '  gmres     newtontr     asm          NONZERO               1E-12               1E-10       1E-15       250     '
  #  #petsc_options = '-snes_ksp_ew'
  #  #petsc_options_iname = '-ksp_type -pc_type -snes_atol -snes_rtol -snes_max_it -ksp_max_it -sub_pc_type -sub_pc_factor_shift_type'
  #  #petsc_options_value = 'gmres asm 1E-10 1E-10 200 500 lu NONZERO'
  #[../]
  [./fieldsplit]
    type = FSP
    topsplit = pT
    [./pT]
      splitting = 'p T'
      splitting_type = multiplicative
      petsc_options_iname = '-ksp_type -ksp_rtol -ksp_max_it -snes_type -snes_linesearch_type -snes_atol -snes_rtol -snes_max_it'
      petsc_options_value = 'fgmres 1.0e-12 50 newtonls basic 1.0e-04 1.0e-12 25'
    [../]
    [./p]
      vars = 'pressure'
      petsc_options_iname = '-ksp_type -pc_type -sub_pc_type -sub_pc_factor_levels -ksp_rtol -ksp_max_it'
      petsc_options_value = 'fgmres asm ilu 1 1e-12 20'
    [../]
    [./T]
      vars = 'temperature'
      petsc_options_iname = '-ksp_type -pc_type -pc_hypre_type -ksp_rtol -ksp_max_it'
      petsc_options_value = 'preonly hypre boomeramg 1e-12 20'
    [../]
  [../]
[]

[Executioner]
  type = Transient
  num_steps = 100
  end_time = 1.5e7
  solve_type = NEWTON
  #l_tol = 1e-10 #difference between first and last linear step
  #nl_rel_step_tol = 1e-14 #machine percision
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
[]
