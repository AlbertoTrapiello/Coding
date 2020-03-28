%NUEVO SCRIPT PARA CALCULAR

%Signal with 1 Hz of frequency
x_100_range = 'D3:D1873'; %range of the cells for the x (time) values
y_100_range = 'E3:E1873'; %range of the cells for the y (angle) values

x_100 = xlsread('Datos micrófono.xlsx', 'Hoja1', x_100_range);
y_100 = xlsread('Datos micrófono.xlsx', 'Hoja1', y_100_range);

%Signal with 101 Hz of frequency
x_200_range = 'H3:H1865'; %range of the cells for the x (time) values
y_200_range = 'I3:I1865'; %range of the cells for the y (angle) values

x_200 = xlsread('Datos micrófono.xlsx', 'Hoja1', x_200_range);
y_200 = xlsread('Datos micrófono.xlsx', 'Hoja1', y_200_range);

%Signal with 201 Hz of frequency
x_500_range = 'L3:L1873'; %range of the cells for the x (time) values
y_500_range = 'M3:M1873'; %range of the cells for the y (angle) values

x_500 = xlsread('Datos micrófono.xlsx', 'Hoja1', x_500_range);
y_500 = xlsread('Datos micrófono.xlsx', 'Hoja1', y_500_range);

T_100 = 1/100*1000;
T_200 = 1/200*1000;
T_500 = 1/500*1000;
%PLOTS

plot(x_100, y_100, 'r'); %not filtered
%xlim([0 (2*T_100)]);
figure(2);
plot(x_200, y_200, 'g'); %not filtered
%xlim([0 (2*T_200)]);
figure(3);
plot(x_500, y_500, 'b'); %not filtered
%xlim([0 (2*T_500)]);
