# Not commutative

#  1 - 1/2  + 1/3 - 1/4 + 1/5 - 1/6 + 1/7 - 1/8 + 1/9 - 1/10 + 1/11 - 1/12 ... = ln 2 = 0.69

# (1 - 1/2) - 1/4 + (1/3 - 1/6) - 1/8 + (1/5 - 1/10) - 1/12 ... = ln 2 = 0.69

# (    1/2) - 1/4 + (      1/6) - 1/8 + (      1/10) - 1/12 ... = 1/2 ln 2 = 0.35

function func(n)
    x = 0

    for i in 1:n
        #=
        if mod(i, 2) == 0
            x -= 1/i
        else
            x += 1/i
        end
        =#
        
        if mod(i, 2) == 0
            x -= 1/(2*i)
            # println("-", 1/(2*i))
        else
            x += 1/(2*i)
            # println("+", 1/(2*i))
        end
        
    end

    return x
end

func(63)
