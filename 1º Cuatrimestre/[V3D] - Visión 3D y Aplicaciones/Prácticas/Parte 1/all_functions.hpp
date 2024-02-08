/*
================================================================================================================================================
                                                                    Práctica 1
================================================================================================================================================
*/

#pragma once

#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

/**
 * @brief Find the first max/min values and their locations.
 * 
 * The implementation must do a rows/cols scanning of the image.
 * 
 * @param input is the input image.
 * @param max_v maximum values per channel.
 * @param min_v minimum values per channel.
 * @param max_loc maximum locations per channel.
 * @param min_loc minimum values per channel.
 * @post max_v.size()==input.channels()
 * @post min_v.size()==input.channels()
 * @post max_loc.size()==input.channels()
 * @post min_loc.size()==input.channels()
 */
void fsiv_find_min_max_loc_1(cv::Mat const& input,
    std::vector<cv::uint8_t>& min_v, std::vector<cv::uint8_t>& max_v,
    std::vector<cv::Point>& min_loc, std::vector<cv::Point>& max_loc);

/**
 * @brief Find the first max/min values and their locations.
 * 
 * The implementation must use the cv::minMaxLoc opencv function to vectorize the code.
 * 
 * @param input is the input image.
 * @param max_v maximum values per channel.
 * @param min_v minimum values per channel.
 * @param max_loc maximum locations per channel.
 * @param min_loc minimum values per channel.
 * @post max_v.size()==input.channels()
 * @post min_v.size()==input.channels()
 * @post max_loc.size()==input.channels()
 * @post min_loc.size()==input.channels()
 */
void fsiv_find_min_max_loc_2(cv::Mat const& input,
    std::vector<double>& min_v, std::vector<double>& max_v,
    std::vector<cv::Point>& min_loc, std::vector<cv::Point>& max_loc);



cv:minMaxLoc()



/*
================================================================================================================================================
                                                                    Práctica 2
================================================================================================================================================
*/

/**
 * @brief Convierte una imagen con tipo byte a flotante [0,1].
 * @param img imagen de entrada.
 * @param out imagen de salida.
 * @return la imagen de salida.
 * @warning la imagen de entrada puede ser monocroma o RGB.
 */
cv::Mat fsiv_convert_image_byte_to_float(const cv::Mat& img, cv::Mat& out);

/**
 * @brief Convierte una imagen con tipo float [0,1] a byte [0,255].
 * @param img imagen de entrada.
 * @param out imagen de salida.
 * @return la imagen de salida.
 * @warning la imagen de entrada puede ser monocroma o RGB.
 */
cv::Mat fsiv_convert_image_float_to_byte(const cv::Mat& img, cv::Mat& out);

/**
 * @brief Convierte una imagen en color BGR a HSV.
 * @param img imagen de entrada.
 * @param out imagen de salida.
 * @return la imagen de salida.
 */
cv::Mat fsiv_convert_bgr_to_hsv(const cv::Mat& img, cv::Mat& out);

/**
 * @brief Convierte una imagen en color HSV a BGR.
 * @param img imagen de entrada.
 * @param out imagen de salida.
 * @return la imagen de salida.
 */
cv::Mat fsiv_convert_hsv_to_bgr(const cv::Mat& img, cv::Mat& out);

/**
 * @brief Realiza un control del brillo/contraste/gamma de la imagen.
 *
 * El proceso sería:
 *
 * O = c * I^g + b
 *
 * Si la imagen es RGB y el flag only_luma es true, se utiliza el espacio HSV
 * para procesar sólo el canal V (luma).
 *
 * @param img  imagen de entrada.
 * @param out  imagen de salida.
 * @param contrast controla el ajuste del contraste.
 * @param brightness controla el ajuste del brillo.
 * @param gamma controla el ajuste de la gamma.
 * @param only_luma si es true sólo se procesa el canal Luma.
 * @return la imagen procesada.
 */
cv::Mat fsiv_cbg_process (const cv::Mat & img, cv::Mat& out,
             double contrast=1.0, double brightness=0.0, double gamma=1.0,
             bool only_luma=true);







/*
================================================================================================================================================
                                                                    Práctica 3
================================================================================================================================================
*/

/**
 * @brief Scale the color of an image so an input color is transformed into an output color.
 * @param in is the image to be rescaled.
 * @param from is the input color.
 * @param to is the output color.
 * @return the color rescaled image.
 * @pre in.type()==CV_8UC3
 * @warning A BGR color space is assumed for the input image.
 */
cv::Mat fsiv_color_rescaling(const cv::Mat& in, const cv::Scalar& from,
                             const cv::Scalar& to);
/**
 * @brief Apply a "white patch" color balance operation to the image.
 * @arg[in] in is the imput image.
 * @return the color balanced image.
 * @pre in.type()==CV_8UC3
 * @warning A BGR color space is assumed for the input image.
 */
cv::Mat fsiv_wp_color_balance(cv::Mat const& in);

/**
 * @brief Apply a "gray world" color balance operation to the image.
 * @arg[in] in is the imput image.
 * @return the color balanced image.
 * @pre in.type()==CV_8UC3
 * @warning A BGR color space is assumed for the input image.
 */
cv::Mat fsiv_gw_color_balance(cv::Mat const& in);

/**
 * @brief Apply a general color balance operation to the image.
 * @arg[in] in is the imput image.
 * @arg[in] p is the percentage of brightest points used to calculate the color correction factor.
 * @return the color balanced image.
 * @pre in.type()==CV_8UC3
 * @pre 0.0 < p < 100.0
 * @warning A BGR color space is assumed for the input image.
 */
cv::Mat fsiv_color_balance(cv::Mat const& in, float p); // blanco automatico








/*
================================================================================================================================================
                                                                    Práctica 2.1
================================================================================================================================================
*/

/**
 * @brief Return a box filter.
 * @arg[in] r is the filter's radius.
 * @return the filter.
 * @pre r>0;
 * @post ret_v.type()==CV_32FC1
 * @post retV.rows==retV.cols==2*r+1
 * @post (abs(cv::sum(retV)-1.0)<1.0e-6
 */
cv::Mat fsiv_create_box_filter(const int r);

/**
 * @brief Return a Gaussian filter.
 * @arg[in] r is the filter's radius.
 * @return the filter.
 * @pre r>0;
 * @post ret_v.type()==CV_32FC1
 * @post retV.rows==retV.cols==2*r+1
 * @post (abs(cv::sum(retV)-1.0)<1.0e-6
 */
cv::Mat fsiv_create_gaussian_filter(const int r);

/**
 * @brief Combine two images using weigths.
 * @param src1 first image.
 * @param src2 second image.
 * @param a weight for first image.
 * @param b weight for the second image.
 * @return a * src1 + b * src2
 * @pre src1.type()==src2.type()
 * @pre src1.rows==src2.rows
 * @pre src1.cols==src2.cols
 * @post retv.type()==src2.type()
 * @post retv.rows==src2.rows
 * @post retv.cols==src2.cols
 */
cv::Mat fsiv_combine_images(const cv::Mat src1, const cv::Mat src2,
                            double a, double b);
/**
 * @brief Apply an unsharp mask enhance to the input image.
 * @arg[in] in is the input image.
 * @arg[in] g is the enhance's gain.
 * @arg[in] r is the window's radius.
 * @arg[in] filter_type specifies which filter to use. 0->Box, 1->Gaussian.
 * @arg[out] unsharp_mask if it is not nullptr, save the unsharp mask used.
 * @pre !in.empty() 
 * @pre g>=0.0
 * @pre r>0
 * @pre filter_type is {0, 1}
 * @post ret_v.rows==in.rows && ret_v.cols==in.cols
 * @post ret_v.type()==CV_32FC1
 */
cv::Mat fsiv_usm_enhance(cv::Mat  const& in, double g=1.0, int r=1,
                         int filter_type=0,
                         cv::Mat* unsharp_mask=nullptr);







/*
================================================================================================================================================
                                                                    Práctica 2.2
================================================================================================================================================
*/

/**
 * @brief Compute image derivatives.
 * 
 * @param[in] img input image
 * @param[out] dx x axis derivate 
 * @param[out] dy y axis derivate
 * @param[in] g_r gaussian radio used to do a gaussian blur.
 * @param[in] s_ap Sobel kernel size.
 * 
 */
void fsiv_compute_derivate(cv::Mat const& img, cv::Mat& dx, cv::Mat& dy, int g_r, 
    int s_ap=1);

/**
 * @brief Compute gradient magnitude.
 * 
 * @param[in] dx x axis derivate.
 * @param[in] dy x axis derivate.
 * @param[out] gradient magnitude.
 */
void fsiv_compute_gradient_magnitude(cv::Mat const& dx, cv::Mat const& dy,
    cv::Mat& gradient);

/**
 * @brief Compute gradient histogram.
 * 
 * @param[in] gradient magnitude.
 * @param[in] n_bins number of histogram's bins.
 * @param[out] hist the gradient histogram.
 * @param[out] max_gradient maximum gradient value.
 */
void fsiv_compute_gradient_histogram(cv::Mat const& gradient, int n_bins,
     cv::Mat& hist, float & max_gradient);

/**
 * @brief Compute the histogram idx corresponding with a percentile.
 * 
 * @param[in] hist the histogrman.
 * @param[in] percentile the percentile to find.
 * @return the histogram index corresponding with the percentile.
 */
int fsiv_compute_histogram_percentile(cv::Mat const& hist, float percentile);

/**
 * @brief Maps from integer range [0, nbins) to float range [min_value, max_value).
 * 
 * @param[in] idx input index.
 * @param[in] n_bins number of bins.
 * @param[in] max_value maximum output range value.
 * @param[in] min_value minimum output range value.
 * @return the mapped value.
 */
float fsiv_histogram_idx_to_value(int idx, int n_bins, float max_value, float min_value=0.0);

/**
 * @brief Detect borders using the percentile method.
 * 
 * @param[in] gradient input magnitude.
 * @param[out] edges the detected borders.
 * @param[in] th is the gradient percentile used as threshold.
 * @param[in] n_bins number of histogram's bins.
 */
void fsiv_percentile_edge_detector(cv::Mat const& gradient, cv::Mat& edges,
    float th, int n_bins=100);

/**
 * @brief Detect borders using the Otsu method.
 * 
 * @param[in] gradient input magnitude.
 * @param[out] edges the detected borders.
 */   
void fsiv_otsu_edge_detector(cv::Mat const& gradient, cv::Mat& edges);

/**
 * @brief Detect borders using the Canny method.
 * 
 * @param[in] gradient input magnitude.
 * @param[out] edges the detected borders.
 * @param[in] th1 is the gradient percentile used as low threshold.
 * @param[in] th2 is the gradient percentile used as high threshold.
 * @param[in] n_bins number of histogram's bins.
 */
void fsiv_canny_edge_detector(cv::Mat const& dx, cv::Mat const& dy,
    cv::Mat& edges, float th_low=0.2, float th_high=0.8, int n_bins=100);

/**
 * @brief Compute a edge detector confusion matrix.
 * 
 * @param[in] gt is the ground truth.
 * @param[in] pred are the predicted edges.
 * @param[out] cm the confusion matrix.
 */
void fsiv_compute_confusion_matrix(cv::Mat const& gt, cv::Mat const& pred,
    cv::Mat & cm);

/**
 * @brief Compute the sensitivity score
 * 
 * @param cm the confusion matrix.
 * @return the score.
 */
float fsiv_compute_sensitivity(cv::Mat const& cm);

/**
 * @brief Compute the precision score
 * 
 * @param cm the confusion matrix.
 * @return the score.
 */
float fsiv_compute_precision(cv::Mat const& cm);

/**
 * @brief Compute the F1 score
 * 
 * @param cm the confusion matrix.
 * @return the score.
 */
float fsiv_compute_F1_score(cv::Mat const& cm);