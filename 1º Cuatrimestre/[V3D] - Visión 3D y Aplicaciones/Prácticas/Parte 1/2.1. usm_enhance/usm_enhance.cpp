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
    "{help h usage ? |      | print this message.}"
    "{i interactive  |      | Interactive mode.}"
    "{r radius       |1     | Window's radius. Default 1.}"
    "{g gain         |1.0   | Enhance's gain. Default 1.0}"
    "{f filter       |0     | Filter type: 0->Box, 1->Gaussian. Default 0.}"
    "{@input         |<none>| input image.}"
    "{@output        |<none>| output image.}"
    ;


int
main (int argc, char* const* argv)
{
    int retCode=EXIT_SUCCESS;

    try {
            cv::CommandLineParser parser(argc, argv, keys);
            parser.about("Apply an unsharp mask enhance to an image.");
            if (parser.has("help"))
            {
                parser.printMessage();
                return EXIT_SUCCESS;
            }

            bool interactive_mode = parser.has("i");

            float radius = 1.0;
            float gain = 1.0;
            int filter = 0;

            radius = parser.get<float>("r");
            gain = parser.get<float>("g");
            filter = parser.get<int>("f"); // 0->Box, 1->Gaussian. Default 0

            
            if (radius<0 || radius>100){
                std::cerr << "Error: r is out of range [0, 5]." << std::endl;
                return EXIT_FAILURE;
            }   
            

            /*if (p<0 || p>100){
                std::cerr << "Error: p is out of range [0, 100]." << std::endl;
                return EXIT_FAILURE;
            }*/

            cv::String input_n = parser.get<cv::String>("@input");
            cv::String output_n = parser.get<cv::String>("@output");

            if (!parser.check())
            {
                parser.printErrors();
                return EXIT_FAILURE;
            }

            cv::Mat unsharp_mask;
            cv::Mat outputImage;

            // Cargar la imagen de entrada
            cv::Mat inputImage = cv::imread(input_n);

            // outputImage = inputImage;

            if (inputImage.empty())
            {
                std::cerr << "Error: could not open input image." << std::endl;
                return EXIT_FAILURE;
            }

            // Convertir la imagen a flotante [0, 1]
            inputImage.convertTo(inputImage, CV_32FC1, 1.0 / 255.0);

            outputImage = inputImage.clone();

            //TODO: use your function fsiv_usm_enhance()
            //Remember: if the input is in RGB you we want only process the "luma".
            //Hint: use HSV color space to this. See cv::cvtColor.
            //Remember: save the unsharp mask in "unsharp_mask" variable.

            // Modo interactivo
            if (interactive_mode) {
                std::cout << "Modo interactivo";

                //radius = radius * 100;
                //gain = gain * 100;

                // Configurar las barras deslizantes
                cv::namedWindow("Interactive Mode", cv::WINDOW_GUI_EXPANDED);
                cv::createTrackbar("Radius", "Interactive Mode", nullptr, 100, nullptr);
                cv::createTrackbar("Gain", "Interactive Mode", nullptr, 200, nullptr);
                cv::createTrackbar("Filter", "Interactive Mode", nullptr, 1, nullptr);
                cv::createTrackbar("RGB", "Interactive Mode", nullptr, 1, nullptr);

                std::vector<cv::Mat> channels;
                cv::split(inputImage, channels);

                int num_channels = channels.size();
                num_channels = inputImage.channels();

                // Loop de actualización de la imagen basado en las barras deslizantes

                cv::Mat grayscaleImage;

                while (true) {
                    // Escalar los valores de las barras
                    int radius = cv::getTrackbarPos("Radius", "Interactive Mode");
                    float gain = cv::getTrackbarPos("Gain", "Interactive Mode") / 100.0;
                    int filter = cv::getTrackbarPos("Filter", "Interactive Mode");
                    int rgb = cv::getTrackbarPos("RGB", "Interactive Mode");

                    if (radius == 0){
                        radius = 1;
                    }

                    if(rgb == 0){ // grayscale
                        cv::cvtColor(inputImage, grayscaleImage, cv::COLOR_BGR2GRAY); 
                        grayscaleImage.convertTo(grayscaleImage, CV_32FC1);
                        outputImage = fsiv_usm_enhance(grayscaleImage, gain, radius, filter, &unsharp_mask);

                        cv::namedWindow("INPUT", cv::WINDOW_GUI_EXPANDED);
                        cv::imshow("INPUT", grayscaleImage);
                    }
                    else if (rgb == 1){    // rgb   
                        cv::Mat hsvImage;
                        cv::cvtColor(inputImage, hsvImage, cv::COLOR_BGR2HSV);

                        // Extraer el canal de luminancia (Value en HSV)
                        std::vector<cv::Mat> channels;
                        cv::split(hsvImage, channels);
                        cv::Mat luma = channels[2];

                        luma.convertTo(luma, CV_32FC1);

                        // Aplicar el procesamiento solo al canal de luminancia
                        cv::Mat lumaChannels = fsiv_usm_enhance(luma, gain, radius, filter, &unsharp_mask);

                        // Recomponer la imagen en el espacio HSV
                        std::vector<cv::Mat> outputChannels;
                        outputChannels.push_back(channels[0]);
                        outputChannels.push_back(channels[1]);
                        outputChannels.push_back(lumaChannels);
                        cv::merge(outputChannels, outputImage);

                        // Deshacer el cambio al espacio BGR
                        cv::cvtColor(outputImage, outputImage, cv::COLOR_HSV2BGR);

                        cv::namedWindow("INPUT", cv::WINDOW_GUI_EXPANDED);
                        cv::imshow("INPUT", inputImage);
                    }
                    else {
                        std::cout << "Error\n";
                    }

                    // Mostrar las imágenes
                    cv::namedWindow("UNSHARP MASK", cv::WINDOW_GUI_EXPANDED);
                    cv::imshow("UNSHARP MASK", unsharp_mask);
                    cv::namedWindow("OUTPUT", cv::WINDOW_GUI_EXPANDED);
                    cv::imshow("OUTPUT", outputImage);

                    // std::cout << "Pulsa ESC para salir." << std::endl;

                    // Espera hasta que se pulse la tecla ESC y luego cierra la ventana
                    int key = cv::waitKey(30);
                    if (key == 27) {
                        break;
                    }
                }
                // Destruye las ventanas al salir
                cv::destroyAllWindows();
            }
            else {
                std::cout << "Modo NO interactivo\n";
                //cv::cvtColor(inputImage, inputImage, cv::COLOR_BGR2GRAY); 
                //inputImage.convertTo(inputImage, CV_32FC1);

                std::vector<cv::Mat> channels;
                cv::split(inputImage, channels);

                int num_channels = channels.size();
                num_channels = inputImage.channels();

                if (num_channels == 1) {
                    std::cout << "Imagen Monocroma\n";
                    outputImage = fsiv_usm_enhance(inputImage, gain, radius, filter, &unsharp_mask);
                }
                else if (num_channels == 3) {
                    std::cout << "Imagen RGB - Procesar solo canal Luma\n";
        
                    cv::Mat hsvImage;
                    cv::cvtColor(inputImage, hsvImage, cv::COLOR_BGR2HSV);

                    // Extraer el canal de luminancia (Value en HSV)
                    std::vector<cv::Mat> channels;
                    cv::split(hsvImage, channels);
                    cv::Mat luma = channels[2];

                    luma.convertTo(luma, CV_32FC1);

                    // Aplicar el procesamiento solo al canal de luminancia
                    cv::Mat lumaChannels = fsiv_usm_enhance(luma, gain, radius, filter, &unsharp_mask);

                    // Recomponer la imagen en el espacio HSV
                    std::vector<cv::Mat> outputChannels;
                    outputChannels.push_back(channels[0]);
                    outputChannels.push_back(channels[1]);
                    outputChannels.push_back(lumaChannels);
                    cv::merge(outputChannels, outputImage);

                    // Deshacer el cambio al espacio BGR
                    cv::cvtColor(outputImage, outputImage, cv::COLOR_HSV2BGR);
                }
                else {
                    std::cout << "Error\n";
                }

                //cv::cvtColor(outputImage, outputImage, cv::COLOR_GRAY2BGR); 

                // outputImage = fsiv_combine_images(inputImage, outputImage, 0.5, 0.5);

                // outputImage = fsiv_combine_images(grayImage, unsharp_mask, 0.5, 0.5);

                // Mostrar las imágenes
                cv::namedWindow("INPUT", cv::WINDOW_GUI_EXPANDED);
                cv::imshow("INPUT", inputImage);
                cv::namedWindow("UNSHARP MASK", cv::WINDOW_GUI_EXPANDED);
                cv::imshow("UNSHARP MASK", unsharp_mask);
                cv::namedWindow("OUTPUT", cv::WINDOW_GUI_EXPANDED);
                cv::imshow("OUTPUT", outputImage);

                std::cout << "Pulsa ESC para salir." << std::endl;

                // Espera hasta que se pulse la tecla ESC y luego cierra la ventana
                while ((cv::waitKey(0) & 0xff) != 27);

                // Destruye las ventanas al salir
                cv::destroyAllWindows();
            }

            int k = cv::waitKey(0)&0xff;
            if (k!=27)
            {
                // Guardar la imagen procesada
                cv::imwrite(output_n, outputImage);
                std::cout << "Image processed and saved successfully." << std::endl;
            }
        }
    catch (std::exception& e){
        std::cerr << "Capturada excepcion: " << e.what() << std::endl;
        retCode = EXIT_FAILURE;
    }
    return retCode;
}
