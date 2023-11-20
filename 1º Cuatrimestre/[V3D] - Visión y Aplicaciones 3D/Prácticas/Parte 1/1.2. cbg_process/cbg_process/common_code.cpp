using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>


#include "common_code.hpp"

#include <iostream>

// Convierte una imagen con tipo byte a flotante [0,1].
cv::Mat fsiv_convert_image_byte_to_float(const cv::Mat& img, cv::Mat& out)
{
    CV_Assert(img.depth()==CV_8U);

    // Hint: use the cv::Mat method convertTo.
    
    // Convierte la imagen de entrada (tipo byte) a flotante en el rango [0,1]
    img.convertTo(out, CV_32F, 1.0 / 255.0);
    
    CV_Assert(out.rows==img.rows && out.cols==img.cols);
    CV_Assert(out.depth()==CV_32F);
    CV_Assert(img.channels()==out.channels());
    return out;
}

// Convierte una imagen con tipo float [0,1] a byte [0,255].
cv::Mat fsiv_convert_image_float_to_byte(const cv::Mat& img, cv::Mat& out)
{
    CV_Assert(img.depth()==CV_32F);

    // Hint: use the cv::Mat method convertTo.
    
    // Convierte la imagen de entrada (tipo flotante) a byte en el rango [0,255]
    img.convertTo(out, CV_8U, 255.0);

    CV_Assert(out.rows==img.rows && out.cols==img.cols);
    CV_Assert(out.depth()==CV_8U);
    CV_Assert(img.channels()==out.channels());
    return out;
}

// Convierte una imagen en color BGR a HSV.
cv::Mat fsiv_convert_bgr_to_hsv(const cv::Mat& img, cv::Mat& out)
{
    CV_Assert(img.channels()==3);

    // Hint: use the cv::cvtColor.
    // Remember that color images loaded from a file use BGR ordering.
    
    // Convierte la imagen en formato BGR a formato HSV
    cv::cvtColor(img, out, cv::COLOR_BGR2HSV);

    CV_Assert(out.channels()==3);
    return out;
}

// Convierte una imagen en color HSV a BGR.
cv::Mat fsiv_convert_hsv_to_bgr(const cv::Mat& img, cv::Mat& out)
{
    CV_Assert(img.channels()==3);

    // Hint: use the cv::cvtColor.
    // Remember that color images to save to a file use BGR ordering.
    
    // Convierte la imagen en formato HSV a formato BGR
    cv::cvtColor(img, out, cv::COLOR_HSV2BGR);

    CV_Assert(out.channels()==3);
    return out;
}

/*
 * Realiza un control del brillo/contraste/gamma de la imagen.
 *
 * El proceso sería:
 *
 * O = c * I^g + b
 *
 * Si la imagen es RGB y el flag only_luma es true, se utiliza el espacio HSV
 * para procesar sólo el canal V (luma).
 * */

// cv::Mat fsiv_cbg_process (const cv::Mat & img, cv::Mat& out, double contrast=1.0, double brightness=0.0, double gamma=1.0, bool only_luma=true);

cv::Mat fsiv_cbg_process(const cv::Mat& in, cv::Mat& out, double contrast, double brightness, double gamma, bool only_luma)
{
    CV_Assert(in.depth() == CV_8U);

    // Comprobamos el tipo de imagen de entrada (monocroma o en color)
    if (in.channels() == 1) {
        // Monocroma
        cv::Mat floatImg, auxImg;

        fsiv_convert_image_byte_to_float(in, floatImg);  // Convertir la imagen a formato float [0,1]

        // Asegurarse de que todas las matrices tengan el mismo tipo de datos (CV_32F)
        floatImg.convertTo(floatImg, CV_32F);
        auxImg.convertTo(auxImg, CV_32F);

        // Aplicar corrección gamma
        cv::pow(floatImg, gamma, auxImg);

        // Aplicar ajustes de brillo y contraste
        auxImg = contrast * auxImg + brightness;

        // Convertir la imagen resultante a byte [0, 255]
        fsiv_convert_image_float_to_byte(auxImg, out);
    }else if (in.channels() == 3) {
        // Imagen en color
        if (only_luma) {
            cv::Mat floatImg, auxImg;
            cv::Mat hsvImg;

            // Asegurarse de que todas las matrices tengan el mismo tipo de datos (CV_32F)
            floatImg.convertTo(floatImg, CV_32F);
            auxImg.convertTo(auxImg, CV_32F);

            // Matrices con el mismo tipo de datos
            fsiv_convert_image_byte_to_float(in, floatImg);  // Convertir la imagen a formato float [0,1]

            // Convertir la imagen RGB a HSV
            fsiv_convert_bgr_to_hsv(floatImg, hsvImg);

            // Separar los canales HSV
            std::vector<cv::Mat> hsvChannels(3);
            cv::split(hsvImg, hsvChannels);

            cv::Mat lumaChannel = hsvChannels[2];
            lumaChannel.convertTo(lumaChannel, CV_32F);

            // Aplicar corrección gamma
            cv::pow(lumaChannel, gamma, auxImg);

            // Aplicar ajustes de brillo y contraste solo al canal V (luma)
            auxImg = contrast * auxImg + brightness;

            // Asegurarse de que todas las matrices tengan el mismo tipo de datos (CV_32F)
            for (int i = 0; i < 3; i++) {
                hsvChannels[i].convertTo(hsvChannels[i], CV_32F);
            }

            // Actualizar el canal V con los ajustes
            hsvChannels[2] = auxImg;

            // Fusionar los canales HSV en una imagen
            cv::merge(hsvChannels, hsvImg);

            // Convertir la imagen HSV resultante a BGR
            fsiv_convert_hsv_to_bgr(hsvImg, out);

            // Convertir la imagen resultante a byte [0, 255]
            fsiv_convert_image_float_to_byte(out, out);
        } else {
            cv::Mat floatImg, auxImg;

            fsiv_convert_image_byte_to_float(in, floatImg);  // Convertir la imagen a formato float [0,1]

            // Asegurarse de que todas las matrices tengan el mismo tipo de datos (CV_32F)
            floatImg.convertTo(floatImg, CV_32F);
            auxImg.convertTo(auxImg, CV_32F);

            // Aplicar corrección gamma
            cv::pow(floatImg, gamma, auxImg);

            // Aplicar ajustes de brillo y contraste
            auxImg = contrast * auxImg + brightness;

            // Convertir la imagen resultante a byte [0, 255]
            fsiv_convert_image_float_to_byte(auxImg, out);
        }
    }
    
    CV_Assert(out.rows == in.rows && out.cols == in.cols);
    CV_Assert(out.depth() == CV_8U);
    CV_Assert(out.channels() == in.channels());

    return out;
}



