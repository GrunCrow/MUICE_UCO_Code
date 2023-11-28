/*!
  Ejemplo de carga de un video y su visualización.

  Se supone que se utilizará OpenCV.

  Para compilar, puedes ejecutar:
    g++ -Wall -o esqueleto esqueleto.cc `pkg-config opencv --cflags --libs`

*/

#include <iostream>
#include <exception>

//Includes para OpenCV, Descomentar según los módulo utilizados.
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/calib3d/calib3d.hpp>

const cv::String keys =
    "{help h usage ? |      | print this message.   }"
    "{w wait         |67    | number of msecs to wait between frames.}"
    "{camera c       |-1    | open camera index.}"
    "{video v        |      | open video source.}"
    ;

/**
 * @brief Función callback para gestión del ratón.
 * @param event Qué ocurrió.
 * @param x coordenada x del cursor del ratón.
 * @param y coordenada y del cursor del ratón.
 * @param flags estado del teclado.
 * @param userdata datos que el usuario ha pasado al crear el callback.
 */
void on_mouse(int event, int x, int y, int flags, void *userdata)
{
    if (event == cv::EVENT_LBUTTONDOWN)
    {
        static_cast<int*>(userdata)[0] = x;
        static_cast<int*>(userdata)[1] = y;
    }
}

int
main (int argc, char* const* argv)
{
  int retCode=EXIT_SUCCESS;
  
  try {    

      cv::CommandLineParser parser(argc, argv, keys);
      parser.about("Muestra un video. Pulsa con el raton en un punto para ver "
                   "los valores RGB. v1.0.0");
      if (parser.has("help"))
      {
          parser.printMessage();
          return 0;
      }
      int wait = parser.get<int>("w");      
      int camera_idx = parser.get<int>("camera");
      std::string video_name = parser.get<std::string>("video");

      if (!parser.check())
      {
          parser.printErrors();
          return 0;
      }

      cv::VideoCapture vid;
      if (parser.has("video"))
          vid.open(video_name);
      else
         //Para abrir un dispositvo opencv usa indices enteros.
         //para identificarlos. Usa -1 para abrir el primero disponible.
         vid.open(camera_idx);

      
      if (!vid.isOpened())
      {
         std::cerr << "Error: no he podido abrir el la fuente de vídeo." << std::endl;
         return EXIT_FAILURE;
      }
        
      //Creo la ventana grafica para visualizar la imagen.
      //El nombre de la ventana sirve como 'handle' para gestionarla despues.
      //Lee la documentacon de namedWindow para mas detalles.
      cv::namedWindow("VIDEO");
      
      cv::Mat frame;

      //Captura el primer frame.
      //Si el frame esta vacio, puede ser un error hardware o fin del video.
      vid >> frame;

      if (frame.empty())
      {
          std::cerr << "Error: could not capture any frame from source." << std::endl;
          return EXIT_FAILURE;
      }

      std::cout << "Input size (WxH): " << frame.cols << 'x' << frame.rows << std::endl;
      std::cout << "Frame rate (fps): " << vid.get(cv::CAP_PROP_FPS) << std::endl;
      std::cout << "Num of frames   : " << vid.get(cv::CAP_PROP_FRAME_COUNT) << std::endl;

      //Coordenadas del pixel a muestrear.
      //Inicialmente muestrearemos el pixel central.
      int coords[2] = {frame.cols/2, frame.rows/2};


      //Creamos la ventana para mostrar el video y
      //le conectamos una función "callback" para gestionar el raton.
      cv::namedWindow("VIDEO");
      cv::setMouseCallback ("VIDEO", on_mouse, coords);
      std::cerr << "Pulsa una tecla para continuar (ESC para salir)." << std::endl;
      int key = cv::waitKey(0) & 0xff;
      
      //Muestro frames hasta fin del video (frame vacio),
      //o que el usario pulse la tecla ESCAPE (codigo ascci 27)
      while (!frame.empty() && key!=27)
      {
         //muestro el frame.
         cv::imshow("VIDEO", frame);
         
         //mostramos los valores RGB del pixel muestreado.
         const cv::Vec3b v = frame.at<cv::Vec3b>(coords[1], coords[0]);
         std::cout << "RGB point (" << coords[0] << ',' << coords[1] << "): "
                   << static_cast<int>(v[0]) << ", "
                   << static_cast<int>(v[1]) << ", "
                   << static_cast<int>(v[2]) << std::endl;

         //Espero un tiempo fijado. Si el usuario pulsa una tecla obtengo
         //el codigo ascci. Si pasa el tiempo, retorna -1.
         key = cv::waitKey(wait) & 0xff;
         
         //capturo el siguiente frame.
         vid >> frame;
      }
      //Destruir la ventana abierta.
      cv::destroyWindow("VIDEO");

  }
  catch (std::exception& e)
  {
    std::cerr << "Capturada excepcion: " << e.what() << std::endl;
    retCode = EXIT_FAILURE;
  }
  return retCode;
}
