warning('off','all')
mex mex_opt.c -IC:\msys64\ucrt64\include -lC:\msys64\ucrt64\lib\libnlopt.dll.a
warning('on','all')

sol = mex_opt([0.0 0.0]);

fprintf("%.1f : %.1f : %.1f\n", sol(1), sol(2), -sol(3))

