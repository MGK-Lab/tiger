[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 1
[]

[AuxVariables]
  [./den]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./vis]
    family = MONOMIAL
    order = CONSTANT
  [../]
[]

[Materials]
  [./water]
    type = TigerFluidMaterialTP
    viscosity = 0.002
    density = 1500
  [../]
[]

[AuxKernels]
  [./density]
    type = MaterialRealAux
    variable = den
    property = density
  [../]
  [./viscosity]
    type = MaterialRealAux
    variable = vis
    property = viscosity
  [../]
[]

[Variables]
  [./dummy]
  [../]
[]

[Kernels]
  [./diff]
    type =  Diffusion
    variable = dummy
  [../]
[]

[Executioner]
  type = Steady
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
[]
