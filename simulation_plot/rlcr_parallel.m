clear all;
close all;
clc;

data=load('sim2out_parparparpar.txt');
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
ylabel('N001(blue), N003(red), N002(yellow) (V)');
xlim([0 1*10^(-2)]);


figure(2);clf;
hold on;

v1 = data(:,5);
r1 = data(:,6);
c1 = data(:,7);
l1 = data(:,8);
r2 = data(:,9);

plot(time, v1, 'b');
plot(time, r1, 'r');
plot(time, c1, 'y');
plot(time, l1, 'm');
plot(time, r2, 'g');
grid on;
xlabel('time');
ylabel('V1(blue), R1(red), C1(yellow), L1(purple), R2(green)');
xlim([0 1*10^(-2)]);
