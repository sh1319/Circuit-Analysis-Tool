
clear all;
close all;
clc;

data=load('sim2out_rlc.txt');
time = data(:,1);
n001 = data(:,2);
n002 = data(:,3);
n003 = data(:,4);

figure(1);clf;
plot(time, n001);
hold on;
plot(time, n002,'r');
plot(time, n003);
grid on;
xlabel('time (s)');
ylabel('N001(blue), N002(red), N003(yellow) (V)');
%xlim([0 1*10^(-2)]);


figure(2);clf;
hold on;

v1 = data(:,5);
r1 = data(:,6);
l1 = data(:,7);
c1 = data(:,8);

plot(time, v1);
plot(time, r1);
plot(time, l1);
plot(time, c1);
grid on;
xlabel('time');
ylabel('V1, R1, L1, C1');
