G_Final = 90*G2DZ; %As established in the Sistem_Ident.m script

%To start with the empiric methods we need to get some parameters
% and this parameters are defined by the line defined by th enext two
% points

p1=[145, 35.5]; %coordenates (x,y) of the first point
p2=[129, 25.1]; %coordenates (x,y) of the first point

% with this two points we create the line between them

m = (p1(2)-p2(2))/(p1(1)-p2(1)); %the gradient of the line
n = p1(2)-p1(1)*m;%th eintercept of the line

x_tan = 0:700;
y_tan = m*x_tan + n;


%we see the representation to properly calculate the parameters
figure(1); plot(x_tan, y_tan,'g');
hold on; step(G_Final);
title('G_Final & tangent liine');
xlabel('time (ms)');  ylabel('Angle (degrees)'); %sets the axis labels
axis([0 1400 -60 120]);
legend({'y_t_a_n','G_Final'});
%with the data showed in the graph we can calculate the vlues for the
%empiric models

L = -n/m; %the result of solving the x vlaue for y = 0
K = dcgain(G_Final); %The K value is the dc gain
T = (K-n)/m - L; %% the T is the time passed between the line passsing through y = 0 and the moment when it reaches y = K
N = 10;

%using the zn function we calculate the values of the Kp, Ti, Td for each
%control

[G1_ZN,Kp_ZN1,Ti_ZN1,Td_ZN1]=zn(1,[K,L,T,N]); %Proportional control
[G2_ZN,Kp_ZN2,Ti_ZN2,Td_ZN2]=zn(2,[K,L,T,N]); %Proportional Integral control
[G3_ZN,Kp_ZN3,Ti_ZN3,Td_ZN3]=zn(3,[K,L,T,N]); %Proportional Integral Derivative control

%using the cohenpid function we calculate the values of the Kp, Ti, Td for each
%control

[G1_CC,Kp_CC1,Ti_CC1,Td_CC1]=cohenpid(1,[K,L,T,N]); %Proportional control
[G3_CC,Kp_CC3,Ti_CC3,Td_CC3]=cohenpid(2,[K,L,T,N]); %Proportional Integral Derivative control
[G4_CC,Kp_CC4,Ti_CC4,Td_CC4]=cohenpid(5,[K,L,T,N]); %Proportional Derivative control

%using the chreswickpid function we calculate the values of the Kp, Ti, Td for each
%control

[G1_CH,Kp_CH1,Ti_CH1,Td_CH1]=chreswickpid(1,1,[K,L,T,N,0]); %Proportional control
[G2_CH,Kp_CH2,Ti_CH2,Td_CH2]=chreswickpid(2,1,[K,L,T,N,0]); %Proportional Integral control
[G3_CH,Kp_CH3,Ti_CH3,Td_CH3]=chreswickpid(3,1,[K,L,T,N,0]); %Proportional Integral Derivative control

%Now that we have the control ssystems we have to arrange the new feeback
%system to control the motor

H = 1; %the feedback is 11

%ZN
S1_ZN = feedback(G_Final*G1_ZN,1); 
S2_ZN = feedback(G_Final*G2_ZN,1);
S3_ZN = feedback(G_Final*G3_ZN,1);

figure(2); %step(G_Final);
hold on; step(S1_ZN);
hold on; step(S2_ZN);
hold on; step(S3_ZN);
title('ZN Control');
xlabel('time (ms)');  ylabel('Angle (degrees)'); %sets the axis labels
%axis([0 1400 -60 120]);
legend({'P','PI', 'PID'});

%Cohen Coon
S1_CC = feedback(G_Final*G1_CC,1); 
S3_CC = feedback(G_Final*G3_CC,1);
S4_CC = feedback(G_Final*G4_CC,1);

figure(3); %step(G_Final);
hold on; step(S1_CC);
hold on; step(S3_CC);
hold on; step(S4_CC);
title('Cohen Coon Control');
xlabel('time (ms)');  ylabel('Angle (degrees)'); %sets the axis labels
%axis([0 1400 -60 120]);
legend({'P','PID', 'PD'});

%Chreswick
S1_CH = feedback(G_Final*G1_CH,1); 
S2_CH = feedback(G_Final*G2_CH,1);
S3_CH = feedback(G_Final*G3_CH,1);

figure(4); %step(G_Final);
hold on; step(S1_CH);
hold on; step(S2_CH);
hold on; step(S3_CH);
title('Chreswick Control');
xlabel('time (ms)');  ylabel('Angle (degrees)'); %sets the axis labels
%axis([0 1400 -60 120]);
legend({'P','PI', 'PID'});


