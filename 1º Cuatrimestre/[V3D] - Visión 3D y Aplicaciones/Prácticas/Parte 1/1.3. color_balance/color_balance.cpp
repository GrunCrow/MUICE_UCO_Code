/*!
  Esto es un esqueleto de programa para usar en las prácticas
  de Visión Artificial.

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

#include "common_code.hpp"

const cv::String keys =
    "{help h usage ? |      | print this message   }"
    "{i interactive  |      | use interactive mode.}"
    "{p              |0     | Percentage of brightest points used. Default 0 means use the WhitPatch method. Value 100 means use the GrayWorld method.}"
    "{@input         |<none>| input image.}"
    "{@output        |<none>| output image.}"
    ;

bool mouseClicked = false;
cv::Mat outputImage;

cv::Mat balancefrompixel(cv::Mat const& in, int r, int g, int b)
{
    CV_Assert(in.type() == CV_8UC3);
    cv::Mat out;

    // Convert 'from' RGB values to cv::Scalar
    cv::Scalar fromScalar(b, g, r);

    // Use fsiv_color_rescaling to apply the color balance
    out = fsiv_color_rescaling(in, fromScalar, cv::Scalar(255, 255, 255));

    CV_Assert(out.type() == in.type());
    CV_Assert(out.rows == in.rows && out.cols == in.cols);
    return out;
}


// Función para manejar clics de ratón
void onMouse(int event, int x, int y, int flags, void* userdata) {
    if (event == cv::EVENT_LBUTTONDOWN) {
        cv::Mat* inputImagePtr = static_cast<cv::Mat*>(userdata);

        if (inputImagePtr == nullptr) {
            std::cerr << "Error: Null pointer to inputImage." << std::endl;
            return;
        }

        cv::Mat& inputImage = *inputImagePtr;

        cv::Vec3b color = inputImage.at<cv::Vec3b>(y, x);
        std::cout << "Clicked pixel color (B, G, R): " << static_cast<int>(color[0]) << ", " << static_cast<int>(color[1]) << ", " << static_cast<int>(color[2]) << std::endl;

        // Llamar a la función balancefrompixel con los valores RGB
        outputImage = balancefrompixel(inputImage, color[2], color[1], color[0]);

        // Convertir de nuevo a double después de la actualización
        cv::imshow("Interactive Mode", outputImage);

        // Establecer la bandera indicando que se ha hecho clic
        mouseClicked = true;
    }
}


int main (int argc, char* const* argv)
{
    int retCode=EXIT_SUCCESS;

    try {

        cv::CommandLineParser parser(argc, argv, keys);
        parser.about("Apply a color balance to an image.");
        if (parser.has("help"))
        {
            parser.printMessage();
            return EXIT_SUCCESS;
        }

        bool interactive_mode = parser.has("i");
        int p = parser.get<int>("p");

        if (p<0 || p>100){
            std::cerr << "Error: p is out of range [0, 100]." << std::endl;
            return EXIT_FAILURE;
        }

        cv::String input_n = parser.get<cv::String>("@input");
        cv::String output_n = parser.get<cv::String>("@output");
        
        if (!parser.check()){
            parser.printErrors();
            return EXIT_FAILURE;
        }

        // Cargar la imagen de entrada
        cv::Mat inputImage = cv::imread(input_n);

        if (inputImage.empty()){
            std::cerr << "Error: Couldn't open the file '" << input_n << "'." << std::endl;
            return EXIT_FAILURE;
        }

        outputImage = inputImage;

        // Modo interactivo
        if (interactive_mode) {
            std::cout << "Modo interactivo";

            // Configurar las barras deslizantes
            cv::namedWindow("Interactive Mode", cv::WINDOW_GUI_EXPANDED);
            cv::createTrackbar("White", "Interactive Mode", nullptr, 1, nullptr);
            cv::createTrackbar("Grey", "Interactive Mode", nullptr, 1, nullptr);
            cv::createTrackbar("General", "Interactive Mode", nullptr, 100, nullptr);

            // Set the mouse callback function
            cv::setMouseCallback("Interactive Mode", onMouse, &inputImage);

            // Loop de actualización de la imagen basado en las barras deslizantes
            while (true) {
                // Escalar los valores de las barras deslizantes según tus necesidades
                int white_mode = cv::getTrackbarPos("White", "Interactive Mode");
                int grey_mode = cv::getTrackbarPos("Grey", "Interactive Mode");
                int p = cv::getTrackbarPos("General", "Interactive Mode");

                // Asegurarse de que solo una barra deslizante está activa
                if (white_mode == 1) {
                    //std::cout<<"White";
                    grey_mode = 0;
                    p=0;
                    cv::setTrackbarPos("Grey", "Interactive Mode", 0);
                    cv::setTrackbarPos("General", "Interactive Mode", 0);
                    outputImage = fsiv_wp_color_balance(inputImage);
                    mouseClicked = false;
                }
                if (grey_mode == 1) {
                    //std::cout<<"Grey";
                    white_mode = 0;
                    p=0;
                    cv::setTrackbarPos("White", "Interactive Mode", 0);
                    cv::setTrackbarPos("General", "Interactive Mode", 0);
                    outputImage = fsiv_gw_color_balance(inputImage);
                    mouseClicked = false;
                }
                if (p > 0) {
                    // Avoid p = 100 (Assert error)
                    if (p>=100)
                        p = 99.90;
                    //std::cout<<"General";
                    white_mode = 0;
                    grey_mode = 0;
                    cv::setTrackbarPos("White", "Interactive Mode", 0);
                    cv::setTrackbarPos("Grey", "Interactive Mode", 0);
                    outputImage = fsiv_color_balance(inputImage, p);
                    mouseClicked = false;
                    //std::cout << p << "\n";
                }
                if (white_mode == 0 && grey_mode == 0 && p == 0 && mouseClicked == false){
                    outputImage = inputImage;
                }

                // Convertir de nuevo a double después de la actualización
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
            if (p==0){ //white
                outputImage = fsiv_wp_color_balance(inputImage);
            }
            else if (p==100){ //grey
                outputImage = fsiv_gw_color_balance(inputImage);
            }
            else{ // general
                outputImage = fsiv_color_balance(inputImage, p);
            }

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
        cv::imwrite(output_n, outputImage);
        std::cout << "Image processed and saved successfully." << std::endl;
        
    }
    catch (std::exception& e)
    {
        std::cerr << "Capturada excepcion: " << e.what() << std::endl;
        retCode = EXIT_FAILURE;
    }
    return retCode;
}