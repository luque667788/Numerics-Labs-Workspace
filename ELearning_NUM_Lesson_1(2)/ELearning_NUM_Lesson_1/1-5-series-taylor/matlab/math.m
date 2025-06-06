% 1 + x + x^2/2 + x^3/6 + x^4/24 + ...

func(1, 9)

function y = func(x, n)
    y = 1

    for i = 1:n
        y = y + x^i/factorial(i)
    end
end