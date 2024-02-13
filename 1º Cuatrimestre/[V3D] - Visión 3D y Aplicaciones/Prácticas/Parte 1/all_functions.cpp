/*
================================================================================================================================================
                                                                    Práctica 1
================================================================================================================================================
*/

using namespace std;

#include "all_functions.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

void fsiv_find_min_max_loc_1(cv::Mat const& input,
    std::vector<cv::uint8_t>& min_v, std::vector<cv::uint8_t>& max_v,
    std::vector<cv::Point>& min_loc, std::vector<cv::Point>& max_loc)
{
    CV_Assert(input.depth() == CV_8U);

    std::vector<cv::Mat> channels;
    cv::split(input, channels);

    int num_channels = channels.size();

    min_v.resize(num_channels);
    max_v.resize(num_channels);
    min_loc.resize(num_channels);
    max_loc.resize(num_channels);

    // Inicializar los valores mínimos y máximos y ubicaciones
    for (int c = 0; c < num_channels; c++) { // Cambiado a num_channels
        min_v[c] = std::numeric_limits<cv::uint8_t>::max();
        max_v[c] = std::numeric_limits<cv::uint8_t>::min();
        min_loc[c] = cv::Point(-1, -1);
        max_loc[c] = cv::Point(-1, -1);

        for (int y = 0; y < channels[c].rows; y++) { // Acceso a canales[c]
            for (int x = 0; x < channels[c].cols; x++) { // Acceso a canales[c]
                cv::uint8_t pixel_value = channels[c].at<cv::uint8_t>(y, x); // Acceso a canales[c]

                if (pixel_value < min_v[c]) {
                    min_v[c] = pixel_value;
                    min_loc[c] = cv::Point(x, y);
                }
                if (pixel_value > max_v[c]) {
                    max_v[c] = pixel_value;
                    max_loc[c] = cv::Point(x, y);
                }
            }
        }
    }

    
    CV_Assert(num_channels == min_v.size());
    CV_Assert(num_channels == max_v.size());
    CV_Assert(num_channels == min_loc.size());
    CV_Assert(num_channels == max_loc.size());
}



void fsiv_find_min_max_loc_2(cv::Mat const& input,
    std::vector<double>& min_v, std::vector<double>& max_v,
    std::vector<cv::Point>& min_loc, std::vector<cv::Point>& max_loc)
{
    //cv::Mat img = cv::imread(input, cv::IMREAD_ANYCOLOR);
    
    // Split the input image into separate channels
    std::vector<cv::Mat> channels;
    cv::split(input, channels);

    min_v.resize(input.channels());
    max_v.resize(input.channels());
    min_loc.resize(input.channels());
    max_loc.resize(input.channels());   

    // Iterate over each channel to find the min and max values
    for (int c = 0; c < input.channels(); c++) {
        cv::minMaxLoc(channels[c], &min_v[c], &max_v[c], &min_loc[c], &max_loc[c]);
    
        //std::cout << endl << "Canal " << c << ": Valor mínimo = " << static_cast<int>(min_v[c])
        //          << " en (" << min_loc[c].x << ", " << min_loc[c].y << ")" << std::endl;
        //std::cout << endl << "Canal " << c << ": Valor máximo = " << static_cast<int>(max_v[c])
        //          << " en (" << max_loc[c].x << ", " << max_loc[c].y << ")" << std::endl;
    
    }

    CV_Assert(input.channels()==min_v.size());
    CV_Assert(input.channels()==max_v.size());
    CV_Assert(input.channels()==min_loc.size());
    CV_Assert(input.channels()==max_loc.size());

}






/*
================================================================================================================================================
                                                                    Práctica 2
================================================================================================================================================
*/

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










/*
================================================================================================================================================
                                                                    Práctica 3
================================================================================================================================================
*/

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







/*
================================================================================================================================================
                                                                    Práctica 2.1
================================================================================================================================================
*/


cv::Mat fsiv_create_box_filter(const int r)
{
    CV_Assert(r>0);

    cv::Mat ret_v = cv::Mat::ones(2*r + 1, 2*r + 1, CV_32FC1) / static_cast<float>((2*r + 1)*(2*r + 1));

    CV_Assert(ret_v.type()==CV_32FC1);
    CV_Assert(ret_v.rows==(2*r+1) && ret_v.rows==ret_v.cols);
    CV_Assert(std::abs(1.0-cv::sum(ret_v)[0])<1.0e-6);
    return ret_v;
}

cv::Mat fsiv_create_gaussian_filter(const int r){
    CV_Assert(r > 0);
    cv::Mat ret_v;

    // 6*sigma is approx 99,73% of the distribution

    // TODO: Implement Gaussian filter creation
    //double sigma = static_cast<double>(r) / 6.0;
    double sizeWindow = 2*r + 1;
    int rows = sizeWindow;
    int cols = sizeWindow;
    double sigma = (sizeWindow) / 6.0;

    ret_v = cv::Mat(rows, cols, CV_32FC1);

    // aplicar formula de -(x² + y²) ...

    //cv::GaussianBlur(cv::Mat(2 * r + 1, 2 * r + 1, CV_32FC1), ret_v, cv::Size(2 * r + 1, 2 * r + 1), sigma);

    for (int i = -r; i<=r; i++){
        for (int j = -r; j <= r; j++){
            double value = (1.0 / (2.0 * M_PI * sigma * sigma)) * exp(-(i*i + j*j) / (2.0 * sigma * sigma));
            ret_v.at<float>(i+r, j+r) = value;
        }
    }

    // Scale the filter coefficients to sum 1
    cv::normalize(ret_v, ret_v, 1.0, 0.0, cv::NORM_L1);

    CV_Assert(ret_v.type() == CV_32FC1);
    CV_Assert(ret_v.rows == (2 * r + 1) && ret_v.rows == ret_v.cols);
    CV_Assert(std::abs(1.0 - cv::sum(ret_v)[0]) < 1.0e-6);

    return ret_v;
}

cv::Mat fsiv_combine_images(const cv::Mat src1, const cv::Mat src2,
                    double a, double b)
{
    CV_Assert(src1.type()==src2.type());
    CV_Assert(src1.rows==src2.rows);
    CV_Assert(src1.cols==src2.cols);
    cv::Mat ret_v;
    
    cv::addWeighted(src1, a, src2, b, 0, ret_v);

    CV_Assert(ret_v.type()==src2.type());
    CV_Assert(ret_v.rows==src2.rows);
    CV_Assert(ret_v.cols==src2.cols);
    return ret_v;
}

cv::Mat fsiv_usm_enhance(cv::Mat  const& in, double g, int r,
                 int filter_type, cv::Mat *unsharp_mask)
{
    CV_Assert(!in.empty());
    CV_Assert(in.type()==CV_32FC1);
    CV_Assert(r>0);
    CV_Assert(filter_type>=0 && filter_type<=1);
    CV_Assert(g>=0.0);

    cv::Mat ret_v;

    cv::Mat blur;
    if (filter_type == 0) {
        //std::cout<<"Box\n";
        cv::Mat box_filter = fsiv_create_box_filter(r);
        cv::filter2D(in, blur, -1, box_filter);
    } else {
        //std::cout<<"Gausian\n";
        cv::Mat gaussian_filter = fsiv_create_gaussian_filter(r);
        cv::filter2D(in, blur, -1, gaussian_filter);
    }

    ret_v = (1 + g) * in - g * blur;

    if (unsharp_mask != nullptr) {
        // *unsharp_mask = g * (in - blur);
        *unsharp_mask = blur;
        // *unsharp_mask = fsiv_combine_images(in, blur, 0.5, 0.5);
    }

    CV_Assert(ret_v.rows==in.rows);
    CV_Assert(ret_v.cols==in.cols);
    CV_Assert(ret_v.type()==CV_32FC1);
    return ret_v;
}








/*
================================================================================================================================================
                                                                    Práctica 2.2
================================================================================================================================================
*/



void fsiv_compute_derivate(cv::Mat const& img, cv::Mat& dx, cv::Mat& dy, int g_r, int s_ap){
    CV_Assert(img.type()==CV_8UC1);
    
    // if g_r > 0 apply a Gaussian Blur operation with kernel size 2*g_r+1
    if (g_r > 0) {
        cv::GaussianBlur(img, img, cv::Size(2 * g_r + 1, 2 * g_r + 1), 0, 0);
    }

    // use Sobel operator to compute derivate.
    cv::Sobel(img, dx, CV_32FC1, 1, 0, s_ap);
    cv::Sobel(img, dy, CV_32FC1, 0, 1, s_ap);

    CV_Assert(dx.size()==img.size());
    CV_Assert(dy.size()==dx.size());
    CV_Assert(dx.type()==CV_32FC1);
    CV_Assert(dy.type()==CV_32FC1);
}

void fsiv_compute_gradient_magnitude(cv::Mat const& dx, cv::Mat const& dy, cv::Mat& gradient){
    CV_Assert(dx.size()==dy.size());
    CV_Assert(dx.type()==CV_32FC1);
    CV_Assert(dy.type()==CV_32FC1);

    // use cv::magnitude
    cv::magnitude(dx, dy, gradient);

    CV_Assert(gradient.size()==dx.size());
    CV_Assert(gradient.type()==CV_32FC1);
}


void fsiv_compute_gradient_histogram(cv::Mat const& gradient, int n_bins, cv::Mat& hist, float& max_gradient) {    
    // use cv::minMaxLoc to get the gradient range {0, max_gradient}
    double max_val;
    cv::minMaxLoc(gradient, nullptr, &max_val);
    max_gradient = static_cast<float>(max_val);
    max_gradient = max_gradient / 1.0f;

    // std::cout<<max_gradient;

    int histSize[] = {n_bins};
    float range[] = {0, float(max_gradient)};
    const float* histRange[] = {range};

    // Calcular el histograma normalizado
    cv::calcHist(&gradient, 1, 0, cv::Mat(), hist, 1, histSize, histRange, true, false);
    // Remember: use cv::normalize to get a normalized histogram.
    cv::normalize(hist, hist, 1, 0, cv::NORM_L1);

    CV_Assert(max_gradient>0.0);
    CV_Assert(hist.rows == n_bins);
    CV_Assert(cv::abs(cv::sum(hist)[0]-1.0)<1.e-6);
}







int fsiv_compute_histogram_percentile(cv::Mat const& hist, float percentile){
    CV_Assert(percentile >= 0.0 && percentile <= 1.0);
    CV_Assert(hist.type() == CV_32FC1);
    CV_Assert(hist.cols == 1);
    CV_Assert(cv::abs(cv::sum(hist)[0]-1.0)<1.e-6);

    // Remember: The percentile p is the first i that sum{h[0], h[1], ..., h[i]} >= p   

    int idx = 0;
    float cumulative_sum = 0.0;

    //std::cout<<std::endl<<hist.rows<<std::endl;

    while (!(cumulative_sum >= percentile) && idx < hist.rows - 1) {
        cumulative_sum += hist.at<float>(idx);
        idx++;
    }

    if (idx != 0 && idx != hist.rows - 1)
        idx--;

    /*std::cout<<std::endl<<cumulative_sum<<std::endl;
    std::cout<<std::endl<<idx<<std::endl;
    std::cout<<std::endl<<cv::sum(hist(cv::Range(0, idx), cv::Range::all()))[0]<<std::endl;*/

    CV_Assert(idx >= 0 && idx < hist.rows);
    CV_Assert(idx <= 0 || cv::sum(hist(cv::Range(0, idx), cv::Range::all()))[0] <= percentile);
    CV_Assert(idx == (hist.rows - 1) || cv::sum(hist(cv::Range(0, idx + 1), cv::Range::all()))[0] >= percentile);

    return idx;
}

float fsiv_histogram_idx_to_value(int idx, int n_bins, float max_value, float min_value){
    CV_Assert(idx >= 0);
    CV_Assert(idx < n_bins);

    // Remember: Map integer range [0, n_bins) into float 
    // range [min_value, max_value)
    float value = static_cast<float>(idx) / static_cast<float>(n_bins) * (max_value - min_value) + min_value;

    CV_Assert(value >= min_value);
    CV_Assert(value < max_value);

    return value;
}

void fsiv_percentile_edge_detector(cv::Mat const& gradient, cv::Mat& edges, float th, int n_bins){
    CV_Assert(gradient.type() == CV_32FC1);

    cv::Mat hist;
    float max_gradient;

    // Remember: user other fsiv_xxx to compute histogram and percentiles.
    // Remember: map histogram range {0, ..., n_bins} to the gradient range {0.0, ..., max_grad}
    fsiv_compute_gradient_histogram(gradient, n_bins, hist, max_gradient);

    int th_idx = fsiv_compute_histogram_percentile(hist, th);
    float threshold_value = fsiv_histogram_idx_to_value(th_idx, n_bins, max_gradient);

    // Hint: use "operator >=" to threshold the gradient magnitude image.
    edges = (gradient >= threshold_value) * 255;

    CV_Assert(edges.type() == CV_8UC1);
    CV_Assert(edges.size() == gradient.size());
}


void fsiv_otsu_edge_detector(cv::Mat const& gradient, cv::Mat& edges){
    CV_Assert(gradient.type() == CV_32FC1);

    // Normalize the gradient to the range (0, 255)
    cv::Mat normalized_gradient;
    cv::normalize(gradient, normalized_gradient, 0, 255, cv::NORM_MINMAX);

    // Convert the normalized gradient to an 8-bit grayscale image
    cv::Mat gradient_8u;
    normalized_gradient.convertTo(gradient_8u, CV_8UC1);

    // Apply the Otsu's method to find the threshold
    cv::threshold(gradient_8u, edges, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    CV_Assert(edges.type() == CV_8UC1);
    CV_Assert(edges.size() == gradient.size());
}


void fsiv_canny_edge_detector(cv::Mat const& dx, cv::Mat const& dy, cv::Mat& edges, float th1, float th2, int n_bins){
    CV_Assert(dx.size() == dy.size());
    CV_Assert(th1 < th2);

    // convert the intput derivatives to CV_16C1 to be used with canny.
    cv::Mat dx_16, dy_16;
    dx.convertTo(dx_16, CV_16SC1);
    dy.convertTo(dy_16, CV_16SC1);

    // Calculate gradient magnitude
    cv::Mat gradient;
    fsiv_compute_gradient_magnitude(dx, dy, gradient);

    // Convert gradient to CV_8U
    gradient.convertTo(gradient, CV_8U);

    // Compute histogram and percentils
    cv::Mat hist;
    float max_gradient;
    fsiv_compute_gradient_histogram(gradient, n_bins, hist, max_gradient);

    int th1_idx = fsiv_compute_histogram_percentile(hist, th1);
    int th2_idx = fsiv_compute_histogram_percentile(hist, th2);

    // Transform percentiles to gradient values
    float threshold1 = fsiv_histogram_idx_to_value(th1_idx, n_bins, max_gradient);
    float threshold2 = fsiv_histogram_idx_to_value(th2_idx, n_bins, max_gradient);

    //gradient.convertTo(gradient, CV_16SC1); 

    // Remember: we compute gradients with L2_NORM so we must indicate this in the canny method too. -> true
    // Apply Canny edge detector
    cv::Canny(dx_16, dy_16, edges, threshold1, threshold2, true);

    // Convert edges to CV_8UC1
    edges.convertTo(edges, CV_8UC1);

    CV_Assert(edges.type() == CV_8UC1);
    CV_Assert(edges.size() == dx.size());
}



void fsiv_compute_confusion_matrix(cv::Mat const& gt, cv::Mat const& pred, cv::Mat & cm){
    CV_Assert(gt.type() == CV_8UC1);
    CV_Assert(pred.type() == CV_8UC1);
    CV_Assert(gt.size() == pred.size());

    // Remember: a edge detector confusion matrix is a 2x2 matrix where the 
    // rows are ground truth {Positive: "is edge", Negative: "is not edge"} and
    // the columns are the predictions labels {"is edge", "is not edge"}
    // A pixel value means edge if it is <> 0, else is a "not edge" pixel.

    cm = cv::Mat::zeros(2, 2, CV_32FC1);

    // Iterate over each pixel in the gt and pred images
    for (int i = 0; i < gt.rows; ++i) {
        for (int j = 0; j < gt.cols; ++j) {
            uchar gt_pixel = gt.at<uchar>(i, j);
            uchar pred_pixel = pred.at<uchar>(i, j);

            // Increment the corresponding values in the confusion matrix
            if (gt_pixel != 0) {  // Ground truth: "is edge"
                if (pred_pixel != 0) {  // Prediction: "is edge"
                    cm.at<float>(0, 0) += 1;  // True Positive
                } else {
                    cm.at<float>(0, 1) += 1;  // False Negative
                }
            } else {  // Ground truth: "is not edge"
                if (pred_pixel != 0) {  // Prediction: "is edge"
                    cm.at<float>(1, 0) += 1;  // False Positive
                } else {
                    cm.at<float>(1, 1) += 1;  // True Negative
                }
            }
        }
    }

    CV_Assert(cm.type()==CV_32FC1);
    CV_Assert(cv::abs(cv::sum(cm)[0]-(gt.rows*gt.cols))<1.0e-6);
}

float fsiv_compute_sensitivity(cv::Mat const& cm)
{
    CV_Assert(cm.type() == CV_32FC1);
    CV_Assert(cm.size() == cv::Size(2, 2));

    // Sensitivity (True Positive Rate) = TP / (TP + FN)
    float sensitivity = cm.at<float>(0, 0) / (cm.at<float>(0, 0) + cm.at<float>(0, 1));
    return sensitivity;
}

float fsiv_compute_precision(cv::Mat const& cm){
    CV_Assert(cm.type() == CV_32FC1);
    CV_Assert(cm.size() == cv::Size(2, 2));

    // Precision = TP / (TP + FP)
    float precision = cm.at<float>(0, 0) / (cm.at<float>(0, 0) + cm.at<float>(1, 0));
    return precision;
}

float fsiv_compute_F1_score(cv::Mat const& cm){
    CV_Assert(cm.type()==CV_32FC1);
    CV_Assert(cm.size()==cv::Size(2,2));

    // Compute sensitivity and precision using the previously defined functions
    float sensitivity = fsiv_compute_sensitivity(cm);
    float precision = fsiv_compute_precision(cm);

    // F1 Score = 2 * (Precision * Sensitivity) / (Precision + Sensitivity)
    float F1 = 2 * (precision * sensitivity) / (precision + sensitivity);
    return F1;
}
