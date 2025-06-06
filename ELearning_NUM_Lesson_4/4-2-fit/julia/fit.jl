# linear two-dimensional least-squares fit

using Plots

a  = 2;
b  = 1.5;
θ  = π/8;
x0 = 2;
y0 = 0;

n  = 100;
ts = LinRange(0, 2π, n);

fx(t) = a*cos(θ)*cos(t) - b*sin(θ)*sin(t) + x0
fy(t) = a*sin(θ)*sin(t) + b*cos(θ)*cos(t) + y0

xs = fx.(ts);
ys = fy.(ts);

x = xs + 0.5*(-0.5.+rand(n));
y = ys + 0.5*(-0.5.+rand(n));

# overdetermined linear system

A = [x.^2 y.^2 x.*y x y]
B = ones(n)

p = A\B

X = LinRange(minimum(x), maximum(x), 100)
Y = LinRange(minimum(y), maximum(y), 100)
F = Array{Float64}(undef,100,100)
for i ∈ 1:100, j ∈ 1:100
    F[i,j] = p[1]*X[i]^2 + p[2]*Y[j]^2 + p[3]*X[i]*Y[j] + p[4]*X[i] + p[5]*Y[j]
end

scatter(x, y, color=:blue)
plot!(xs, ys, linewidth=4, color=:red)
contour!(X, Y, F, linewidth=3, levels=[1], color=:cyan)
plot!([], color=:cyan, label="ŷ")
