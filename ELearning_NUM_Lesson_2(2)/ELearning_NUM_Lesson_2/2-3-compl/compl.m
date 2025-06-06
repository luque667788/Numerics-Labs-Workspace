% recursive sequence z^2 + c = (x^2 - y^2 + a) + j (2xy + b)

a = -0.1;
b = -0.0;

% a = -0.6;
% b = -0.6;

% a = -0.7;
% b = -0.7;

x = 0.0;
y = 0.0;
n = 1;

while (n < 10)
	xn = x^2 - y^2 + a;
    y = 2*x*y + b;
    x = xn;
	
	fprintf("x(%2d) = (% 1.2f, % 1.2f)\n", n, x, y)

	n = n + 1;
end
