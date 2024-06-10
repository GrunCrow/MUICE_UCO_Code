clear
figure(1)

% Variables
r = [];
v = [];
F = [];
x = [];
y = [];

% Parámetros modificados
g = [0; -9.81]; % Aceleración debido a la gravedad (m/s^2)
m = 2; % Masa del proyectil (kg)
h = 0.01; % Paso de integración

% Condiciones iniciales modificadas
r0 = [0; 0]; % Posición inicial (m)
v0 = [3; 8]; % Velocidad inicial (m/s)

% Variable externa
F = m * g; % Fuerza

% Inicialización de variables
r = r0;
v = v0;

% Integración numérica usando el método de Euler
for step = 1:200
    plot(r(1), r(2), 'ob');
    title(['Paso: ' num2str(step)]);
    axis([0 5 -1 4]);
    set(gca, 'DataAspectRatio', [1 1 1]);
    pause(0.01);

    x = [x, r(1)];
    y = [y, r(2)];

    % Guardar valor anterior
    ra = r;
    va = v;

    % Paso integración
    vpm = va + (h / 2) * (F / m);
    rpm = ra + (h / 2) * va;

    v = va + h * (F / m);
    r = ra + h * vpm;
end

% Graficar la trayectoria
figure(2);
plot(x, y, 'r');
axis([0 5 -1 4]);
set(gca, 'DataAspectRatio', [1 1 1]);
title('Trayectoria del tiro parabólico (Solución numérica modificada)');
xlabel('Distancia horizontal (m)');
ylabel('Altura (m)');
grid on;
