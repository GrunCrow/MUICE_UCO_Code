# Mini tutorial para iniciar las prácticas con OpenCV

*© F.J. Madrid Cuevas (fjmadrid@uco.es). Fundamentos de Sistemas Inteligentes en Visión. Grado de Ingeniería Informática. Universidad de Córdoba. España*

## Objetivos

Aprender a instalar y compilar la librería OpenCV en un entorno Linux con distribución Debian/Ubuntu o Windows. Tener una primera visión general de la librería.
Descripción:

Para completar la práctica debes realizar las siguientes tareas:

- Instalar OpenCV usando alguno de los métodos descritos. Lo recomendable es usar Linux con Debian/Ubuntu e instalar con el instalador de paquetes.
- Compila el proyecto usando la herramienta CMake. Estudia el fichero CMakeLists.txt para ver cómo configurar un proyecto con CMake.
- Estudia el código "esqueleto.cc" para ver cómo se crea una aplicación típica para las prácticas y cómo se gestionará la línea de comandos. 
- Ejecuta y estudia el código del programa show_img <input-image> que cargar una imagen (cv::imread), la visualizarla (cv::imshow) y esperar hasta que el usuario pulse alguna tecla (cv::waitKey) para salir.
- Estudia el código "compute_stats.cpp" para ver distintas formas de acceder a la imagen y formas de procesarla. 
- Ejecuta y estudia el programa show_video para cargar un vídeo (o conectar con una cámara) (cv::VideoCapture), y visualizarlo (cv::imshow) esperando 't' msecs entre frames hasta finalizarlo o que el usuario pulse la tecla "ESC" (cv::waitKey).  Además se añadirá gestión del ratón para mostrar los valores rgb del punto de la imagen sobre el que se pulse.
- Utilizando la fuente "esqueleto.cc", crea tu propio programa e intenta hacer un programa que cargue una imagen y imprima los valores máximo/mínimo por canal.

## Enlaces

### Cómo instalar OpenCV

- (Modo recomendado) Instrucciones para su instalación en Linux (usando una distribución Debian/Ubuntu): [https://docs.google.com/document/d/1Nr65cP6MOHBo9bD2Wj7W27Xq-HkXGJYpZu8OcOdaB3w/edit?usp=sharing]
- Instrucciones para su instalación en Windows: [http://docs.opencv.org/3.4.0/d3/d52/tutorial_windows_install.html]
- Instrucciones para su compilación desde los fuentes en Linux (no se recomienda): [http://docs.opencv.org/3.4.0/d7/d9f/tutorial_linux_install.html]

### Cómo editar/compilar/ejecutar las prácticas

- Página con documentación y tutoriales de OpenCV: [http://docs.opencv.org].
- Instrucciones para compilar desde la línea de comandos Linux: [https://docs.google.com/document/d/1H5dwdb_HhmqFjOakdSgOaaTqPvxYanorMb7bBCzQuzY/edit?usp=sharing]
- Instrucciones para compilar con el IDE QtCreator: [https://docs.google.com/document/d/1v3FVDR4x5u3i9E5f71hYvVtO_TTk5FZNnm_aecB-dl8/edit?usp=sharing]
- Instrucciones para compilar con el IDE VSCode con CMake: [https://code.visualstudio.com/docs/cpp/cmake-linux]
- Instrucciones para compilar en sistemas Windows usando Visual Studio: [https://docs.opencv.org/3.4.0/dd/d6e/tutorial_windows_visual_studio_opencv.html]