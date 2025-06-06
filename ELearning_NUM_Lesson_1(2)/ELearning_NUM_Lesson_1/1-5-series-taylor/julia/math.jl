using Plots

# 1 + x + x^2/2 + x^3/6 + x^4/24 + ...

function func(x, n)
    y = 1
    display(scatter([0], [y]))

    for i in 1:n
        y += x^i/factorial(i)
        println("y( ", i, ") = ", y)
        display(scatter!([i], [y]))
    end

    return y
end

func(1, 9)
