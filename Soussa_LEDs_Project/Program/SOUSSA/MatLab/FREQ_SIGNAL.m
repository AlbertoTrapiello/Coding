%NUEVO SCRIPT PARA CALCULAR

%Signal with 100 Hz of frequency
x_100_range = 'D3:D1873'; %range of the cells for the x (time) values
y_100_range = 'E3:E1873'; %range of the cells for the y (angle) values
sample_rate_100 = 1873-3+1;

x_100 = xlsread('Datos micrófono.xlsx', 'Hoja1', x_100_range);
y_100 = xlsread('Datos micrófono.xlsx', 'Hoja1', y_100_range);

%Signal with 200 Hz of frequency
x_200_range = 'H3:H1865'; %range of the cells for the x (time) values
y_200_range = 'I3:I1865'; %range of the cells for the y (angle) values
sample_rate_200 = 1865-3+1;

x_200 = xlsread('Datos micrófono.xlsx', 'Hoja1', x_200_range);
y_200 = xlsread('Datos micrófono.xlsx', 'Hoja1', y_200_range);

%Signal with 201 Hz of frequency
x_500_range = 'L3:L1865'; %range of the cells for the x (time) values
y_500_range = 'M3:M1865'; %range of the cells for the y (angle) values
sample_rate_500 = 1873-3+1;

x_500 = xlsread('Datos micrófono.xlsx', 'Hoja1', x_500_range);
y_500 = xlsread('Datos micrófono.xlsx', 'Hoja1', y_500_range);

T_100 = 1/100*1000;
T_200 = 1/200*1000;
T_500 = 1/500*1000;

sample_100 = 50e3;
sample_200 = 50e3;
sample_500 = 100e3;

%PLOTS 

plot(x_100, y_100, 'r'); %not filtered
%xlim([0 (2*T_100)]);
figure(2);
plot(x_200, y_200, 'g'); %not filtered
%xlim([0 (2*T_200)]);
figure(3);
plot(x_500, y_500, 'b'); %not filtered
%xlim([0 (2*T_500)]);

%Filter

BandFilt_100 = designfilt('bandpassiir','FilterOrder',20, ...          
'HalfPowerFrequency1', 20, 'HalfPowerFrequency2', 20e3, ...          
'SampleRate',sample_100); 
y_100_filtered = filter(BandFilt_100,y_100);
BandFilt_200 = designfilt('bandpassiir','FilterOrder',20, ...          
'HalfPowerFrequency1', 20, 'HalfPowerFrequency2', 20e3, ...          
'SampleRate',sample_200); 
y_200_filtered = filter(BandFilt_200,y_200);
BandFilt_500 = designfilt('bandpassiir','FilterOrder',20, ...          
'HalfPowerFrequency1', 20, 'HalfPowerFrequency2', 20e3, ...          
'SampleRate',sample_500); 
y_500_filtered = filter(BandFilt_500,y_500);

x_100_filtered = 0:1:1870;
x_200_filtered = 0:1:1862; 
x_500_filtered = 0:1:1862; 

%PLOT FILTERED

figure(4);
plot(x_100_filtered, y_100_filtered, 'r'); %not filtered
%xlim([0 (2*T_100)]);
figure(5);
plot(x_200_filtered, y_200_filtered, 'g'); %not filtered
%xlim([0 (2*T_200)]);
figure(6);
plot(x_500_filtered, y_500_filtered, 'b'); %not filtered
%xlim([0 (2*T_500)]);
