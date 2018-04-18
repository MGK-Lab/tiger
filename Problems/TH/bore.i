[Mesh]
  type = FileMesh
  file = untitled.msh
  parallel_type = replicated
[]

[MeshModifiers]
  # [./well]
  #   type = MeshSideSet
  #   block_id = 1
  #   boundaries = 3
  # [../]
  # [./rename]
  #   type = RenameBlock
  #   old_block_id = '0 1'
  #   new_block_name = 'unit well'
  #   depends_on = 'well'
  # [../]
  # [./inject]
  #   type = AddExtraNodeset
  #   new_boundary = inject
  #   coord = '1 0'
  # [../]
  # [./inject_b]
  #   type = AddExtraNodeset
  #   new_boundary = bot
  #   coord = '1 -20'
  # [../]
[]

[UserObjects]
  [./water_uo]
    type = TigerFluidConst
  [../]
[]

[Materials]
  [./matrix_t]
    type = TigerUncoupledThermalMaterialTH
    fp_UO = water_uo
    porosity = 0
    conductivity_type = isotropic
    lambda = 1e-4
    density = 1
    specific_heat = 1
    user_velocity = maz
    block = unit
  [../]
  [./well_t]
    type = TigerUncoupledThermalMaterialTH
    fp_UO = water_uo
    porosity = 0
    conductivity_type = isotropic
    lambda = 1e-2
    density = 1
    specific_heat = 1
    user_velocity = maz
    block = well
  [../]
[]

[ICs]
  [./tu]
    type = FunctionIC
    function = icu
    variable = temperature_u
  [../]
  [./tw]
    type = FunctionIC
    function = icw
    variable = temperature_w
  [../]
[]

[Functions]
  [./maz]
    type = ParsedVectorFunction
  [../]
  [./icu]
    type = ParsedFunction
    value = 'if(x<1, 20, 100)'
  [../]
  [./icw]
    type = ParsedFunction
    value = 'if(x<1 & y>=0, 20, 0)'
  [../]
[]

[BCs]
  [./right_tu]
    type = DirichletBC
    variable = temperature_u
    boundary = rightu
    value = 100
  [../]
  # [./inject_tu]
  #   type = DirichletBC
  #   variable = temperature_u
  #   boundary = inject
  #   value = 20
  # [../]
  [./inject_tw]
    type = DirichletBC
    variable = temperature_w
    boundary = inject
    value = 20
  [../]
  # [./inject_tu1]
  #   type = MatchedValueBC
  #   variable = temperature_u
  #   boundary = left
  #   v = temperature_w
  # [../]
  # [./inject_tw1]
  #   type = MatchedValueBC
  #   variable = temperature_w
  #   boundary = left
  #   v = temperature_u
  # [../]
[]

[Constraints]
  [./unit1]
    type = TiedValueConstraint
    master = 5
    master_variable = temperature_u
    slave = 2
    variable = temperature_u
    scaling = 1e-4
  [../]
  [./unit2]
    type = TiedValueConstraint
    master = 4
    master_variable = temperature_u
    slave = 2
    variable = temperature_u
    scaling = 1e-4
  [../]
  [./well1]
    type = TiedValueConstraint
    master = 5
    master_variable = temperature_w
    slave = 2
    variable = temperature_w
    scaling = 1e-4
  [../]
  [./well2]
    type = TiedValueConstraint
    master = 4
    master_variable = temperature_w
    slave = 2
    variable = temperature_w
    scaling = 1e-4
  [../]
[]


[Variables]
  [./temperature_u]
  [../]
  [./temperature_w]
  [../]
[]

[Kernels]
  [./Tw_diff]
    type = TigerDiffusionKernelT
    variable = temperature_w
  [../]
  [./Tw_time]
    type = TigerTimeDerivativeT
    variable = temperature_w
  [../]
  [./Tu_diff]
    type = TigerDiffusionKernelT
    variable = temperature_u
  [../]
  [./Tu_time]
    type = TigerTimeDerivativeT
    variable = temperature_u
  [../]
  # [./Tu_source]
  #   type = CoupledForce
  #   variable = temperature_u
  #   v = temperature_w
  #   coef = 1
  # [../]
  # [./Tu_line]
  #   type = Reaction
  #   variable = temperature_u
  #   coef = 1e5
  # [../]
[]

# [Problem]
#   coord_type = RZ
#   type = FEProblem
#   rz_coord_axis = Y
# []

[Preconditioning]
  active = 'p3'
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
[]

[Executioner]
  type = Transient
  num_steps = 100
  end_time = 3e7
  solve_type = NEWTON
  l_tol = 1e-14
  l_max_its = 50
  nl_rel_tol = 1e-7
  nl_abs_tol = 1e-13
  nl_max_its = 100
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
[]
