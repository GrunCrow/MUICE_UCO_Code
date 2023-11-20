#pragma once

#include <vector>
#include <opencv2/core.hpp>

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

