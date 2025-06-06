warning('off','all')
mex mex_interp.c
warning('on','all')

t = -1:1/25:1;
f_interp = t;
for k = 0:length(t)-1
    f_interp(k+1) = mex_interp(t(k+1));
end
plot(t, f_interp, '-o')
