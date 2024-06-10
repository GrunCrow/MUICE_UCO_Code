clc;
clear;

figure(1)
theta_graf = [];

% Variables
theta=[];
w=[];
pos=[];

% Parámetros
m = 2; % Cambio de masa
g = 9.81;
L = 1.5; % Cambio de longitud del péndulo
C = 0.5; % Constante de fricción viscosa (valor para oscilación subamortiguada)
h = 0.01;

% Condiciones iniciales
theta_0 = 45*(pi/180); % Cambio de ángulo inicial
w_0 = 0; % Velocidad angular inicial

% Inicialización del movimiento
pos = [L*sin(theta_0);-L*cos(theta_0)];

for step = 1:1000
    hold off;
    plot(pos(1), pos(2), 'o', 'MarkerFaceColor', 'b', 'MarkerSize', 10);
    hold on;
    plot([0; pos(1)], [0; pos(2)]);

    title(['Paso: ' num2str(step)]);
    axis([-L L -L 0]);
    set(gca, 'dataAspectRatio', [1 1 1]);
    pause(0.001);

    t = step * 0.001;
    theta = theta_0*sin((sqrt(g/L))*t+pi/2);
    pos = [L*sin(theta);-L*cos(theta)];
    theta_graf = [theta_graf theta];
end

figure(2)
plot(theta_graf, 'b');
title('Evolución del ángulo en función del tiempo (Subamortiguado)');
xlabel('Paso');
ylabel('Ángulo (rad)');
grid on;
