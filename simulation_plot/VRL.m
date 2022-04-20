clear all;
close all;
clc;

data=load('sim2out_vrl.txt');
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



figure(2);clf;
hold on;
v1 = data(:,4);
r1 = data(:,5);
l1 = data(:,6);
plot(time, v1);
plot(time, r1);
plot(time, l1);
grid on;
xlabel('time (s)');
ylabel('V1(blue), R1(red), L1(yellow) (V)');
xlim([0 5*10^(-3)]);

