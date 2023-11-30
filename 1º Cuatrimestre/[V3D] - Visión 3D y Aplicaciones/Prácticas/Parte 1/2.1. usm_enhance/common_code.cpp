#include <opencv2/imgproc.hpp>
#include "common_code.hpp"
#include <iostream>


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

