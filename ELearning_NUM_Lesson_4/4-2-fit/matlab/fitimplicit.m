% linear two-dimensional least-squares fit

a  = 2;
b  = 1.5;
th = pi/8;
x0 = 2;
y0 = 0;

n  = 100;
ts = 0:2*pi/n:2*pi;
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

D = 0.05;
dummy = -0.5:D:4.5;
Sx = zeros(1, length(dummy));
Sy = zeros(1, length(dummy));
k = 1;
for X = -0.5:D:4.5
    for Y = -2.5:D:2.5
        Z = p(1)*X^2 + p(2)*Y^2 + p(3)*X*Y + p(4)*X + p(5)*Y - 1;
        if (Z^2 < 0.0001)
            Sx(k) = X;
            Sy(k) = Y;
            k = k + 1;
        end
    end
end
N = k-1;

hold on
plot(Sx(1:N), Sy(1:N), "cyo")
hold off

% find convex hull set

Hx = zeros(1, N+1);
Hy = zeros(1, N+1);

% find left-most point
[dummy, l] = min(Sx(1:N));

k = 1;
p = l;
while true
    q = mod(p + 1, N) + 1;

    % find convex hull
    for i=1:N
        if rotation(Sx(p), Sy(p), Sx(i), Sy(i), Sx(q), Sy(q)) < 0
            q = i;	% if counterclockwise
        end
    end

    % add to convex hull set
	Hx(k) = Sx(q);
	Hy(k) = Sy(q);
	k = k + 1;

    % back to left-most point ?
	if q == l
	    break;
    end
		
	p = q;
end

% close convex hull

Hx(k) = Hx(1);
Hy(k) = Hy(1);

hold on
plot(Hx(1:k), Hy(1:k), "cy-")
hold off

function v = rotation(ax, ay, bx, by, cx, cy)
    v = (by - ay) * (cx - bx) - (bx - ax) * (cy - by);
end

function x = fx(t, a, b, th, x0)
    x = a*cos(th)*cos(t) - b*sin(th)*sin(t) + x0;
end

function y = fy(t, a, b, th, y0)
    y = a*sin(th)*sin(t) + b*cos(th)*cos(t) + y0;
end
