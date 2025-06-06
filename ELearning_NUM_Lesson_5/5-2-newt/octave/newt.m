% Newton-Raphson

disp("Newton-Raphson")

function y = f(x)
    y = x^x - 100;
end

function y = fp(x)
    y = x^x * (log(x) + 1);
end

function x = NewtonRaphson(x0)
    maxIter = 1000;
    tol = 1e-8;

    iter = 0;
    x = x0;
    fx = f(x);
    while abs(fx) > tol && iter < maxIter
        x = x - fx/fp(x);
        fx = f(x);
        iter = iter + 1;
    end
    x;
end

NewtonRaphson(1)

