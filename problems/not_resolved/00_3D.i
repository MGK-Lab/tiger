# Creates the mesh for the remainder of the tutorial
[Mesh]
  type = AnnularMesh
  dim = 2
  nr = 10
  rmin = 1.0
  rmax = 10
  growth_r = 1.4
  nt = 4
  tmin = 0
  tmax = 1.57079632679
[]

[MeshModifiers]
  [./make3D]
    type = MeshExtruder
    extrusion_vector = '0 0 12'
    num_layers = 3
    bottom_sideset = 'bottom'
    top_sideset = 'top'
  [../]
  [./shift_down]
    type = Transform
    transform = TRANSLATE
    vector_value = '0 0 -6'
    depends_on = make3D
  [../]
  [./aquifer]
    type = SubdomainBoundingBox
    block_id = 1
    bottom_left = '0 0 -2'
    top_right = '10 10 2'
    depends_on = shift_down
  [../]
  [./injection_area]
    type = ParsedAddSideset
    combinatorial_geometry = 'x*x+y*y<1.01'
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
    boundary = rmax
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
  type = FEProblem
[]

[Executioner]
  type = Steady
  solve_type = NEWTON
  petsc_options = '-snes_ksp_ew'
  petsc_options_iname = '-ksp_type -pc_type -snes_atol -snes_rtol -snes_max_it -ksp_max_it -sub_pc_type -sub_pc_factor_shift_type'
  petsc_options_value = 'gmres asm 1E-10 1E-10 200 500 lu NONZERO'
[]

[Outputs]
  file_base = 3D_mesh
  exodus = true
[]
