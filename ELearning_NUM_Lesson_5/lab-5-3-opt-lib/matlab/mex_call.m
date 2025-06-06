warning('off','all')
mex mex_opt.c -lnlopt
warning('on','all')

sol = mex_opt([0.0 0.0]);

fprintf("%.1f : %.1f : %.1f\n", sol(1), sol(2), -sol(3))
