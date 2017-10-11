[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
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

[ICs]
  [./ic]
    type =  ConstantIC
    value = 0.5
    variable =  u
  [../]
[]

[AuxKernels]
  [./v_x]
    type =  VariableGradientComponent
    variable = vx
    gradient_variable = u
    component = x
  [../]
  [./v_y]
    type =  VariableGradientComponent
    variable = vy
    gradient_variable = u
    component = y
  [../]
[]

[Variables]
  [./u]
  [../]
[]

[Kernels]
  [./diff]
    type =  Diffusion
    variable = u
  [../]
  [./diff_t]
    type = TimeDerivative
    variable = u
  [../]
[]

[BCs]
  [./left]
    type = DirichletBC
    variable = u
    boundary = left
    value = 0
  [../]
  [./right]
    type = DirichletBC
    variable = u
    boundary = right
    value = 1
  [../]
[]

[Executioner]
  type = Transient
  dt = 0.01
  num_steps = 50
  nl_abs_tol = 1e-15
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
[]
