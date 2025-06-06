# NLopt for minimum under constraints

using NLopt

# function
# f = 143*x + 60*y

# constraints
# 120x + 210y <= 15000
# 110x + 30y <= 4000
# x + y <= 75
# 0 <= x and 0 <= y

function f(x::Vector, grad::Vector)
    -(143*x[1] + 60*x[2]);
end

function constr1(x::Vector, grad::Vector)
    120*x[1] + 210*x[2] - 15000
end

function constr2(x::Vector, grad::Vector)
    110*x[1] + 30*x[2] - 4000
end

function constr3(x::Vector, grad::Vector)
    x[1] + x[2] - 75;
end

# algorithm and dimensionality

# NLOPT_LN_COBYLA : local optimization, no derivatives
# NLOPT_LD_MMA    : local optimization, derivative/gradient-based

opt = Opt(:LN_COBYLA, 2)
opt.lower_bounds = [0.0, 0.0]
opt.xtol_rel = 1e-4

opt.min_objective = f

inequality_constraint!(opt, (x, g) -> constr1(x, g), 1e-8)
inequality_constraint!(opt, (x, g) -> constr2(x, g), 1e-8)
inequality_constraint!(opt, (x, g) -> constr3(x, g), 1e-8)

(minf, minx, ret) = optimize(opt, [0.0, 0.0])
maxf = -minf
println("$minx : $maxf")
