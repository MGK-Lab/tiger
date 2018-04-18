# Creates the mesh for the remainder of the tutorial
[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  xmin = 1.0
  xmax = 10
  bias_x = 1.4
  ny = 3
  ymin = -6
  ymax = 6
[]

[MeshModifiers]
  [./aquifer]
    type = SubdomainBoundingBox
    block_id = 1
    bottom_left = '0 -2 0'
    top_right = '10 2 0'
  [../]
  [./injection_area]
    type = ParsedAddSideset
    combinatorial_geometry = 'x<1.0001'
    included_subdomain_ids = 1
    new_sideset_name = 'injection_area'
    depends_on = 'aquifer'
  [../]
  [./rename]
    type = RenameBlock
    old_block_id = '0 1'
    new_block_name = 'caps aquifer'
    depends_on = 'injection_area'
  [../]
[]

[UserObjects]
  [./water_uo]
    type = TigerFluidConst
    density = 1
    specific_heat = 1
    conductivity = 1e-4
  [../]
[]

[Materials]
  [./matrix_t]
    type = TigerRockMaterialT
    fp_UO = water_uo
    porosity = 1.0
    conductivity_type = isotropic
    mean_calculation_type = arithmetic
    lambda = 1
    density = 1
    specific_heat = 1
    block = caps
  [../]
  [./well_t]
    type = TigerRockMaterialT
    fp_UO = water_uo
    porosity = 1.0
    conductivity_type = isotropic
    mean_calculation_type = arithmetic
    lambda = 100
    density = 1
    specific_heat = 1
    block = aquifer
  [../]
[]

[BCs]
  [./top_t]
    type = DirichletBC
    variable = temperature
    boundary = right
    value = 0
  [../]
  [./inject_t]
    type = DirichletBC
    variable = temperature
    boundary = injection_area
    value = 100
  [../]
[]


[Variables]
  [./temperature]
  [../]
[]

[Kernels]
  [./T_advect]
    type = TigerDiffusionKernelT
    variable = temperature
  [../]
[]

[Problem]
  coord_type = RZ
  type = FEProblem
  rz_coord_axis = Y
[]

[Executioner]
  type = Steady
  solve_type = NEWTON
  petsc_options = '-snes_ksp_ew'
  petsc_options_iname = '-ksp_type -pc_type -snes_atol -snes_rtol -snes_max_it -ksp_max_it -sub_pc_type -sub_pc_factor_shift_type'
  petsc_options_value = 'gmres asm 1E-10 1E-10 200 500 lu NONZERO'
[]

[Outputs]
  file_base = 2D_mesh
  exodus = true
[]
