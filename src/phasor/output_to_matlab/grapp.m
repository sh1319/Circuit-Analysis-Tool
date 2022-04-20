clear all;
close all;
clc;

data=load('first_test.txt');
time = data(:,1);
vin = data(:,2);
vout = data(:,3);

figure(1);clf;
plot(time, vin);
hold on;
plot(time, vout,'r');
grid on;
xlabel('time (s)');
ylabel('VIN(blue), VOUT(red) (V)');
xlim([0 1*10^(-3)]);
data=load('second_test.txt');
time = data(:,1);
vin = data(:,2);
vout = data(:,3);

figure(2);clf;
plot(time, vin);
hold on;
plot(time, vout,'r');
grid on;
xlabel('time (s)');
ylabel('VIN(blue), VOUT(red) (V)');
xlim([0 1*10^(-3)]);
data=load('third_test.txt');
time = data(:,1);
vin = data(:,2);
v1 = data(:,3);
v2 = data(:,4);

figure(3);clf;
plot(time, vin);
hold on;
plot(time, v1,'r');
plot(time, v2);
grid on;
xlabel('time (s)');
ylabel('vin(blue) v1(red) v2 (V)');