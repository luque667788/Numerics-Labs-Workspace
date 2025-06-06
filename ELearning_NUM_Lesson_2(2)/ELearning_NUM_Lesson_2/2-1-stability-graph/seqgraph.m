% recursive sequence x^2 - 1

nmax = 50;

t = 0:nmax;
x = zeros(1, nmax+1);

% stable
x0 = -1.5;

x(1) = x0;
for n = 1:nmax
    x(n+1) = (x(n))^2 - 1;
end

stairs(t, x)
grid on
pause

% threshold
x0 = -1.618;

x(1) = x0;
for n = 1:nmax
    x(n+1) = (x(n))^2 - 1;
end

stairs(t, x)
grid on
pause

% unstable
% x0 = -1.618-0.000033;
x0 = -1.618-0.000034;

x(1) = x0;
for n = 1:nmax
    x(n+1) = (x(n))^2 - 1;
end

stairs(t, x)
grid on
