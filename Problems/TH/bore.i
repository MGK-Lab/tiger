# Creates the mesh for the remainder of the tutorial
[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 20
  xmin = 1
  xmax = 21
  # bias_x = 1.1
  ny = 20
  ymin = 0
  ymax = -20
[]

[MeshModifiers]
  [./well]
    type = MeshSideSet
    block_id = 1
    boundaries = 3
  [../]
  [./rename]
    type = RenameBlock
    old_block_id = '0 1'
    new_block_name = 'unit well'
    depends_on = 'well'
  [../]
  [./inject]
    type = AddExtraNodeset
    new_boundary = inject
    coord = '1 0'
  [../]
  [./inject_b]
    type = AddExtraNodeset
    new_boundary = bot
    coord = '1 -20'
  [../]
[]

[UserObjects]
  [./water_uo]
    type = TigerFluidConst
  [../]
[]

[Materials]
  [./matrix_t]
    type = TigerRockMaterialT
    fp_UO = water_uo
    porosity = 0
    conductivity_type = isotropic
    mean_calculation_type = arithmetic
    lambda = 1e-4
    density = 1
    specific_heat = 1
    block = unit
  [../]
  [./well_t]
    type = TigerRockMaterialT
    fp_UO = water_uo
    porosity = 0
    conductivity_type = isotropic
    mean_calculation_type = arithmetic
    lambda = 1e-2
    density = 1
    specific_heat = 1
    scaling_factor = 0.1
    block = well
  [../]
  [./w_th]
    type = TigerAdvectionMaterialTH
    fp_UO = water_uo
    has_supg = false
    user_velocity = maz
  [../]
[]

[ICs]
  [./tu]
    type = FunctionIC
    function = ic
    variable = temperature_u
  [../]
  [./tw]
    type = FunctionIC
    function = ic
    variable = temperature_w
  [../]
[]

[Functions]
  [./maz]
    type = ParsedVectorFunction
  [../]
  [./ic]
    type = ParsedFunction
    value = 'if(x<=1 & y>=0, 20, 100)'
  [../]
[]

[BCs]
  [./right_tu]
    type = DirichletBC
    variable = temperature_u
    boundary = right
    value = 100
  [../]
  [./inject_tu]
    type = DirichletBC
    variable = temperature_u
    boundary = inject
    value = 20
  [../]
  [./inject_tw]
    type = DirichletBC
    variable = temperature_w
    boundary = inject
    value = 20
  [../]
  [./inject_tu1]
    type = MatchedValueBC
    variable = temperature_u
    boundary = left
    v = temperature_w
  [../]
  [./inject_tw1]
    type = MatchedValueBC
    variable = temperature_w
    boundary = left
    v = temperature_u
  [../]
[]


[Variables]
  [./temperature_u]
     block = unit
  [../]
  [./temperature_w]
    block = well
  [../]
[]

[Kernels]
  [./Tw_diff]
    type = TigerDiffusionKernelT
    variable = temperature_w
    block = well
  [../]
  [./Tw_time]
    type = TigerTimeDerivativeT
    variable = temperature_w
    block = well
  [../]
  [./Tu_diff]
    type = TigerDiffusionKernelT
    variable = temperature_u
    block = unit
  [../]
  [./Tu_time]
    type = TigerTimeDerivativeT
    variable = temperature_u
    block = unit
  [../]
[]

[Problem]
  coord_type = RZ
  type = FEProblem
  rz_coord_axis = Y
[]

[Preconditioning]
  active = 'p1'
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
  l_tol = 1e-10
  l_max_its = 50
  nl_rel_tol = 1e-7
  nl_abs_tol = 1e-14
  nl_max_its = 50
[]

[Outputs]
  exodus = true
[]
