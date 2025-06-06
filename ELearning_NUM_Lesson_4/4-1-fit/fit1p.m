% linear least-squares fit

a0 = 0.5;
a1 = 0.5;

n  = 20;
x  = 0:10/n:10;
ys = a1*x + a0;

plot(x, ys, "r-.")

% simulate measured data

y = ys + 2*(-0.5+rand(1, n+1));

hold on
plot(x, y, "bo")
hold off

% determined equations

A = [n, sum(x);
     sum(x), sum(x.^2)];
B = [sum(y);
     sum(x.*y)];
af = A\B;

yf = af(2)*x + af(1);

hold on
plot(x, yf, "cy-");
hold off
