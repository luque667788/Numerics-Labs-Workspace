% Sampling

n = 15;
% EquispacedNodes
xnodes = 0:n;
xnodes = -1+xnodes*2/n;
% EquispacedBarWeights
w = xnodes;
for k = 0:n
    % binomial(n, k)
    w(k+1) = (-1)^k * nchoosek(n, k);
end

f_sample = f(xnodes);

t = -1:1/25:1;
f_interp = t;
for k = 0:length(t)-1
    f_interp(k+1) = LagrangeInterp1D(f_sample, xnodes, w, t(k+1));
end
plot(t, f(t), '-+', t, f_interp, '-o')

function y = f(x)
    y = 1./(1 + 16*x.^2);
end

function y = LagrangeInterp1D(fvals, xnodes, barw, t)
    numt = 0;
    denomt = 0;
    
    for j = 1:length(xnodes)
        tdiff = t - xnodes(j);
        numt = numt + barw(j) / tdiff * fvals(j);
        denomt = denomt + barw(j) / tdiff;
        
        if (abs(tdiff) < 1e-15)
            numt = fvals(j);
            denomt = 1.0;
            break
        end
    end
    y = numt / denomt;
end
