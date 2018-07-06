[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 1
  ny = 300
  ymin = -3000
[]

[MeshModifiers]
  [./well1]
    type = SubdomainBoundingBox
    bottom_left = '-0.1 -2000 0'
    top_right = '1.1 -1000 0'
    block_id = 1
  [../]
  [./well2]
    type = SubdomainBoundingBox
    bottom_left = '-0.1 -3000 0'
    top_right = '1.1 -2000 0'
    block_id = 2
  [../]
[]

[Modules]
  [./FluidProperties]
    [./water_uo]
      type = SimpleFluidProperties
    [../]
  [../]
[]

[Materials]
  [./rock_t]
    type = TigerUncoupledThermalMaterialTH
    fp_UO = water_uo
    porosity = 0.2
    conductivity_type = isotropic
    lambda = 1
    density = 2600
    specific_heat = 850
    block = 0
  [../]
  [./rock_t0]
    type = TigerUncoupledThermalMaterialTH
    fp_UO = water_uo
    porosity = 0
    conductivity_type = isotropic
    lambda = 3
    density = 2600
    specific_heat = 850
    block = 1
    user_velocity = maz
  [../]
  [./rock_t1]
    type = TigerUncoupledThermalMaterialTH
    fp_UO = water_uo
    porosity = 0
    conductivity_type = isotropic
    lambda = 3
    density = 2600
    specific_heat = 850
    block = 2
  [../]
[]

[Functions]
  [./maz]
    type = ParsedVectorFunction
    value_y = -1e-9
  [../]
[]

[BCs]
  [./bott]
    type =  DirichletBC
    variable = temperature
    boundary = bottom
    value = 100
  [../]
  [./top]
    type = NeumannBC
    variable = temperature
    boundary = top
    value = -0.01
  [../]
  # [./right]
  #   type = TigerHeatConductionOutflowT
  #   variable = temperature
  #   boundary = 'right'
  # [../]
[]

[Variables]
  [./temperature]
  [../]
[]

[Kernels]
  [./T_diff]
    type = TigerDiffusionKernelT
    variable = temperature
  [../]
  [./T_advect]
    type = TigerAdvectionKernelTH
    variable = temperature
  [../]
  [./T_Source]
    type = TigerHeatSourceT
    variable = temperature
    value = 1.0e-6
  [../]
[]

[Executioner]
  type = Steady
  l_tol = 1e-10 #difference between first and last linear step
  nl_rel_step_tol = 1e-10 #machine percision
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
   print_linear_residuals = false
[]
