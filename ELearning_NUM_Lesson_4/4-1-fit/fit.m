% linear least-squares fit

% example

x = [-1 1 2];
y = [-2 0 3];
n = 3;

% y = a1 x + a0
% a0 + x a1 = y
% [1 x][a0 a1] = [y]
% A a = B

A = [ones(n, 1) x']
B = y'

% solve overdetermined equations

af = A\B
% p = mldivide(A, B);
% p = lsqminnorm(A, B);
pause

% alternative: normal equations

% X = A
% X' X a = X' y'

X = A;

A = X'*X
B = X'*y'

% solve determined equations

af = A\B
pause

% alternative formulation of normal equations

A = [n, sum(x);
     sum(x), sum(x.^2)]
B = [sum(y);
     sum(x.*y)]

% solve determined equations

af = A\B