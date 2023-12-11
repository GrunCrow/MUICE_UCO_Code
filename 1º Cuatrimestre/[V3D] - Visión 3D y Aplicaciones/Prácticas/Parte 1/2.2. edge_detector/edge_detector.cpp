#include <iostream>
#include <exception>

//Includes para OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include "common_code.hpp"

const char * keys =
    "{help h usage ? |      | print this message   }"
    "{i              |      | Activate interactive mode.}"
    "{s_ap           | 1    | Sobel kernel aperture radio: 0, 1, 2, 3}"
    "{n_bins         | 100  | Gradient histogram size}"
    "{g_r            | 1    | radius of gaussian filter (2r+1). Value 0 means don't filter.}"
    "{th             | 0.8  | Gradient percentile used as threshold for the gradient percentile detector (th2 for canny).}"
    "{th1            | 0.2  | Gradient percentile used as th1 threshold for the Canny detector (th1 < th).}"
    "{m method       | 0    | Detector used: 0:percentile detector, 1:Otsu detector, 2:canny detector}"
    "{c consensus    | 0    | If a ground truth was given, use greater to c% consensus to generate ground truth.}"
    "{@input         |<none>| input image.}"
    "{@output        |<none>| output image.}"
    "{@ground_truth  |      | optional ground truth image to compute the detector metrics.}"
    ;



int
main (int argc, char* const* argv)
{
  int retCode=EXIT_SUCCESS;
  
  try {    

      cv::CommandLineParser parser(argc, argv, keys);
      parser.about("Edge detector v0.0");
      if (parser.has("help"))
      {
          parser.printMessage();
          return 0;
      }

      cv::String input_fname = parser.get<cv::String>("@input");  
      cv::String output_fname = parser.get<cv::String>("@output");
       
      cv::String gt_fname = parser.get<cv::String>("@ground_truth");  // optional ground truth image to compute the detector metrics
      int n_bins = parser.get<int>("n_bins");     // Gradient histogram size
      int g_r = parser.get<int>("g_r");           // radius of gaussian filter (2r+1). Value 0 means don't filter
      float th2 = parser.get<float>("th");        // Gradient percentile used as threshold for the gradient percentile detector (th2 for canny)
      float th1 = parser.get<float>("th1");       // Gradient percentile used as th1 threshold for the Canny detector (th1 < th).
      int s_ap = parser.get<int>("s_ap");         // Sobel kernel aperture radio: 0, 1, 2, 3
      int method = parser.get<int>("method");     // Detector used: 0:percentile detector, 1:Otsu detector, 2:canny detector
      float consensus = parser.get<float>("c");   // If a ground truth was given, use greater to c% consensus to generate ground truth.
      bool interactive = parser.has("i");         // Activate interactive mode

      if (!parser.check())
      {
          parser.printErrors();
          return 0;
      }

    cv::Mat inputImage = cv::imread(input_fname, cv::IMREAD_GRAYSCALE);  

    cv::Mat gt_img;
    if (gt_fname != "")
      gt_img = cv::imread(gt_fname, cv::IMREAD_GRAYSCALE);

    // TODO
    // Remember: you must threshold gt image (>consensus) to get a 0/255 ground-truth.

    if (inputImage.empty())
    {
        std::cerr << "Error: could not open input image." << std::endl;
        return EXIT_FAILURE;
    }

    cv::Mat originalImage = inputImage.clone();

    cv::Mat dx, dy, gradient, edges; 

    // Modo interactivo
    if (interactive) {
        std::cout << "Modo interactivo";

        int initial_n_bins = 100;
        int initial_th1 = 0;
        int initial_th2 = 0;

        // Configurar las barras deslizantes
        cv::namedWindow("Interactive Mode", cv::WINDOW_AUTOSIZE);
        cv::createTrackbar("Sobel Aperture", "Interactive Mode", nullptr, 3, nullptr);
        cv::createTrackbar("N_bins", "Interactive Mode", &initial_n_bins, 100, nullptr);
        cv::createTrackbar("Gaussian Filter Radius", "Interactive Mode", nullptr, 20, nullptr);
        cv::createTrackbar("Threshold 2", "Interactive Mode", &initial_th2, 100, nullptr); // / 100
        cv::createTrackbar("Threshold 1", "Interactive Mode", &initial_th1, 100, nullptr); // / 100
        cv::createTrackbar("Method", "Interactive Mode", nullptr, 2, nullptr);
        cv::createTrackbar("Consensus", "Interactive Mode", nullptr, 100, nullptr); // /100

        //cv::namedWindow("Interactive Mode", cv::WINDOW_GUI_EXPANDED);
        //cv::imshow("Interactive Mode", inputImage);

        edges = inputImage.clone();

        while (true) {

            inputImage = originalImage.clone();

            // Values
            s_ap = cv::getTrackbarPos("Sobel Aperture", "Interactive Mode");
            n_bins = cv::getTrackbarPos("N_bins", "Interactive Mode");
            g_r = cv::getTrackbarPos("Gaussian Filter Radius", "Interactive Mode");
            th2 = cv::getTrackbarPos("Threshold 2", "Interactive Mode") / 100.0;
            th1 = cv::getTrackbarPos("Threshold 1", "Interactive Mode") / 100.0;
            method = cv::getTrackbarPos("Method", "Interactive Mode");
            consensus = cv::getTrackbarPos("Consensus", "Interactive Mode") / 100.0;
            
            if(n_bins==0)
                n_bins=1;

            if(!(th1<th2)){
                th1=th2-0.01;
                initial_th1 = th1;
                initial_th2 = th2;
            }

            if (s_ap % 2 == 0){
                s_ap++;
            }
                

            if(gt_fname == ""){ // Detector used: 0:percentile detector, 1:Otsu detector, 2:canny detector}"

                fsiv_compute_derivate(inputImage, dx, dy, g_r, s_ap);
                fsiv_compute_gradient_magnitude(dx, dy, gradient);
                cv::normalize(gradient, gradient, 0, 255, cv::NORM_MINMAX);

                cv::namedWindow("Interactive Mode", cv::WINDOW_AUTOSIZE);
                cv::imshow("Interactive Mode", originalImage);

                if (method == 0){ // percentile
                    fsiv_percentile_edge_detector(gradient, edges, th2, n_bins);
                }
                else if (method == 1){ // otsu
                    fsiv_otsu_edge_detector(gradient, edges);
                }
                else if (method == 2){
                    fsiv_canny_edge_detector(dx, dy, edges, th1, th2, n_bins);
                }
                else{
                    edges = inputImage.clone();
                }
            }
            else{ // If GT -> Show parameters and gt img
                //std::cout << std::endl << "Ground Truth" << std::endl;
                cv::Mat confusionMatrix;
                
                float sensitivity = 0.0;
                float precision = 0.0;
                float F1 = 0.0;

                //cv::namedWindow("InputImage", cv::WINDOW_GUI_EXPANDED);
                //cv::imshow("InputImage", originalImage);

                // Threshold
                cv::Mat gt_img_th;
                int thresholdValue = static_cast<int>(consensus * 255);
                cv::threshold(gt_img, gt_img_th, thresholdValue, 255, cv::THRESH_BINARY);
                //cv::threshold(gt_img, gt_img, consensus, 255, cv::THRESH_BINARY);

                //cv::namedWindow("GroundTruth", cv::WINDOW_GUI_EXPANDED);
                //cv::imshow("GroundTruth", gt_img);

                fsiv_compute_derivate(inputImage, dx, dy, g_r, s_ap);
                fsiv_compute_gradient_magnitude(dx, dy, gradient);
                cv::normalize(gradient, gradient, 0, 255, cv::NORM_MINMAX);

                if (method == 0){ // percentile
                    // Edge Detector
                    fsiv_percentile_edge_detector(gradient, edges, th2, n_bins);
                }
                else if (method == 1){ // otsu

                    fsiv_otsu_edge_detector(gradient, edges);
                }
                else if (method == 2){
                    fsiv_canny_edge_detector(dx, dy, edges, th1, th2, n_bins);
                }        

                // Confussion Matrix
                fsiv_compute_confusion_matrix(gt_img_th, edges, confusionMatrix);

                // Metrics Calculation
                sensitivity = fsiv_compute_sensitivity(confusionMatrix);
                precision = fsiv_compute_precision(confusionMatrix);
                F1 = fsiv_compute_F1_score(confusionMatrix);

                // Metrics Window
                cv::namedWindow("Metrics", cv::WINDOW_AUTOSIZE);
                cv::Mat metricsImage = cv::Mat::zeros(150, 400, CV_8UC3);  // Ajusta el tamaño según sea necesario
                cv::putText(metricsImage, "Sensitivity: " + std::to_string(sensitivity), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
                cv::putText(metricsImage, "Precision: " + std::to_string(precision), cv::Point(10, 60), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
                cv::putText(metricsImage, "F1: " + std::to_string(F1), cv::Point(10, 90), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
                cv::imshow("Metrics", metricsImage); 
            }

            cv::namedWindow("Interactive Mode", cv::WINDOW_AUTOSIZE);
            cv::imshow("Interactive Mode", edges);

            // Espera hasta que se pulse la tecla ESC y luego cierra la ventana
            int key = cv::waitKey(30);
            if (key == 27) {
                // Guardar la imagen procesada
                cv::imwrite(output_fname, edges);
                std::cout << "Image processed and saved successfully." << std::endl;

            }
        }
        // Destruye las ventanas al salir
        cv::destroyAllWindows();
    }
    else {
        std::cout << "Modo NO interactivo\n";

        if(gt_fname == ""){ // Detector used: 0:percentile detector, 1:Otsu detector, 2:canny detector}"

            fsiv_compute_derivate(inputImage, dx, dy, g_r, s_ap);
            fsiv_compute_gradient_magnitude(dx, dy, gradient);
            cv::normalize(gradient, gradient, 0, 255, cv::NORM_MINMAX);

            cv::namedWindow("InputImage", cv::WINDOW_AUTOSIZE);
            cv::imshow("InputImage", originalImage);

            if (method == 0){ // percentile
                //fsiv_compute_gradient_magnitude(dx, dy, gradient);
                //cv::normalize(gradient, gradient, 0, 255, cv::NORM_MINMAX);
                
                fsiv_percentile_edge_detector(gradient, edges, th2, n_bins);

                cv::namedWindow("Percentile", cv::WINDOW_AUTOSIZE);
                cv::imshow("Percentile", edges);
            }
            else if (method == 1){ // otsu
                //fsiv_compute_gradient_magnitude(dx, dy, gradient);
                //cv::normalize(gradient, gradient, 0, 255, cv::NORM_MINMAX);

                fsiv_otsu_edge_detector(gradient, edges);

                cv::namedWindow("Otsu", cv::WINDOW_AUTOSIZE);
                cv::imshow("Otsu", edges);
            }
            else if (method == 2){
                fsiv_canny_edge_detector(dx, dy, edges, th1, th2, n_bins);

                cv::namedWindow("Canny", cv::WINDOW_AUTOSIZE);
                cv::imshow("Canny", edges);
            }
            else{
                cv::Mat percentileImage, otsuImage, cannnyImage;
                fsiv_percentile_edge_detector(gradient, percentileImage, th2, n_bins);

                cv::namedWindow("Percentile", cv::WINDOW_AUTOSIZE);
                cv::imshow("Percentile", percentileImage);

                fsiv_otsu_edge_detector(gradient, otsuImage);

                cv::namedWindow("Otsu", cv::WINDOW_AUTOSIZE);
                cv::imshow("Otsu", otsuImage);

                fsiv_canny_edge_detector(dx, dy, cannnyImage, th1, th2, n_bins);

                cv::namedWindow("Canny", cv::WINDOW_AUTOSIZE);
                cv::imshow("Canny", cannnyImage);
            }
        }
        else{ // If GT -> Show parameters and gt img
            std::cout << std::endl << "Ground Truth" << std::endl;
            cv::Mat confusionMatrix;

            cv::namedWindow("InputImage", cv::WINDOW_AUTOSIZE);
            cv::imshow("InputImage", originalImage);

            // Threshold
            int thresholdValue = static_cast<int>(consensus * 255);
            cv::threshold(gt_img, gt_img, thresholdValue, 255, cv::THRESH_BINARY);
            //cv::threshold(gt_img, gt_img, consensus, 255, cv::THRESH_BINARY);

            cv::namedWindow("GroundTruth", cv::WINDOW_AUTOSIZE);
            cv::imshow("GroundTruth", gt_img);

            fsiv_compute_derivate(inputImage, dx, dy, g_r, s_ap);
            fsiv_compute_gradient_magnitude(dx, dy, gradient);
            cv::normalize(gradient, gradient, 0, 255, cv::NORM_MINMAX);

            /*
            gradient.convertTo(gradient, CV_8U);
            cv::namedWindow("Gradient", cv::WINDOW_GUI_EXPANDED);
            cv::imshow("Gradient", gradient);*/

            if (method == 0){ // percentile
                // Edge Detector
                fsiv_percentile_edge_detector(gradient, edges, th2, n_bins);

                // Confussion Matrix
                fsiv_compute_confusion_matrix(gt_img, edges, confusionMatrix);

                // Metrics Calculation
                float sensitivity = fsiv_compute_sensitivity(confusionMatrix);
                float precision = fsiv_compute_precision(confusionMatrix);
                float F1 = fsiv_compute_F1_score(confusionMatrix);

                cv::namedWindow("Percentile", cv::WINDOW_AUTOSIZE);
                cv::imshow("Percentile", edges);

                // Metrics Widow
                cv::namedWindow("Percentile Metrics", cv::WINDOW_AUTOSIZE);
                cv::Mat metricsImage = cv::Mat::zeros(150, 400, CV_8UC3);  // Ajusta el tamaño según sea necesario
                cv::putText(metricsImage, "Sensitivity: " + std::to_string(sensitivity), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
                cv::putText(metricsImage, "Precision: " + std::to_string(precision), cv::Point(10, 60), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
                cv::putText(metricsImage, "F1: " + std::to_string(F1), cv::Point(10, 90), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
                cv::imshow("Percentile Metrics", metricsImage);

            }
            else if (method == 1){ // otsu

                fsiv_otsu_edge_detector(gradient, edges);

                // Confussion Matrix
                fsiv_compute_confusion_matrix(gt_img, edges, confusionMatrix);

                // Metrics Calculation
                float sensitivity = fsiv_compute_sensitivity(confusionMatrix);
                float precision = fsiv_compute_precision(confusionMatrix);
                float F1 = fsiv_compute_F1_score(confusionMatrix);

                cv::namedWindow("Otsu", cv::WINDOW_AUTOSIZE);
                cv::imshow("Otsu", edges);

                // Metrics WWindow
                cv::namedWindow("Otsu Metrics", cv::WINDOW_AUTOSIZE);
                cv::Mat metricsImage = cv::Mat::zeros(150, 400, CV_8UC3);  // Ajusta el tamaño según sea necesario
                cv::putText(metricsImage, "Sensitivity: " + std::to_string(sensitivity), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
                cv::putText(metricsImage, "Precision: " + std::to_string(precision), cv::Point(10, 60), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
                cv::putText(metricsImage, "F1: " + std::to_string(F1), cv::Point(10, 90), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
                cv::imshow("Percentile Metrics", metricsImage);

            }
            else if (method == 2){
                fsiv_canny_edge_detector(dx, dy, edges, th1, th2, n_bins);

                // Confussion Matrix
                fsiv_compute_confusion_matrix(gt_img, edges, confusionMatrix);

                // Metrics Calculation
                float sensitivity = fsiv_compute_sensitivity(confusionMatrix);
                float precision = fsiv_compute_precision(confusionMatrix);
                float F1 = fsiv_compute_F1_score(confusionMatrix);

                cv::namedWindow("Canny", cv::WINDOW_AUTOSIZE);
                cv::imshow("Canny", edges);

                // Metrics WWindow
                cv::namedWindow("Canny Metrics", cv::WINDOW_AUTOSIZE);
                cv::Mat metricsImage = cv::Mat::zeros(150, 400, CV_8UC3);  // Ajusta el tamaño según sea necesario
                cv::putText(metricsImage, "Sensitivity: " + std::to_string(sensitivity), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
                cv::putText(metricsImage, "Precision: " + std::to_string(precision), cv::Point(10, 60), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
                cv::putText(metricsImage, "F1: " + std::to_string(F1), cv::Point(10, 90), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
                cv::imshow("Percentile Metrics", metricsImage);

            }
            else{
                cv::Mat percentileImage, otsuImage, cannyImage;
                fsiv_percentile_edge_detector(gradient, percentileImage, th2, n_bins);

                cv::namedWindow("Percentile", cv::WINDOW_AUTOSIZE);
                cv::imshow("Percentile", percentileImage);

                // Confussion Matrix
                fsiv_compute_confusion_matrix(gt_img, percentileImage, confusionMatrix);

                // Metrics Calculation
                float sensitivity = fsiv_compute_sensitivity(confusionMatrix);
                float precision = fsiv_compute_precision(confusionMatrix);
                float F1 = fsiv_compute_F1_score(confusionMatrix);

                // Metrics WWindow
                cv::namedWindow("Percentile Metrics", cv::WINDOW_AUTOSIZE);
                cv::Mat metricsImage = cv::Mat::zeros(150, 400, CV_8UC3);  // Ajusta el tamaño según sea necesario
                cv::putText(metricsImage, "Sensitivity: " + std::to_string(sensitivity), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
                cv::putText(metricsImage, "Precision: " + std::to_string(precision), cv::Point(10, 60), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
                cv::putText(metricsImage, "F1: " + std::to_string(F1), cv::Point(10, 90), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
                cv::imshow("Percentile Metrics", metricsImage);

                fsiv_otsu_edge_detector(gradient, otsuImage);

                cv::namedWindow("Otsu", cv::WINDOW_AUTOSIZE);
                cv::imshow("Otsu", otsuImage);

                // Confussion Matrix
                fsiv_compute_confusion_matrix(gt_img, otsuImage, confusionMatrix);

                // Metrics Calculation
                sensitivity = fsiv_compute_sensitivity(confusionMatrix);
                precision = fsiv_compute_precision(confusionMatrix);
                F1 = fsiv_compute_F1_score(confusionMatrix);

                // Metrics WWindow
                cv::namedWindow("Otsu Metrics", cv::WINDOW_AUTOSIZE);
                metricsImage = cv::Mat::zeros(150, 400, CV_8UC3);  // Ajusta el tamaño según sea necesario
                cv::putText(metricsImage, "Sensitivity: " + std::to_string(sensitivity), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
                cv::putText(metricsImage, "Precision: " + std::to_string(precision), cv::Point(10, 60), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
                cv::putText(metricsImage, "F1: " + std::to_string(F1), cv::Point(10, 90), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
                cv::imshow("Otsu Metrics", metricsImage);

                fsiv_canny_edge_detector(dx, dy, cannyImage, th1, th2, n_bins);

                cv::namedWindow("Canny", cv::WINDOW_AUTOSIZE);
                cv::imshow("Canny", cannyImage);

                // Confussion Matrix
                fsiv_compute_confusion_matrix(gt_img, cannyImage, confusionMatrix);

                // Metrics Calculation
                sensitivity = fsiv_compute_sensitivity(confusionMatrix);
                precision = fsiv_compute_precision(confusionMatrix);
                F1 = fsiv_compute_F1_score(confusionMatrix);

                // Metrics Window
                cv::namedWindow("Canny Metrics", cv::WINDOW_AUTOSIZE);
                metricsImage = cv::Mat::zeros(150, 400, CV_8UC3);  // Ajusta el tamaño según sea necesario
                cv::putText(metricsImage, "Sensitivity: " + std::to_string(sensitivity), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
                cv::putText(metricsImage, "Precision: " + std::to_string(precision), cv::Point(10, 60), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
                cv::putText(metricsImage, "F1: " + std::to_string(F1), cv::Point(10, 90), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
                cv::imshow("Canny Metrics", metricsImage);
            }
          
        }
        std::cout << "Pulsa ESC para salir." << std::endl;

        // Espera hasta que se pulse la tecla ESC y luego cierra la ventana
        int k = cv::waitKey(0)&0xff;
        if (k!=27)
        {
            // Guardar la imagen procesada
            cv::imwrite(output_fname, edges);
            std::cout << "Image processed and saved successfully." << std::endl;
        }

        // Destruye las ventanas al salir
        cv::destroyAllWindows();
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Capturada excepcion: " << e.what() << std::endl;
    retCode = EXIT_FAILURE;
  }
  catch (...)
  {
    std::cerr << "Capturada excepcion desconocida!" << std::endl;
    retCode = EXIT_FAILURE;
  }
  return retCode;
}
