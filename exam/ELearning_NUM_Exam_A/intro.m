% x'+ax=u

% sampling time
T = 0.1;
n = 100;
t = 0:T:n*T;
a = 1.5;

% input
u  = exp(-(t-5).^10);
x0 = 0;

% x'+ax=u, x'=u-ax
Dx = @(t, x) exp(-(t-5)^10) - a*x;

% solution by MATLAB
[ts, xs] = ode45(Dx, t, x0);

% solution by Euler method

% x'+ax=u, x'=u-ax
% Dx/T=u-ax
% x(k+1)=x(k)+T(u(k)-ax(k))=(1-aT)x(k)+Tu(k)

x = zeros(1, n+1);

x(1) = x0;
for k = 1:n
    x(k+1) = (1-a*T)*x(k) + T*u(k);
end

plot(t, u, ts, xs, t, x)
xlabel("t")
ylabel("x(t)")
legend("u", "ode", "T=0.1")
title("x(t)'+ax(t)=u(t)")
grid on
