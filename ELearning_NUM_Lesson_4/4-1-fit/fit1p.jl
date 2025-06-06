# linear least-squares fit

using Plots

a0 = 0.5;
a1 = 0.5;

n  = 20;
x  = LinRange(0, 10, n);
ys = a1.*x .+ a0

plot(x, ys, linewidth=4, color=:red)

# simulate measured data

y = ys + 2*(-0.5.+rand(n));

scatter!(x, y, color=:blue)

# determined equations

A = [n sum(x); sum(x) sum(x.^2)];
B = [sum(y), sum(x.*y)];
af = A\B;

yf = af[2].*x .+ af[1];

plot!(x, yf, linewidth=4, color=:cyan)
