using namespace std;

#include "common_code.hpp"
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
