x_range = 'E262:E525'; %range of the cells for the x (time) values
y_range = 'G262:G525'; %range of the cells for the y (angle) values

y = xlsread('Excel Datos.xlsx', 'Data', x_range);
x = xlsread('Excel Datos.xlsx', 'Data', y_range);

plot(x,y,'g'); %represents the function of the system
figure(2);
plot(x,y);
figure(3);
plot(x,y);

u = ones(264,1); %vector of ones to make an step input
for k=1:264, u(k,1) = 90; end %making the step input into an 90 degrees step

xlabel('time (ms)');  ylabel('Angle (degrees)'); %sets the axis labels
hold on; plot(x,u,'r'); %adds the 90 degrees step to compare

ident(); %calls the ident functionto estimate the transfer function of the sistem

G1=tf(P1);
G1I=tf(P1I);
G1Z=tf(P1Z);
G1D=tf(P1D);
G1DI=tf(P1DI);
G1DZ=tf(P1DZ);
G1IZ=tf(P1IZ);
G1DIZ=tf(P1DIZ);

G2=tf(P2);
G2I=tf(P2I);
G2Z=tf(P2Z);
G2D=tf(P2D);
G2DI=tf(P2DI);
G2DZ=tf(P2DZ);
G2IZ=tf(P2IZ);
G2DIZ=tf(P2DIZ);

G3=tf(P3);
G3I=tf(P3I);
G3Z=tf(P3Z);
G3D=tf(P3D);
G3DI=tf(P3DI);
G3DZ=tf(P3DZ);
G3IZ=tf(P3IZ);
G3DIZ=tf(P3DIZ);

figure(1);
%hold on; step(90*G1); %with this we can adjust the ident moel so that it mimics the real sistem
%hold on; step(90*G1I); %with this we can adjust the ident moel so that it mimics the real sistem
%hold on; step(90*G1Z); %with this we can adjust the ident moel so that it mimics the real sistem
%hold on; step(90*G1D); %with this we can adjust the ident moel so that it mimics the real sistem
%hold on; step(90*G1DI); %with this we can adjust the ident moel so that it mimics the real sistem
%hold on; step(90*G1DZ); %with this we can adjust the ident moel so that it mimics the real sistem
%hold on; step(90*G1IZ); %with this we can adjust the ident moel so that it mimics the real sistem
%hold on; step(90*G1DIZ); %with this we can adjust the ident moel so that it mimics the real sistem
figure(2);
hold on; step(90*G2); %with this we can adjust the ident moel so that it mimics the real sistem
hold on; step(90*G2I); %with this we can adjust the ident moel so that it mimics the real sistem
hold on; step(90*G2Z); %with this we can adjust the ident moel so that it mimics the real sistem
hold on; step(90*G2D); %with this we can adjust the ident moel so that it mimics the real sistem
hold on; step(90*G2DI); %with this we can adjust the ident moel so that it mimics the real sistem
hold on; step(90*G2DZ); %with this we can adjust the ident moel so that it mimics the real sistem
hold on; step(90*G2IZ); %with this we can adjust the ident moel so that it mimics the real sistem
hold on; step(90*G2DIZ); %with this we can adjust the ident moel so that it mimics the real sistem
figure(3);
% hold on; step(90*G3); %with this we can adjust the ident moel so that it mimics the real sistem
% hold on; step(90*G3I); %with this we can adjust the ident moel so that it mimics the real sistem
% hold on; step(90*G3Z); %with this we can adjust the ident moel so that it mimics the real sistem
% hold on; step(90*G3D); %with this we can adjust the ident moel so that it mimics the real sistem
% hold on; step(90*G3DI); %with this we can adjust the ident moel so that it mimics the real sistem
% hold on; step(90*G3DZ); %with this we can adjust the ident moel so that it mimics the real sistem
% hold on; step(90*G3IZ); %with this we can adjust the ident moel so that it mimics the real sistem
% hold on; step(90*G3DIZ); %with this we can adjust the ident moel so that it mimics the real sistem

legend({'y1','step','G2','G2Z','G2I','G2D','G2DI','GDZ','G2IZ','G2DIZ'}); % sets the name of the graph showing


