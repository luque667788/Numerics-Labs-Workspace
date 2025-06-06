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

function y = f(x)
    y = 1./(1 + 16*x.^2);
end

f_sample = f(xnodes);

t = -1:1/25:1;
f_interp = t;
for k = 0:length(t)-1
    f_interp(k+1) = mex_interp(f_sample, xnodes, w, t(k+1));
end
plot(t, f(t), '-+', t, f_interp, '-o')

