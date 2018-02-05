[Mesh]
  type = GeneratedMesh
  dim = 1
  xmin = 0
  xmax = 10
  nx = 100
[]

[MeshModifiers]
  [./block1]
    type = ParsedSubdomainMeshModifier
    combinatorial_geometry = 'x<1'
    block_id = 1
  [../]
  [./block2]
    type = ParsedSubdomainMeshModifier
    combinatorial_geometry = 'x>=1 & x<2'
    block_id = 2
  [../]
  [./block3]
    type = ParsedSubdomainMeshModifier
    combinatorial_geometry = 'x>=2 & x<3'
    block_id = 3
  [../]
  [./block4]
    type = ParsedSubdomainMeshModifier
    combinatorial_geometry = 'x>=3 & x<4'
    block_id = 4
  [../]
  [./block5]
    type = ParsedSubdomainMeshModifier
    combinatorial_geometry = 'x>=4 & x<5'
    block_id = 5
  [../]
  [./block6]
    type = ParsedSubdomainMeshModifier
    combinatorial_geometry = 'x>=5 & x<6'
    block_id = 6
  [../]
  [./block7]
    type = ParsedSubdomainMeshModifier
    combinatorial_geometry = 'x>=6 & x<7'
    block_id = 7
  [../]
  [./block8]
    type = ParsedSubdomainMeshModifier
    combinatorial_geometry = 'x>=7 & x<8'
    block_id = 8
  [../]
  [./block9]
    type = ParsedSubdomainMeshModifier
    combinatorial_geometry = 'x>=8 & x<9'
    block_id = 9
  [../]
  [./block10]
    type = ParsedSubdomainMeshModifier
    combinatorial_geometry = 'x>=9 & x<10'
    block_id = 10
  [../]
[]

[UserObjects]
  [./water_uo]
    type = TigerFluidConst
  [../]
  [./rock_uo1]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '1.0e-9'
  [../]
  [./rock_uo2]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '2.0e-9'
  [../]
  [./rock_uo3]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '3.0e-9'
  [../]
  [./rock_uo4]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '4.0e-9'
  [../]
  [./rock_uo05]
    type =  TigerPermeabilityConst
    permeability_type = isotropic
    k0 = '5.0e-10'
  [../]
[]

[Materials]
  [./rock_h128]
    type = TigerRockMaterialH
    fp_UO = water_uo
    porosity = 0.2
    compressibility = 1.0e-9
    kf_UO = rock_uo1
    block = '1 2 8'
  [../]
  [./rock_h37]
    type = TigerRockMaterialH
    fp_UO = water_uo
    porosity = 0.2
    compressibility = 1.0e-9
    kf_UO = rock_uo2
    block = '3 7'
  [../]
  [./rock_h46]
    type = TigerRockMaterialH
    fp_UO = water_uo
    porosity = 0.2
    compressibility = 1.0e-9
    kf_UO = rock_uo3
    block = '4 6'
  [../]
  [./rock_h5]
    type = TigerRockMaterialH
    fp_UO = water_uo
    porosity = 0.2
    compressibility = 1.0e-9
    kf_UO = rock_uo4
    block = 5
  [../]
  [./rock_h910]
    type = TigerRockMaterialH
    fp_UO = water_uo
    porosity = 0.2
    compressibility = 1.0e-9
    kf_UO = rock_uo05
    block = '9 10'
  [../]
[]

[BCs]
  [./left]
    type = DirichletBC
    variable = pressure
    boundary = left
    value = 2e4
  [../]
  [./back]
    type = DirichletBC
    variable = pressure
    boundary = right
    value = 5e3
  [../]
[]

[AuxVariables]
  [./vx]
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
[]

[Variables]
  [./pressure]
    initial_condition = 5e3
  [../]
[]

[Kernels]
  [./diff]
    type = TigerKernelH
    variable = pressure
  [../]
  [./time]
    type = TigerTimeDerivativeH
    variable = pressure
  [../]
[]

[Executioner]
  type = Transient
  num_steps = 20
  end_time = 0.25
  l_tol = 1e-10 #difference between first and last linear step
  nl_rel_step_tol = 1e-14 #machine percision
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
[]
