% recursive sequence x1i+1=x1i+T(x1i(x2i-1)) and x2i+1=x2i+T(x2i(x1i-1))

T=0.01;

x10=-2.5;
x20=-2.5;

n=100+1;
dx=0.05;
nmax=600;
xk=ones(n, n);
yk=ones(n, n);
zk=ones(n, n);
x1=x10;
x2=x20;
for j=1:n
    for i=1:n
        x1k=x1;
        x2k=x2;
        for k=1:nmax
            x1k=x1k+T*(x1k*(x2k-1)); % x1i+1=x1i+T(x1i(x2i-1))
            x2k=x2k+T*(x2k*(x1k-1)); % x2i+1=x2i+T(x2i(x1i-1))
            r=sqrt(x1k^2+x2k^2);
            if (r<0.1)
                break
            end
        end
        xk(i,j)=x1;
        yk(i,j)=x2;
        zk(i,j)=k;
        x1=x1+dx;
    end
    x1=x10;
    x2=x2+dx;
end

h=surf(xk,yk,zk);
set(h,'edgecolor','none')
colorbar
view(-20, 60)
shading interp
set(gca,'XLim',[-2.5 2.5])
set(gca,'YLim',[-2.5 2.5])
xlabel('x1')
ylabel('x2')
zlabel('no. iterations')
hold on
plot3(1, 1, nmax+10, 'ro', 0, 0, 0+10+50, 'ro', 2.5, -0.15, 0+300, 'bx', -0.15, 2.5, 0+300, 'bx', 'MarkerFaceColor', [1, 0, 0])
hold off
