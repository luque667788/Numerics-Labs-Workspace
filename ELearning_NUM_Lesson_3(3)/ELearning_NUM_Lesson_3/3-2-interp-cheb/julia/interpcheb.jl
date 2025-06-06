using Plots

function LagrangeInterp1D(fvals, xnodes, barw, t)
    numt = 0
    denomt = 0

    for j = 1:length(xnodes)
        tdiff = t - xnodes[j]
        numt = numt + barw[j] / tdiff * fvals[j]
        denomt = denomt + barw[j] / tdiff

        if (abs(tdiff) < 1e-15)
            numt = fvals[j]
            denomt = 1.0
            break
        end
    end

    return numt / denomt
end

ClosedChebNodes(n) = [cos(j*π/n) for j=0:n]
ClosedChebBarWeights(n) = [0.5; [(-1)^j for j=1:n-1]; 0.5*(-1)^n]

f(x) = 1/(1 + 16*x^2)

# Sampling

n = 32;
xnodes = ClosedChebNodes(n);
w = ClosedChebBarWeights(n);
f_sample = f.(xnodes);

t = LinRange(-1, 1, 50)
f_interp = [LagrangeInterp1D(f_sample, xnodes, w, t[j]) for j=1:length(t)]
plot(t, f.(t), marker = 8)
plot!(t, f_interp, marker = 4)
