%=========================================================================
% script to compute the hydraulic head profile from a conductivity
% profile
%
% 1D problem which solves M*h=b
% plot result
%
% A/(dx^2)*{ (k_(i-1)+k_i)*h_(i-1)/2 -
%           [(k_(i-1)+k_i)+(k_i+k_(i+1))]*h_i/2 +
%            (k_i+k_(i+1))*h_(i+1)/2 }= 0
%
% INPUT:
%   - K_m, [nx1]%f: conductivity vector
%   - Hbc, [2x1]%f: Dirichlet boundary condition of hydraulic head
%       pressure, left (index 1) and right (index 2)
%   - area, %f: area of the cell, constant value
%   - dx, %f: cell length, constant
%
% OUTPUT:
%   - NONE
%
% ASSUMPTIONS:
%   - conductivity profile can be heterogeneous
%   - area kept constant
%   - 1D cell length kept constant
%=========================================================================
% function findif1D(K_m, Hbc, area, dx)

%% define initial parameters
%============================
area = 1;
dx = 1;
Hbc(1) = 2;
Hbc(2) = 0.5;

% 7 cells
% K_m = 1e-3*ones(7,1);                             % homogen
% K_m = [1e-4; 1e-4; 1e-4; 2e-3; 2e-3; 2e-3; 2e-3]; % heterogen

% 15 cells
% K_m = [0.01; 0.01; 0.01; 0.0005; 0.0005; 0.0005; 0.0005; 0.0005; 0.0005; 0.0005; 0.0005; 0.01; 0.01; 0.01; 0.01];
K_m = [0.01; 0.01; 0.02; 0.03; 0.04; 0.03; 0.02; 0.01; 0.005; 0.005];

% check that conductivity vector is a column vector
if size(K_m,2)~=1
    K_m = K_m';
end

% from now on all vectors will be vertical vectors (this is important to
% know for the multiplication between vectors and matrices...)

% calculate number of cells in the profile, which depends on the length of
% the conductivity vector
ncell = length(K_m);

% check that conductivitty vector has at least 3 elements otherwise exit!
if ncell<3
    fprintf(1,'ERROR: the conductivity vector must have at least 3 elements\n');
    return
end

%% define matrix M
%==================

% 1. creates the left- and right-vectors of conductivity of the finite
% difference equation
% vector-based calculation

% K_right below corresponds to a shift to the left of K_m. With this trick
% we can "align" cells i and i+1 of K_m for later summation
K_r = [K_m(2:ncell);0];

% then, the conductivity sum of the right hand side equals
Ks_r = (K_m + K_r)/2;
% correctly set last element: it is just the last value of the original K_m
Ks_r = [Ks_r(1:ncell-1); K_m(ncell)];

% left vector of conductivity sum is just the same as the right vector but with
% one cell delay. Indeed cell(i) of left vector = cell(i-1) of right vector
Ks_l = [K_m(1); Ks_r(1:ncell-1)]; % first element is simply the K_m initial value

% 2. build matrix
% initialise matrix with zeros
MAT = zeros(ncell);
% diagonal elements
MAT(1:ncell+1:end) = (Ks_l + Ks_r);
% lower off-diagonal
MAT(2:ncell+1:end) = -Ks_l(2:ncell);
% upper off-diagnonal
MAT(ncell+1:ncell+1:end) = -Ks_r(1:ncell-1);

% 3. don't forget to multiply by the area and divide by dx*dx
MAT = area*MAT/dx^2;

% 4. invert matrix
MATINV = inv(MAT);

%% build boundary condition vector
%==================================
% use hydraulic head boundary conditions

% 1. initialize to zero
Bound = zeros(ncell,1);

% 2. apply left and right boundaries
% do not forget area and dx...
Bound(1) = area*Ks_l(1)/dx^2 * Hbc(1);
Bound(ncell) = area*Ks_l(ncell)/dx^2 * Hbc(2);

%% calculate solution and show results
%======================================

% this is a matrix and vector multiplication
% Hsol = MATINV*Bound;
Hsol = MAT\Bound;

% plot results
figure;

% assume that left boundary is at 0 and first cell is between x=0.5 and 1.5
% hence, centre of 1st cell is at 1
% last boundary condition will be positioned at x = ncell*dx + dx
plot(dx*(1:ncell), Hsol, 'o-');
hold on;
% add left boundary to graph
plot(0, Hbc(1), 'o', 'markeredgecolor', 'r', 'markerfacecolor', 'r');
% add right boundary to graph
plot(dx*(ncell+1), Hbc(2), 'o', 'markeredgecolor', 'r', 'markerfacecolor', 'r');
grid on;
xlabel('X (m)');
ylabel('Hydraulic head');
hold off;

