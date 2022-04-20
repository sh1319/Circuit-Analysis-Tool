clear all; close all; clc;

data=load('add_series.txt');
num = data(:,1);
n001 = data(:,2);

p2=polyfit(num,n001,2);

xx=0:0.01:20;
line_2=p2(1)*xx.^2+p2(2)*xx+p2(3);

figure(1);clf;
plot(num, n001,'o');
hold on;
plot(xx,line_2,'r');
legend('data','2nd order');
xlabel('Number of resistors connected in series');
ylabel('Time taken for Simulation (s)');

