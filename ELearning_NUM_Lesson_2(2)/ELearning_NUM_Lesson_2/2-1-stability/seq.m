% recursive sequence x^2 - 1

x = -1.5;
n = 1;

while (n < 10)
	x = x^2 - 1;
	
	fprintf("x(%2d) = % 1.2f\n", n, x)

	n = n + 1;
end
