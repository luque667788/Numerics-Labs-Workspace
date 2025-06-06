# Newton-Raphson

f(x) = x^x - 100

fp(x) = x^x * (log(x) + 1)

function NewtonRaphson(x0)
    maxIter = 1000
    tol = 1e-8

    iter = 0
    x = x0
    fx = f(x)
    while abs(fx) > tol && iter < maxIter
        x = x  - fx/fp(x)
        fx = f(x)
        iter += 1
    end
    return x
end

NewtonRaphson(1)
