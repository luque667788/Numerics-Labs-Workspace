% quadratic least-squares fit

a0 = 0.5;
a1 = 0.5;
a2 = 0.1;

n  = 20;
x  = 0:10/n:10;
ys = a2*x.^2 + a1*x + a0;

plot(x, ys, "r-.")

% simulate measured data

y = ys + 4*(-0.5+rand(1, n+1));

hold on
plot(x, y, "bo")
hold off

% determined equations

A = [n, sum(x), sum(x.^2);
     sum(x), sum(x.^2), sum(x.^3);
     sum(x.^2), sum(x.^3), sum(x.^4)];
B = [sum(y);
     sum(x.*y);
     sum((x.^2).*y)];
af = A\B;

yf = af(3)*x.^2 + af(2)*x + af(1);

hold on
plot(x, yf, "cy-");
hold off
