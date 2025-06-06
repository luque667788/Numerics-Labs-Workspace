using Plots

# 1 + 1/2 + 1/4 + 1/8 + 1/16 + 1/32 + 1/64 + 1/128 + 1/256 ...

function func(n)
    y = 1
    display(scatter([0], [y]))

    for i in 1:n
        x = 2^i
        # x = Int128(2)^i
        if x == 0
            println("Error")
        end
        y += 1/x
        # println("y(", i, ") = ", y)
        display(scatter!([i], [y]))
    end

    return y
end

func(63)

# func(64)
