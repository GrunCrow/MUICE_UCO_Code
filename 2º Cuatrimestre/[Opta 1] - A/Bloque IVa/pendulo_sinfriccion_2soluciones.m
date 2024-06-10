clc;
clear;

figure(1)
theta_graf = [];
theta_graf2 = [];

% Variables
theta=[];
theta2=[];
w=[];
pos=[];
pos2=[];

% Parámetros
m = 1;
g = 9.81;
L = 1; % Longitud del péndulo
C = 0; % Constante de fricción viscosa (sin fricción)
h = 0.01;

% Condiciones iniciales
theta_0 = 5*(pi/180); % Ángulo inicial en radianes
w_0 = 0; % Velocidad angular inicial

% Inicialización del movimiento
theta = theta_0;
theta2 = theta_0;

w = w_0;
pos = [L*sin(theta);-L*cos(theta)];
pos2 = [L*sin(theta2);-L*cos(theta2)];
alpha = -(L*w*C + m*g*sin(theta))/(L*m);

for step = 1:1000
    hold off;
    plot(pos(1), pos(2), 'o', 'MarkerFaceColor', 'b', 'MarkerSize', 10);
    hold on;
    plot(pos2(1), pos2(2), 'o', 'MarkerFaceColor', 'r', 'MarkerSize', 10);
    plot([0; pos(1)], [0; pos(2)]);
    plot([0; pos2(1)], [0; pos2(2)]);

    title(['Paso: ' num2str(step)]);
    axis([-L L -L 0]);
    set(gca, 'dataAspectRatio', [1 1 1]);
    pause(0.001);

    theta_a = theta;
    wa = w;

    % Paso Integración
    wpm = wa + (h/2)*alpha;
    theta_pm = theta_a + (h/2)*wa;
    alpha_pm = -(L*wpm*C + m*h*sin(theta_pm))/(L*m);
    
    w = wa + h*alpha_pm;
    theta = theta_a + h*wpm;
    pos = [L*sin(theta);-L*cos(theta)];
    alpha = -(L*w*C + m*g*sin(theta))/(L*m);

    theta_graf = [theta_graf theta];

    t = step * 0.001;
    theta2 = theta_0*sin((sqrt(g/L))*t+pi/2);
    pos2 = [L*sin(theta2);-L*cos(theta2)];
    theta_graf2 = [theta_graf2 theta2];
end

figure(2)
plot(theta_graf, 'b');
hold on;
plot(theta_graf2, 'r');
title('Evolución del ángulo en función del tiempo (Solución numérica y analítica simplificada)');
xlabel('Paso');
ylabel('Ángulo (rad)');
legend('Solución Numérica', 'Solución Analítica Simplificada');
grid on;
