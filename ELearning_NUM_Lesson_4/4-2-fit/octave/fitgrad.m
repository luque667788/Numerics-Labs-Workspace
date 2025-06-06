% linear two-dimensional least-squares fit

a  = 2;
b  = 1.5;
th = pi/8;
x0 = 2;
y0 = 0;

n  = 100;
ts = 0:2*pi/n:2*pi;

function x = fx(t, a, b, th, x0)
    x = a*cos(th)*cos(t) - b*sin(th)*sin(t) + x0;
end

function y = fy(t, a, b, th, y0)
    y = a*sin(th)*sin(t) + b*cos(th)*cos(t) + y0;
end

xs = fx(ts, a, b, th, x0);
ys = fy(ts, a, b, th, y0);

plot(xs, ys, "r-")

x = xs + 0.5*(-0.5+rand(1, n+1));
y = ys + 0.5*(-0.5+rand(1, n+1));

hold on
plot(x, y, "bo")
hold off

% overdetermined linear system

A = [x.^2; y.^2; x.*y; x; y]';
B = ones(n+1, 1);

p = A\B;
% p = mldivide(A, B);
% p = lsqminnorm(A, B);

X = min(x):(max(x)-min(x))/100:max(x);
Y = min(y):(max(y)-min(y))/100:max(y);
F = zeros(100+1, 100+1);
for i = 1:100
  for j = 1:100
    F(i,j) = p(1)*X(i)^2 + p(2)*Y(j)^2 + p(3)*X(i)*Y(j) + p(4)*X(i) + p(5)*Y(j);
  end
end

hold on
contour(X, Y, F, 'LevelList', [1])
hold off

