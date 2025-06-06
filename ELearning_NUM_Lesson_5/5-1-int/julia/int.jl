# Trapezoidal Rule

using Plots

f(x) = exp(cos(x)^3)

function f_trap(a, b, n)
    h = (b - a)/n
    res = h * ( f(a) + f(b) ) / 2
    for k = 1:n-1
        xk = (b - a) * k/n + a
        res += h * f(xk)
    end
    return res
end

println(f_trap(0, 2π, 100))

x_true = LinRange(0, 2π, 1600)
x_trap = LinRange(0, 2π, 16)

plot(x_trap, f.(x_trap), fillrange=0)
plot!(x_true, f.(x_true), line=4)
