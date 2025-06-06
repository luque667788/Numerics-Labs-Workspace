% search for maximum under constraints

% function
% f = 143*x + 60*y

% constraints
% 120x + 210y <= 15000
% 110x + 30y <= 4000
% x + y <= 75
% 0 <= x and 0 <= y

xmax  = 75;
ymax  = 75;
ticks = 20;

xk = zeros(1, xmax*ticks*ymax*ticks);
yk = zeros(1, xmax*ticks*ymax*ticks);
zk = zeros(1, xmax*ticks*ymax*ticks);
xs = 0;
ys = 0;
zs = 0;
k  = 1;
for x = 1:1/ticks:xmax
    for y = 1:1/ticks:ymax
        if 120*x + 210*y <= 15000
            if 110*x + 30*y <= 4000
                if (x + y) <= 75
                    zk(k) = 143*x + 60*y;
                    xk(k) = x;
                    yk(k) = y;
                    
                    if zk(k) > zs
                        zs = zk(k);
                        xs = x;
                        ys = y;
                    end
                    k = k + 1;
                end
            end
        end
    end
end

n = k-1;

scatter3(xk(1:n), yk(1:n), zk(1:n), 'filled')
view(-30, 10)
grid on
pause

scatter(1:n, zk(1:n), 'filled')
grid on

fprintf("%.1f : %.1f : %.1f\n", xs, ys, zs)