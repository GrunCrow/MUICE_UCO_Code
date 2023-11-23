#include "common_code.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

/*
Scale the color of an image so an input color is transformed into an output color.
*/
cv::Mat fsiv_color_rescaling(const cv::Mat& in, const cv::Scalar& from, const cv::Scalar& to)
{
    CV_Assert(in.type()==CV_8UC3);
    cv::Mat out;

    // Convert 'from' and 'to' scalars to 3x1 matrices
    cv::Mat fromMat = cv::Mat(from, false);
    cv::Mat toMat = cv::Mat(to, false);

    // Multiply each element of 'in' by the corresponding element in (to/from) - use cv::Mat method mult().
    out = in.mul(toMat / fromMat);

    CV_Assert(out.type()==in.type());
    CV_Assert(out.rows==in.rows && out.cols==in.cols);
    return out;
}

/*
Apply a "white patch" color balance operation to the image.
*/
cv::Mat fsiv_wp_color_balance(cv::Mat const& in)
{
    CV_Assert(in.type() == CV_8UC3);
    cv::Mat out;

    // Convert to GRAY color space to get the illuminance.
    cv::Mat illuminance;
    cv::cvtColor(in, illuminance, cv::COLOR_BGR2GRAY);

    // Use cv::minMaxLoc to locate the brightest pixel.
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(illuminance, &minVal, &maxVal, &minLoc, &maxLoc);

    // Create a scalar for the brightest pixel in BGR format
    cv::Scalar brightestPixel = in.at<cv::Vec3b>(maxLoc);

    // Use the brightest pixel as 'from' and (255, 255, 255) as 'to' in fsiv_color_rescaling
    out = fsiv_color_rescaling(in, brightestPixel, cv::Scalar(255, 255, 255));

    CV_Assert(out.type() == in.type());
    CV_Assert(out.rows == in.rows && out.cols == in.cols);
    return out;
}


/*
Apply a "gray world" color balance operation to the image.
*/
cv::Mat fsiv_gw_color_balance(cv::Mat const& in)
{
    CV_Assert(in.type() == CV_8UC3);
    cv::Mat out;

    // Use cv::mean to compute the mean pixel value.
    cv::Scalar mean = cv::mean(in);

    // Use fsiv_color_rescaling to apply the color balance
    out = fsiv_color_rescaling(in, mean, cv::Scalar(128, 128, 128));

    CV_Assert(out.type() == in.type());
    CV_Assert(out.rows == in.rows && out.cols == in.cols);
    return out;
}


cv::Mat fsiv_color_balance(cv::Mat const& in, float p)
{
    CV_Assert(in.type() == CV_8UC3);
    CV_Assert(0.0f < p && p < 100.0f);
    cv::Mat out;

    // Convertir a espacio de color GRAY para obtener la luminancia.
    cv::Mat gray_img;
    cv::cvtColor(in, gray_img, cv::COLOR_BGR2GRAY);

    // Calcular el histograma de niveles de gris.
    cv::Mat hist;
    int histSize = 256; // Número de bins
    float range[] = { 0, 256 };
    const float* histRange = { range };
    cv::calcHist(&gray_img, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, true, false);

    // Encontrar el valor umbral para p.
    float threshold = (1.0f - p / 100.0f);
    int totalPixels = gray_img.rows * gray_img.cols;
    int percentilThreshold = -1;
    int cumulativeSum = 0;

    for (int i = 0; i < histSize; ++i){
        cumulativeSum += hist.at<float>(i);
        if (cumulativeSum >= totalPixels * threshold)
        {
            percentilThreshold = i;
            break;
        }
    }

    percentilThreshold += 1;

    // Crear una máscara con píxeles por encima del umbral.
    cv::Mat mask = gray_img >= percentilThreshold;

    // Calcular el valor medio de los píxeles por encima del umbral.
    cv::Scalar mean = cv::mean(in, mask);

    // Aplicar balance de color.
    out = fsiv_color_rescaling(in, mean, cv::Scalar(255, 255, 255));

    CV_Assert(out.type() == in.type());
    CV_Assert(out.rows == in.rows && out.cols == in.cols);
    return out;
}