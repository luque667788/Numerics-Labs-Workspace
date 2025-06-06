warning('off','all')
mex mex_interp.c
warning('on','all')

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
f_interp = mex_interp(f_sample, xnodes, w, t);
plot(t, f(t), '-+', t, f_interp, '-o')

function y = f(x)
    y = 1./(1 + 16*x.^2);
end
