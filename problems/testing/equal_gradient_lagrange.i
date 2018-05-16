[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 50
  ny = 50
[]

[MeshModifiers]
  [./box1]
    type = SubdomainBoundingBox
    block_id = 1
    bottom_left = '0 0 0'
    top_right = '0.51 1 0'
  [../]
  [./box2]
    type = SubdomainBoundingBox
    block_id = 2
    bottom_left = '0.49 0 0'
    top_right = '1 1 0'
  [../]
  [./iface]
    type = SideSetsBetweenSubdomains
    master_block = 1
    paired_block = 2
    new_boundary = 10
    depends_on = 'box1 box2'
  [../]
[]

[GlobalParams]
  order = FIRST
  family = LAGRANGE
[]

[Variables]
  [./u]
    block = 1
    initial_condition = 0
  [../]
  [./v]
    block = 2
    initial_condition = 0
  [../]
  [./lambda]
  [../]
[]


[BCs]
  [./left]
    type = DirichletBC
    variable = u
    boundary = left
    value = 2
  [../]
  [./right]
    type = DirichletBC
    variable = v
    boundary = right
    value = 1
  [../]
[]

[Kernels]
  [./u_diff]
    type = Diffusion
    variable = u
    block = 1
  [../]
  [./u_dt]
    type = TimeDerivative
    variable = u
    block = 1
  [../]
  [./v_diff]
    type = Diffusion
    variable = v
    block = 2
  [../]
  [./v_dt]
    type = TimeDerivative
    variable = v
    block = 2
  [../]
  [./lambda]
    type = NullKernel
    variable = lambda
  [../]
[]

[InterfaceKernels]
   #  [./iface]
   #   type = InterfaceDiffusionBoundaryTerm
   #   boundary = 10
   #   variable = u
   #   neighbor_var = v
   # [../]
   # [./lambda]
   #   type = EqualGradientLagrangeMultiplier
   #   variable = lambda
   #   boundary = 10
   #   element_var = u
   #   neighbor_var = v
   #   component = 0
   # [../]
   # [./constraint]
   #   type = EqualGradientLagrangeInterface
   #   boundary = 10
   #   lambda = lambda
   #   variable = u
   #   neighbor_var = v
   #   component = 0
   # [../]

  [./flux_continuity]
    type = InterfaceDiffusionFluxMatch
    variable = u
    boundary = 10
    neighbor_var = v
    D = 10
    D_neighbor = 1
  [../]
  [./diffusion_surface_term]
    type = InterfaceDiffusionBoundaryTerm
    boundary = 10
    variable = u
    neighbor_var = v
  [../]
[]

[Preconditioning]
  active = 'p2'
  [./p1]
    type = SMP
    full = true
    petsc_options_iname = '-pc_type -pc_hypre_type'
    petsc_options_value = 'hypre boomeramg'
  [../]
  [./p2]
    type = SMP
    full = true
    petsc_options_iname = '-pc_type -sub_pc_type -sub_pc_factor_shift_type'
    petsc_options_value = 'asm lu NONZERO'
  [../]
  [./p3]
    type = SMP
    full = true
    petsc_options_iname = '-pc_type -ksp_type -sub_pc_type -pc_asm_overlap -sub_pc_factor_shift_type'
    petsc_options_value = 'asm gmres lu 2 NONZERO'
  [../]
[]

[Executioner]
  type = Transient
  nl_abs_tol = 1e-13
  nl_rel_tol = 1e-08
  dt = 0.01
  num_steps = 20
[]

[Outputs]
  exodus = true
  print_linear_residuals = true
[]
