warning('off','all')
mex mex_seq.c
warning('on','all')

plot(mex_seq(50), '-o')
