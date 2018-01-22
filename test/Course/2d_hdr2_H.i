[Mesh]
  #type = FileMesh
  #file = ex_hdr.msh
  type = GeneratedMesh
  dim = 2
  xmin = 0
  xmax = 500
  ymin = 0
  ymax = 500
  nx = 50
  ny = 50
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
    k0 = '1.0e-15'
  [../]
  [./fracture_uo1]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '8.3333e-12'
  [../]
[]

[Materials]
  [./matrix_h]
    type = TigerRockMaterialH
    fp_UO = water_uo
    kf_UO = matrix_uo1
    porosity = 0.1
    compressibility = 1.0e-10
    #block = 'unit'
  [../]
  #[./fracure_h]
  #  type = TigerRockMaterialH
  #  fp_UO = water_uo
  #  kf_UO = matrix_uo1
  #  porosity = 1.0
  #  compressibility = 4.0e-10
  #  block = 'frac1 frac2'
  #[../]
[]

[BCs]
  #[./whole_h]
  #  type = DirichletBC
  #  variable = pressure
  #  boundary = circum
  #  value = 1e7
  #[../]
  [./right_h]
    type = DirichletBC
    variable = pressure
    boundary = right
    value = 1e7
  [../]
  [./left_h]
    type = DirichletBC
    variable = pressure
    boundary = left
    value = 1e7
  [../]
  #[./top_h]
  #  type = DirichletBC
  #  variable = pressure
  #  boundary = top
  #  value = 1e7
  #[../]
  #[./bottom_h]
  #  type = DirichletBC
  #  variable = pressure
  #  boundary = bottom
  #  value = 1e7
  #[../]
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
[]

[Variables]
  [./pressure]
    #initial_condition = 1e7
  [../]
[]

#[DiracKernels]
#  [./pump_in]
#    type = TigerPointSourceH
#    point = '150.0 250.0 0.0'
#    mass_flux = 3.0
#    variable = pressure
#  [../]
#  [./pump_out]
#    type = TigerPointSourceH
#    point = '350.0 250.0 0.0'
#    mass_flux = -3.0
#    variable = pressure
#  [../]
#[]

[Kernels]
  [./H_diff]
    type = TigerKernelH
    variable = pressure
  [../]
[]

[Executioner]
  type = Steady
  l_tol = 1e-10 #difference between first and last linear steps
  nl_rel_step_tol = 1e-15 #machine percision
  nl_rel_tol = 1e-10 #difference between first and last nonlinear steps
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
  #petsc_options = '-snes_ksp_ew'
  #petsc_options_iname = '-ksp_type -pc_type -snes_atol -snes_rtol -snes_max_it -ksp_max_it -sub_pc_type -sub_pc_factor_shift_type'
  #petsc_options_value = 'gmres asm 1E-10 1E-15 200 500 lu NONZERO'
  #petsc_options = '-snes_ksp_ew'
  #petsc_options_iname = '-ksp_type -pc_type -snes_atol -snes_rtol -snes_max_it -ksp_max_it -sub_pc_factor_shift_type'
  #petsc_options_value = 'gmres lu 1E-10 1E-15 200 500 NONZERO'
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
[]
