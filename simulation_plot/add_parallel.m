clear all; close all; clc; 

data=load('add_parallel.txt');
num = data(:,1);
n001 = data(:,2);

p1=polyfit(num,n001,1);
xx=0:0.01:20;
line_1=p1(1)*xx+p1(2);

figure(1);clf;
plot(num, n001,'o');
hold on;
plot(xx,line_1,'b');
legend('data','1st order');
grid on;
xlabel('Number of resistors connected in parallel');
ylabel('Time taken for Simulation (s)');

