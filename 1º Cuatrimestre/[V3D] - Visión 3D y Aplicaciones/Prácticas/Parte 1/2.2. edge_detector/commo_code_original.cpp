#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "common_code.hpp"

void 
fsiv_compute_derivate(cv::Mat const& img, cv::Mat& dx, cv::Mat& dy, int g_r, 
    int s_ap)
{
    CV_Assert(img.type()==CV_8UC1);
    // TODO
    // Hint: if g_r > 0 apply a Gaussian Blur operation with kernel size 2*g_r+1.
    // Hint: use Sobel operator to compute derivate.

    //
    CV_Assert(dx.size()==img.size());
    CV_Assert(dy.size()==dx.size());
    CV_Assert(dx.type()==CV_32FC1);
    CV_Assert(dy.type()==CV_32FC1);
}

void 
fsiv_compute_gradient_magnitude(cv::Mat const& dx, cv::Mat const& dy,
    cv::Mat& gradient)
{
    CV_Assert(dx.size()==dy.size());
    CV_Assert(dx.type()==CV_32FC1);
    CV_Assert(dy.type()==CV_32FC1);

    // TODO
    // Hint: use cv::magnitude.


    //

    CV_Assert(gradient.size()==dx.size());
    CV_Assert(gradient.type()==CV_32FC1);
}

void
fsiv_compute_gradient_histogram(cv::Mat const& gradient, int n_bins, cv::Mat& hist, float & max_gradient)
{   
    // TODO
    // Hint: use cv::minMaxLoc to get the gradient range {0, max_gradient}
    // Remember: use cv::normalize to get a normalized histogram.

    //
    CV_Assert(max_gradient>0.0);
    CV_Assert(hist.rows == n_bins);
    CV_Assert(cv::abs(cv::sum(hist)[0]-1.0)<1.e-6);
}

int
fsiv_compute_histogram_percentile(cv::Mat const& hist, float percentile)
{
    CV_Assert(percentile>=0.0 && percentile<=1.0);
    CV_Assert(hist.type()==CV_32FC1);
    CV_Assert(hist.cols==1);
    CV_Assert(cv::abs(cv::sum(hist)[0]-1.0)<1.e-6);
    int idx = 0;
    // TODO
    // Remember: The percentile p is the first i that sum{h[0], h[1], ..., h[i]} >= p    

    //
    CV_Assert(idx>=0 && idx<hist.rows);
    CV_Assert(idx<=0 || cv::sum(hist(cv::Range(0, idx), cv::Range::all()))[0]<=percentile);
    CV_Assert(idx==(hist.rows-1) || cv::sum(hist(cv::Range(0, idx+1), cv::Range::all()))[0]>=percentile);
    return idx;
}

float 
fsiv_histogram_idx_to_value(int idx, int n_bins, float max_value,
    float min_value)
{
    CV_Assert(idx>=0);
    CV_Assert(idx<n_bins);
    float value = 0.0;
    // TODO
    // Remember: Map integer range [0, n_bins) into float 
    // range [min_value, max_value)

    //
    CV_Assert(value >= min_value);
    CV_Assert(value < max_value);
    return value;
}

void 
fsiv_percentile_edge_detector(cv::Mat const& gradient, cv::Mat& edges,
    float th, int n_bins)
{
    CV_Assert(gradient.type()==CV_32FC1);

    // TODO
    // Remember: user other fsiv_xxx to compute histogram and percentiles.
    // Remember: map histogram range {0, ..., n_bins} to the gradient range 
    // {0.0, ..., max_grad}
    // Hint: use "operator >=" to threshold the gradient magnitude image.

    //
    CV_Assert(edges.type()==CV_8UC1);
    CV_Assert(edges.size()==gradient.size());
}

void 
fsiv_otsu_edge_detector(cv::Mat const& gradient, cv::Mat& edges)
{
    CV_Assert(gradient.type()==CV_32FC1);

    // TODO
    // Hint: normalize input gradient into rango (0, 255) to use cv::threshold properly.

    CV_Assert(edges.type()==CV_8UC1);
    CV_Assert(edges.size()==gradient.size());
}

void
fsiv_canny_edge_detector(cv::Mat const& dx, cv::Mat const& dy, cv::Mat& edges,
    float th1, float th2, int n_bins)
{
    CV_Assert(dx.size()==dy.size());
    CV_Assert(th1<th2);

    // TODO
    // Hint: convert the intput derivatives to CV_16C1 to be used with canny.
    // Remember: th1 and th2 are given as percentiles so you must transform to
    // gradient range to be used in canny method.
    // Remember: we compute gradients with L2_NORM so we must indicate this in
    // the canny method too.

    //
    CV_Assert(edges.type()==CV_8UC1);
    CV_Assert(edges.size()==dx.size());
}

void
fsiv_compute_confusion_matrix(cv::Mat const& gt, cv::Mat const& pred, cv::Mat & cm)
{
    CV_Assert(gt.type()==CV_8UC1);
    CV_Assert(pred.type()==CV_8UC1);
    CV_Assert(gt.size()==pred.size());

    // TODO
    // Remember: a edge detector confusion matrix is a 2x2 matrix where the 
    // rows are ground truth {Positive: "is edge", Negative: "is not edge"} and
    // the columns are the predictions labels {"is edge", "is not edge"}
    // A pixel value means edge if it is <> 0, else is a "not edge" pixel.

    //
    CV_Assert(cm.type()==CV_32FC1);
    CV_Assert(cv::abs(cv::sum(cm)[0]-(gt.rows*gt.cols))<1.0e-6);
}

float
fsiv_compute_sensitivity(cv::Mat const& cm)
{
    CV_Assert(cm.type()==CV_32FC1);
    CV_Assert(cm.size()==cv::Size(2,2));
    float sensitivity=0.0;
    // TODO    
    // Hint: see https://en.wikipedia.org/wiki/Confusion_matrix

    //
    return sensitivity;
}

float
fsiv_compute_precision(cv::Mat const& cm)
{
    CV_Assert(cm.type()==CV_32FC1);
    CV_Assert(cm.size()==cv::Size(2,2));
    float precision=0.0;
    // TODO    
    // Hint: see https://en.wikipedia.org/wiki/Confusion_matrix

    //
    return precision;
}

float
fsiv_compute_F1_score(cv::Mat const& cm)
{
    CV_Assert(cm.type()==CV_32FC1);
    CV_Assert(cm.size()==cv::Size(2,2));
    float F1 = 0.0;
    // TODO
    // Hint: see https://en.wikipedia.org/wiki/Confusion_matrix

    //
    return F1;
}