#include "common_code.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

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
