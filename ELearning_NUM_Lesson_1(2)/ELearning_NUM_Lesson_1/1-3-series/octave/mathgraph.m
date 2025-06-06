% 1 + 1/2 + 1/4 + 1/8 + 1/16 + 1/32 + 1/64 + 1/128 + 1/256 ...

disp("Octave")

function x = func(n)
    x = 1;

    for i = 1:n
        x = x + 1/(2^i);

        hold on
        scatter(i, x)
        grid on
        hold off
    end
end

func(10)

