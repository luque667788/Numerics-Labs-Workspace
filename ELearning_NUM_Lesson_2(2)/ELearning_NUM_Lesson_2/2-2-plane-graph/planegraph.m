% recursive sequence x1i+1=x1i+T(x1i(x2i-1)) and x2i+1=x2i+T(x2i(x1i-1))

T=0.01;
t=0:T:10;
x1=zeros(size(t));
x2=zeros(size(t));

% sample sequence 1

x101=1.5;
x201=0.5;

x1(1)=x101;
x2(1)=x201;
for i=1:length(t)-1
    x1(i+1)=x1(i)+T*(x1(i)*(x2(i)-1)); % x1i+1=x1i+T(x1i(x2i-1))
    x2(i+1)=x2(i)+T*(x2(i)*(x1(i)-1)); % x2i+1=x2i+T(x2i(x1i-1))
end

x11=x1;
x21=x2;

% sample sequence 2

x102=0.5;
x202=1.5;

x1(1)=x102;
x2(1)=x202;
for i=1:length(t)-1
    x1(i+1)=x1(i)+T*(x1(i)*(x2(i)-1)); % x1i+1=x1i+T(x1i(x2i-1))
    x2(i+1)=x2(i)+T*(x2(i)*(x1(i)-1)); % x2i+1=x2i+T(x2i(x1i-1))
end

x12=x1;
x22=x2;

plot(x11,x21,x12,x22)
set(gca,'dataaspectratio',[1,1,1])
grid on
hold on
plot(1, 1, 'ro', 0, 0, 'ro', x101, x201, 'bx', x102, x202, 'bx', 'MarkerFaceColor', [1, 0, 0])
hold off
