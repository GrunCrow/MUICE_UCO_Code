/*!
  Ejemplo de carga de imagen y su recorrido para calcular estadisticos
  simples.

  Se supone que se utilizará OpenCV.

  Para compilar, puedes ejecutar:
    g++ -Wall -o esqueleto esqueleto.cc `pkg-config opencv --cflags --libs`

*/

#include <iostream>
#include <exception>
#include <valarray>

//Includes para OpenCV, Descomentar según los módulo utilizados.
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/calib3d/calib3d.hpp>

const cv::String keys =
    "{help h usage ? |      | print this message.   }"
    "{@image         |<none>| input image.          }"            
    ;

/*!
    @brief Calcular el valor medio de una imagen y su varianza.
        
    Esta es la forma más intuitiva de recorrer una imagen.
    
    @param[in] img es la imagen de entrada.
    @param[out] media la media de los valores.
    @param[out] dev la desviación estárdar de los valores.
    
    @pre img no está vacia.
    @pre img es de tipo CV_8UC1 (Un sólo canal en formato byte).
*/    
void 
compute_stats1(const cv::Mat& img, float& media, float& dev)
{
    //Comprobacion de precondiciones.
    CV_Assert( !img.empty() );
    CV_Assert( img.type() == CV_8UC1 );
   
    media = 0.0;
    dev = 0.0;
    
    //Para cada fila 0 ... img.rows-1
    for (int row=0; row<img.rows; ++row)
        //Para cada columna 0 ... img.cols-1
        for (int col=0; col<img.cols; ++col)
        {
            //Acceder a un pixel con el metodo at<Tipo de pixel>(fila, columna).
            //Aqui cada pixel es un byte (uchar).

            const float v = img.at<uchar>(row, col);

            //También podríamos tener otros tipos:
            //   Tres bytes por pixel (CV_8UC3) ->  at<cv::Vec3b>
            //   Un solo float por pixel (CV_32FC1)  ->  at<float>
            //   tres floats por pixel (CV_32FC3)   -> at<cv::Vec3f>
            //   ... más combinaciones.


            media += v;
            dev += v*v;
        }

    const float count = img.rows*img.cols;
    
    media /= count;
    dev /= count;
    dev = cv::sqrt(dev - media*media);
}

/*!
    @brief Calcular el valor medio de una imagen y su varianza.

    Esta forma usa iteradores, más fácil de codificar cuando sólo queremos
    procesar todos los pixeles uno a uno.

    @param[in] img es la imagen de entrada.
    @param[out] media la media de los valores.
    @param[out] dev la desviación estárdar de los valores.

    @pre img no está vacia.
    @pre img es de tipo CV_8UC1 (Un sólo canal en formato byte).
*/
void
compute_stats2(const cv::Mat& img, float& media, float& dev)
{
    //Comprobacion de precondiciones.
    CV_Assert( !img.empty() );
    CV_Assert( img.type() == CV_8UC1 );

    media = 0.0;
    dev = 0.0;

    const auto end = img.end<uchar>();
    for (auto p = img.begin<uchar>(); p != end; ++p )
    {
        //También podríamos tener otros tipos:
        //   Tres bytes por pixel (CV_8UC3) ->  begin<cv::Vec3b>()
        //   Un solo float por pixel (CV_32FC1)  ->  begin<float>()
        //   tres floats por pixel (CV_32FC3)   -> begin<cv::Vec3f>()
        //   ... mas combinaciones.

        const float v = *p;
        media += v;
        dev += v*v;
    }

    const float count = img.rows*img.cols;

    media /= count;
    dev /= count;
    dev = cv::sqrt(dev - media*media);
}

/*!
    @brief Calcular el valor medio de una imagen y su varianza.

    Esta forma usa código vectorizado (funciones SIMD). Utilizamos funciones
    de opencv que permiten vectorizar el código.

    @param[in] img es la imagen de entrada.
    @param[out] media la media de los valores.
    @param[out] dev la desviación estárdar de los valores.

    @pre img no está vacia.
    @pre img es de tipo CV_32FC1 (Un sólo canal en formato float).
*/
void
compute_stats3(const cv::Mat& img, float& media, float& dev)
{
    //Comprobacion de precondiciones.
    CV_Assert( !img.empty() );
    CV_Assert( img.depth() == CV_32FC1 );

    media = static_cast<float>(cv::sum(img)[0]);
    dev = static_cast<float>(cv::sum(img.mul(img))[0]);

    const float count = img.rows*img.cols;

    media /= count;
    dev /= count;
    dev = cv::sqrt(dev - media*media);
}

/*!
    @brief Calcular el valor medio de una imagen y su varianza.

    Esta forma usa una función de opencv (no siempre hay una!!).

    @param[in] img es la imagen de entrada.
    @param[out] media la media de los valores.
    @param[out] dev la desviación estárdar de los valores.

    @pre img no está vacia.
    @pre img es de tipo CV_32FC1 (Un sólo canal en formato float).
*/
void
compute_stats4(const cv::Mat& img, float& media, float& dev)
{
    //Comprobacion de precondiciones.
    CV_Assert( !img.empty() );
    CV_Assert( img.type() == CV_32FC1 );

    cv::Scalar mean;
    cv::Scalar stdev;
    cv::meanStdDev(img, mean, stdev);
    media = static_cast<float>(mean[0]);
    dev = static_cast<float>(stdev[0]);
}

int
main (int argc, char* const* argv)
{
  int retCode=EXIT_SUCCESS;
  
  try {    

      cv::CommandLineParser parser(argc, argv, keys);
      parser.about("Calcula los estadísticos de una imagen v1.0.0");

      if (parser.has("help"))
      {
          parser.printMessage();
          return 0;
      }
      cv::String img_name = parser.get<cv::String>("@image");

      if (!parser.check())
      {
          parser.printErrors();
          return 0;
      }


      //Carga la imagen desde archivo.
      //En funcion de como se compilo opencv podra
      //cargar mas o menos formatos graficos.
      //Lee la documentacion de imread para ver mas detalles.
      cv::Mat img = cv::imread(img_name, cv::IMREAD_ANYCOLOR);
      //cv::Mat img = cv::imread(img_name, cv::IMREAD_GRAYSCALE);
      //cv::Mat img = cv::imread(img_name, cv::IMREAD_COLOR);
      
      if (img.empty())
      {
         std::cerr << "Error: no he podido abrir el fichero '" << img_name << "'." << std::endl;
         return EXIT_FAILURE;
      }

      //Vamos a mostrar alguna información sobre la imagen cargada.
      std::cout << "Ancho : " << img.cols << std::endl;
      std::cout << "Alto  : " << img.rows << std::endl;
      std::cout << "Número de canales: " << img.channels() << std::endl;
      std::cout << "Profundidad de bit: ";
      switch (img.depth())
      {
      case CV_8S:
          std::cout << " entero 8 bits con signo." << std::endl;
          break;
      case CV_8U:
          std::cout << " entero 8 bits sin signo." << std::endl;
          break;
      case CV_16S:
          std::cout << " entero 16 bits con signo." << std::endl;
          break;
      case CV_16U:
          std::cout << " entero 16 bits sin signo." << std::endl;
          break;
      case CV_32S:
          std::cout << " entero 32 bits con signo." << std::endl;
          break;
      case CV_32F:
          std::cout << " flotante 32 bits." << std::endl;
          break;
      case CV_64F:
          std::cout << " flotante 64 bits." << std::endl;
          break;
      default:
          std::cout << " otros tipos." << std::endl;
          break;
      }

      std::vector<cv::Mat> canales;

      //De-entrelaza la imagen (si lo está), guardando cada canal
      //como una matriz diferente.
      //la operación contraria es cv::merge()
      cv::split(img, canales);


      for(size_t c = 0; c<canales.size(); ++c)
      {
          std::cout << "Canal " << c << ":" << std::endl;
          float media = 0.0f;
          float dev = 0.0f;
          cv::Mat aux_img;
          cv::TickMeter tick_meter;

          tick_meter.reset();
          tick_meter.start();
          compute_stats1(canales[c], media, dev);
          tick_meter.stop();
          std::cerr << "Usando método 1: " << " media: " << media
                    << " desviación: " << dev << " , "
                    << tick_meter.getTimeMilli() << " ms." << std::endl;
          tick_meter.reset();
          tick_meter.start();
          compute_stats2(canales[c], media, dev);
          tick_meter.stop();
          std::cerr << "Usando método 2: " << " media: " << media
                    << " desviación: " << dev << " , "
                    << tick_meter.getTimeMilli() << " ms." << std::endl;

          canales[c].convertTo(aux_img, CV_32F);

          tick_meter.reset();
          tick_meter.start();
          compute_stats3(aux_img, media, dev);
          tick_meter.stop();
          std::cerr << "Usando método 3: " << " media: " << media
                    << " desviación: " << dev << " , "
                    << tick_meter.getTimeMilli() << " ms." << std::endl;

          tick_meter.reset();
          tick_meter.start();
          compute_stats4(aux_img, media, dev);
          tick_meter.stop();
          std::cerr << "Usando método 4: " << " media: " << media
                    << " desviación: " << dev << " , "
                    << tick_meter.getTimeMilli() << " ms." << std::endl;
      }
  }
  catch (std::exception& e)
  {
    std::cerr << "Capturada excepcion: " << e.what() << std::endl;
    retCode = EXIT_FAILURE;
  }
  return retCode;
}




