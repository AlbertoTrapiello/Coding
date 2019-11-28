x_range = 'E262:E525'; %range of the cells for the x (time) values
y_range = 'G262:G525'; %range of the cells for the y (angle) values

y = xlsread('Excel Datos.xlsx', 'Data', x_range);
x = xlsread('Excel Datos.xlsx', 'Data', y_range);


u = ones(264,1); %vector of ones to make an step input
for k=1:264, u(k,1) = 90; end %making the step input into an 90 degrees step

xlabel('time (ms)');  ylabel('Angle (degrees)'); %sets the axis labels

plot(x,y,'g'); %represents the function of the system
hold on; plot(x,u,'r'); %adds the 90 degrees step to compare
figure(2);
plot(x,y);
hold on; plot(x,u,'r'); %adds the 90 degrees step to compare
figure(3);
plot(x,y);
hold on; plot(x,u,'r'); %adds the 90 degrees step to compare

%ident(); %calls the ident function to estimate the transfer function of the sistem

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
hold on; step(90*G1); %with this we can adjust the ident moel so that it mimics the real sistem
hold on; step(90*G1I); %with this we can adjust the ident moel so that it mimics the real sistem
hold on; step(90*G1Z); %with this we can adjust the ident moel so that it mimics the real sistem
hold on; step(90*G1D); %with this we can adjust the ident moel so that it mimics the real sistem
hold on; step(90*G1DI); %with this we can adjust the ident moel so that it mimics the real sistem
hold on; step(90*G1DZ); %with this we can adjust the ident moel so that it mimics the real sistem
hold on; step(90*G1IZ); %with this we can adjust the ident moel so that it mimics the real sistem
hold on; step(90*G1DIZ); %with this we can adjust the ident moel so that it mimics the real sistem
legend({'y1','step','G1','G1I','G1Z','G1D','G1DI','G1DZ','G1IZ','G1DIZ'}); % sets the name of the graph showing
xlabel('time (ms)');  ylabel('Angle (degrees)'); %sets the axis labels
axis([0 1000 0 120])

figure(2);
hold on; step(90*G2); %with this we can adjust the ident moel so that it mimics the real sistem
hold on; step(90*G2I); %with this we can adjust the ident moel so that it mimics the real sistem
hold on; step(90*G2Z); %with this we can adjust the ident moel so that it mimics the real sistem
hold on; step(90*G2D); %with this we can adjust the ident moel so that it mimics the real sistem
hold on; step(90*G2DI); %with this we can adjust the ident moel so that it mimics the real sistem
hold on; step(90*G2DZ); %with this we can adjust the ident moel so that it mimics the real sistem
hold on; step(90*G2IZ); %with this we can adjust the ident moel so that it mimics the real sistem
hold on; step(90*G2DIZ); %with this we can adjust the ident moel so that it mimics the real sistem
legend({'y1','step','G2','G2I','G2Z','G2D','G2DI','G2DZ','G2IZ','G2DIZ'}); % sets the name of the graph showing
xlabel('time (ms)');  ylabel('Angle (degrees)'); %sets the axis labels
axis([0 1000 0 120])

figure(3);
hold on; step(90*G3); %with this we can adjust the ident moel so that it mimics the real sistem
hold on; step(90*G3I); %with this we can adjust the ident moel so that it mimics the real sistem
hold on; step(90*G3Z); %with this we can adjust the ident moel so that it mimics the real sistem
hold on; step(90*G3D); %with this we can adjust the ident moel so that it mimics the real sistem
hold on; step(90*G3DI); %with this we can adjust the ident moel so that it mimics the real sistem
hold on; step(90*G3DZ); %with this we can adjust the ident moel so that it mimics the real sistem
hold on; step(90*G3IZ); %with this we can adjust the ident moel so that it mimics the real sistem
hold on; step(90*G3DIZ); %with this we can adjust the ident moel so that it mimics the real sistem
legend({'y1','step','G3','G3I','G3Z','G3D','G3DI','G3DZ','G3IZ','G3DIZ'}); % sets the name of the graph showing
xlabel('time (ms)');  ylabel('Angle (degrees)'); %sets the axis labels
axis([0 1000 0 120])

Info_G1 = stepinfo(G1); %gets the info of the step response of G1
Info_G1Z = stepinfo(G1Z); %gets the info of the step response of G1Z
Info_G2 = stepinfo(G2); %gets the info of the step response of G2
Info_G2Z = stepinfo(G2Z); %gets the info of the step response of G2Z
Info_G2DZ = stepinfo(G2DZ); %gets the info of the step response of G2DZ
Info_G3DZ = stepinfo(G3DZ); %gets the info of the step response of G3DZ
Info_Sist = stepinfo(y,x); %gets the info of the sistem to establish comparison

Times = [Info_G1.RiseTime Info_G1Z.RiseTime Info_G2.RiseTime Info_G2Z.RiseTime Info_G2DZ.RiseTime Info_G3DZ.RiseTime;
             Info_G1.SettlingTime Info_G1Z.SettlingTime Info_G2.SettlingTime Info_G2Z.SettlingTime Info_G2DZ.SettlingTime Info_G3DZ.SettlingTime;
             Info_G1.PeakTime Info_G1Z.PeakTime Info_G2.PeakTime Info_G2Z.PeakTime Info_G2DZ.PeakTime Info_G3DZ.PeakTime;]; %Saves the info of the steps in diferent rows 
deviation = [0 0 0 0 0 0;
             0 0 0 0 0 0;
             0 0 0 0 0 0]; %
         %RiseTime comparsion
for k=1:6, deviation(1,k) = Info_Sist.RiseTime - Times(1,k); end
%SettlingTime comparsion
for k=1:6, deviation(2,k) = Info_Sist.SettlingTime -Times(2,k); end
%PeakTime comparsion
for k=1:6, deviation(3,k) = Info_Sist.PeakTime - Times(3,k); end

%Separates the info in colum vectors
Dev_G1 = deviation(:,1); 
Dev_G1Z = deviation(:,2); 
Dev_G2 = deviation(:,3); 
Dev_G2Z = deviation(:,4); 
Dev_G2DZ = deviation(:,5); 
Dev_G3DZ = deviation(:,6);

%After reviewing the data we select the Sistem G2DZ as the closest to the
%original sistem. So that form now on this will be the sistem used to
%estimate the PID and further models





