% x''+2ax'+a2x=u as 2-stage x'+ax=u

% sampling time
T = 0.1;
n = 100;
t = 0:T:n*T;
a = 1.5;

% input
u  = exp(-(t-5).^10);
x0 = 0;

% solution by Euler method

% x'+ax=u, x'=u-ax
% Dx/T=u-ax
% x(k+1)=x(k)+T(u(k)-ax(k))=(1-aT)x(k)+Tu(k)

function x = euler(u, a, x0, T, n)
    x = zeros(1, n+1);
    x(1) = x0;
    for k = 1:n
        x(k+1) = (1-a*T)*x(k) + T*u(k);
    end
end

% x'+ax=u
x1 = euler(u, a, x0, T, n);

% x'+ax=u
x2 = euler(x1, a, x0, T, n);

plot(t, u, t, x1, t, x2)
disp([t' u' x1' x2'])
grid on

