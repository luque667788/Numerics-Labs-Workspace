% fmincon for minimum under constraints

% function
% f = 143*x + 60*y

% constraints
% 120x + 210y <= 15000
% 110x + 30y <= 4000
% x + y <= 75
% 0 <= x and 0 <= y

f = @(x) -(143*x(1) + 60*x(2))

A = [120 210 ;
     110  30 ;
       1   1];

B = [15000 ;
      4000 ;
        75];

% fmincon(f, x0, A, b, Aeq, beq, lb, ub)
% linprog(f,     A, b, Aeq, beq, lb, ub)

sol = fmincon(f, [0 0], A, B, [], [], [0 0]);

fprintf("%.1f : %.1f : %.1f\n", sol(1), sol(2), -f(sol))