clear
figure(1)

% Variables
r = [];
F = [];
x = [];
y = [];

% Parámetros modificados
g = [0; -9.81]; % Aceleración debido a la gravedad (m/s^2)
m = 2; % Masa del proyectil (kg)
h = 0.01; % Paso de integración

% Condiciones iniciales modificadas
ro = [0; 0]; % Posición inicial (m)
vo = [3; 8]; % Velocidad inicial (m/s)

% Variable externa
F = m * g; % Fuerza

% Inicializa posición
r=ro;

% Integración numérica usando el método de Euler
for step = 1:200
    plot(r(1), r(2), 'ob');
    title(['Paso: ' num2str(step)]);
    axis([0 5 -1 4]);
    set(gca, 'DataAspectRatio', [1 1 1]);
    pause(0.01);

    x = [x, r(1)];
    y = [y, r(2)];

    % Calculo posición para instante t
    t = step*h;
    r = ro + vo*t+g*t*t/2;
end

% Graficar la trayectoria
figure(2);
plot(x, y, 'r');
axis([0 5 -1 4]);
set(gca, 'DataAspectRatio', [1 1 1]);
title('Trayectoria del tiro parabólico (Solución analítica modificada)');
xlabel('Distancia horizontal (m)');
ylabel('Altura (m)');
grid on;
