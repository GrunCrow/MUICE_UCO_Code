// cbg_process.cpp

#include <iostream>

#include "common_code.hpp"

//Includes para OpenCV, Descomentar según los módulo utilizados.
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//#include <QCoreApplication>
//#include <QThread>
//#include <QTimer>

//Interfaz de línea de comandos.
const char * keys = //const cv::String keys =
    "{help h usage ? |      | print this message.}"
    "{i interactive  |      | Activate interactive mode.}"
    "{l luma         |      | process only \"luma\" if color image.}"
    "{c contrast     |1.0   | contrast parameter.}"
    "{b bright       |0.0   | bright parameter.}"
    "{g gamma        |1.0   | gamma parameter.}"
    "{@input         |<none>| input image.}"
    "{@output        |<none>| output image.}";


// cv::Mat fsiv_cbg_process(const cv::Mat& in, cv::Mat& out, double contrast, double brightness, double gamma, bool only_luma)


void updateImage(const cv::Mat& inputImage, cv::Mat& outputImage, double contrast, double bright, double gamma, bool luma_is_set) {
    //inputImage.copyTo(outputImage);
    fsiv_cbg_process(inputImage, outputImage, contrast, bright, gamma, luma_is_set);
}

int main(int argc, char* const* argv)
{
    try
    {
        // Parsear argumentos de la línea de comandos
        cv::CommandLineParser parser(argc, argv, keys);
        parser.about("Adjust the contrast/bright/gamma parameters of an image. (V 0.0.0)");

        if (parser.has("help"))
        {
            parser.printMessage();
            return 0;
        }

        cv::String input_name = parser.get<cv::String>(0);
        cv::String output_name = parser.get<cv::String>(1);

        auto contrast = parser.get<double>("c");
        auto bright = parser.get<double>("b");
        auto gamma = parser.get<double>("g");
        auto luma_is_set = parser.has("l");
        auto interactive_is_set = parser.has("i");

        if (!parser.check())
        {
            parser.printErrors();
            return 0;
        }

        // Cargar la imagen de entrada
        cv::Mat inputImage = cv::imread(input_name);

        if (inputImage.empty()){
            std::cerr << "Error: Couldn't open the file '" << input_name << "'." << std::endl;
            return EXIT_FAILURE;
        }

        cv::Mat outputImage;

        // Modo interactivo
        if (interactive_is_set) {
            std::cout << "Modo interactivo";
            // Multiplicar por 100 y convertir a int
            // contrast def = 1 [0,2]
            // brightness def = 0 [-1,1]
            // gamma def = 1 [0,2]

            int contrast_int = static_cast<int>(contrast) * 100;
            int bright_int = static_cast<int>(bright) * 100 + 100; // + 100 porque la barra va de 0 a 200 y no de -100 a 100
            int gamma_int = static_cast<int>(gamma) * 100;
            int luma_int = static_cast<int>(luma_is_set);

            // Configurar las barras deslizantes
            cv::namedWindow("Interactive Mode", cv::WINDOW_GUI_EXPANDED);
            cv::createTrackbar("Contrast", "Interactive Mode", &contrast_int, 200);
            cv::createTrackbar("Brightness", "Interactive Mode", &bright_int, 200);
            cv::createTrackbar("Gamma", "Interactive Mode", &gamma_int, 200);
            cv::createTrackbar("Luma", "Interactive Mode", &luma_int, 1);

            // Loop de actualización de la imagen basado en las barras deslizantes
            while (true) {
                // Convertir de nuevo a double después de la actualización
                contrast = static_cast<double>(contrast_int) / 100.0;
                bright = (static_cast<double>(bright_int) - 100) / 100.0;
                gamma = static_cast<double>(gamma_int) / 100.0;
                luma_is_set = static_cast<bool>(luma_int);

                updateImage(inputImage, outputImage, contrast=contrast, bright, gamma, luma_is_set);
                cv::imshow("Interactive Mode", outputImage);

                // Espera hasta que se pulse la tecla ESC para salir
                int key = cv::waitKey(30);
                if (key == 27) {
                    break;
                }
            }

            cv::destroyWindow("Interactive Mode");
        }
        else{   
            std::cout << "Modo NO interactivo";
            fsiv_cbg_process(inputImage, outputImage, contrast, bright, gamma, luma_is_set);
            
            // Crea un hilo para manejar la interfaz gráfica
            //QThread guiThread;
            //guiThread.start();

            // Mueve la ventana a un hilo diferente
            cv::namedWindow("IMG", cv::WINDOW_GUI_EXPANDED);
            cv::imshow("IMG", outputImage);
            std::cout << "Pulsa ESC para salir." << std::endl;

            // Espera hasta que se pulse la tecla ESC y luego cierra la ventana
            while ((cv::waitKey(0) & 0xff) != 27);

            // Detiene el hilo de la interfaz gráfica
            //guiThread.quit();
            //guiThread.wait();
            cv::destroyWindow("IMG");
        
        }

        // Guardar la imagen procesada
        cv::imwrite(output_name, outputImage);
        std::cout << "Image processed and saved successfully." << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
