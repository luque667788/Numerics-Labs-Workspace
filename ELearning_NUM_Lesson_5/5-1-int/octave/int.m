% Trapezoidal Rule

disp("Trapezoidal Rule")

function y = f(x)
    y = exp(cos(x).^3);
end

function res = f_trap(a, b, n)
    h = (b - a)/n;
    res = h * ( f(a) + f(b) ) / 2;
    for k = 1:n-1
        xk = (b-a) * k/n + a;
        res = res + h * f(xk);
    end
    res;
end

f_trap(0, 2*pi, 100)

x_true = 0:2*pi/1600:2*pi;
x_trap = 0:2*pi/16:2*pi;

area(x_trap, f(x_trap))

hold on
plot(x_true, f(x_true), 'LineWidth', 2.0)
hold off

