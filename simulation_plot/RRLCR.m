clear all;
close all;
clc;

data=load('sim2out_parpar.txt');
time = data(:,1);
n001 = data(:,2);
n003 = data(:,3);
n002 = data(:,4);

figure(1);clf;
plot(time, n001);
hold on;
plot(time, n003,'r');
plot(time, n002);
grid on;
xlabel('time (s)');
ylabel('N001(blue), N002(red) (V)');
xlim([0 0.5*10^-2]);

