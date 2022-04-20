
clear all;
close all;
clc;

data=load('sim2out_rc.txt');
time = data(:,1);
n001 = data(:,2);
n002 = data(:,3);

figure(1);clf;
plot(time, n001);
hold on;
plot(time, n002,'r');
grid on;
xlabel('time');
ylabel('N001(blue), N002(red)');
%xlim([0 1*10^(-2)]);


figure(2);clf;
hold on;
r1 = data(:,4);
c1 = data(:,5);
v1 = data(:,6);
plot(time, r1);
plot(time, c1);
plot(time, v1);
grid on;
xlabel('time');
ylabel('R1, C1, I1(yellow)');
