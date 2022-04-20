clear all;
close all;
clc;

data=load('sim2out_rcrc.txt');
time = data(:,1);
n001 = data(:,2);
n002 = data(:,3);

figure(1);clf;
plot(time, n001);
hold on;
plot(time, n002,'r');
grid on;
xlabel('time (s)');
ylabel('N001(blue), N002(red) (V)');
xlim([0 0.3*10^(-2)]);


figure(2);clf;
hold on;
r1 = data(:,4);
c1 = data(:,5);
v1 = data(:,6);
plot(time, r1);
plot(time, c1);
plot(time, v1);
grid on;
xlabel('time (s)');
ylabel('R1(blue), C1(red), V1(yellow) (V)');

