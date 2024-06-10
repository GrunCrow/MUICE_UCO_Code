clear
figure(1)

% Variables
r=[];
v=[];
F=[];
x=[];
y=[];

% Parámetros
g = [0;-9.81]; % Aceleración debido a la gravedad (m/s^2)
m = 1; % Masa del proyectil (kg)
h=0.01;

% Condiciones iniciales
r0=[0;0];
v0=[1;4];

% Variable externa
F = m * g;

% Inicialización de variables
r = r0;
v = v0;

% Integración numérica usando el método de Euler
for step=1:100
    plot(r(1),r(2),'ob');
    title(['paso :' num2str(step)]);
    axis([0 2 -1 1]);
    set(gca,'dataAspectRatio',[1 1 1]);
    pause(0.01);

    x=[x,r(1)];
    y=[y,r(2)];

    % Guardar valor anterior
    ra = r;
    va = v;

    % Paso integración
    vpm = va + (h/2)*(F/m);
    rpm = ra + (h/2)*va;

    v = va + h*(F/m);
    r = ra + h*vpm;
end

% Graficar la trayectoria
figure(2);
plot(x, y,'g');
axis([0 2 -1 1]);
set(gca,'dataAspectRatio',[1 1 1]);
