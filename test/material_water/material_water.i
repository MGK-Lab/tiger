[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 100
  ny = 1
[]

[Functions]
  [./temp_func]
    type =  ParsedFunction
    value = '350.0*x'
  [../]
[]

[ICs]
  [./temp_ic]
    type = FunctionIC
    function = temp_func
    variable = temp
  [../]
[]

[AuxVariables]
  [./press]
    family = MONOMIAL
    order = CONSTANT
    initial_condition = 100e6
  [../]
  [./temp]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./den]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./vis]
    family = MONOMIAL
    order = CONSTANT
  [../]
[]

[UserObjects]
  [./water]
    type =  TigerWaterProperties
  [../]
[]

[Materials]
  [./water]
    type = TigerFluidMaterialTP
    pressure = press
    temperature = temp
    fp = water 
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
  csv = true
[]
