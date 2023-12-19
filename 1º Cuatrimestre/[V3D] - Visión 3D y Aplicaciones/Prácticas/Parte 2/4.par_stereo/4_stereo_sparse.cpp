// unset GTK_PATH
// build/3_stereo_disparity images/stereo/reconstruction/m001.jpg calibration.yml myout.obj

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <fstream>

struct StereoParams {
    cv::Mat mtxL, distL, R_L, T_L;
    cv::Mat mtxR, distR, R_R, T_R;
    cv::Mat Rot, Trns, Emat, Fmat;
};

void rectifyStereoImages(const StereoParams &sti,cv::Mat &left,cv::Mat &rigth){
   cv::Mat rect_l, rect_r, proj_mat_l, proj_mat_r, Q;
   cv::Mat Left_Stereo_Map1, Left_Stereo_Map2;
   cv::Mat Right_Stereo_Map1, Right_Stereo_Map2;
   cv::stereoRectify(sti.mtxL, sti.distL,sti.mtxR,sti.distR,left.size(),sti.Rot,sti.Trns,
                     rect_l,rect_r,proj_mat_l,proj_mat_r,
                     Q,cv::CALIB_ZERO_DISPARITY, 0);
   cv::initUndistortRectifyMap(sti.mtxL,sti.distL,rect_l,proj_mat_l,
                               left.size(),CV_16SC2,
                               Left_Stereo_Map1,Left_Stereo_Map2);
   cv::initUndistortRectifyMap(sti.mtxR,sti.distR,
                               rect_r,proj_mat_r,
                               left.size(),CV_16SC2,
                               Right_Stereo_Map1,Right_Stereo_Map2);
   cv::Mat AuxImage, Right_nice;
   cv::remap(left, AuxImage, Left_Stereo_Map1, Left_Stereo_Map2,
             cv::INTER_LANCZOS4,cv::BORDER_CONSTANT,0);
   AuxImage.copyTo(left);
   cv::remap(rigth,  AuxImage,  Right_Stereo_Map1,  Right_Stereo_Map2,
             cv::INTER_LANCZOS4,cv::BORDER_CONSTANT,0);
   AuxImage.copyTo(rigth);
}

void writeToOBJ(std::string path,std::vector<cv::Point3f> points){
    std::ofstream file(path,std::ios::binary);
    for(auto p:points)
        file<<"v "<<p.x<<" "<<p.y<<" "<<p.z<<std::endl;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: stereo_sparse stereo_image.jpg calibration.yml out.obj" << std::endl;
        return -1;
    }

    // Read stereo calibration parameters from YML file   
    StereoParams stParams;
    cv::FileStorage fs(argv[2], cv::FileStorage::READ);
    if (!fs.isOpened()) {
        std::cerr << "Error: Could not open the calibration file." << std::endl;
        return -1;
    }

    fs["LEFT_K"] >> stParams.mtxL;
    fs["LEFT_D"] >> stParams.distL;
    fs["RIGHT_K"] >> stParams.mtxR;
    fs["RIGHT_D"] >> stParams.distR;
    fs["R"] >> stParams.Rot;
    fs["T"] >> stParams.Trns;
    fs["E"] >> stParams.Emat;
    fs["F"] >> stParams.Fmat;

    // Read stereo image
    cv::Mat stereoImage = cv::imread(argv[1]);

    if (stereoImage.empty()) {
        std::cerr << "Error: Could not read the stereo image." << std::endl;
        return -1;
    }

    // Rectify images
    cv::Mat left = stereoImage.colRange(0, stereoImage.cols / 2);
    cv::Mat right = stereoImage.colRange(stereoImage.cols / 2, stereoImage.cols);

    cv::Mat leftRectified = left.clone();
    cv::Mat rightRectified = right.clone();

    rectifyStereoImages(stParams, leftRectified, rightRectified);

    // Convert images to gray scale
    cv::cvtColor(leftRectified, leftRectified, cv::COLOR_BGR2GRAY);
    cv::cvtColor(rightRectified, rightRectified, cv::COLOR_BGR2GRAY);

    // Detect keypoints and compute descriptors using AKAZE
    std::vector<cv::KeyPoint> keypoints_query, keypoints_train;
    cv::Mat descriptors_query, descriptors_train;
    auto Detector = cv::AKAZE::create(cv::AKAZE::DESCRIPTOR_MLDB, 0, 3, 1e-4f, 8);
    Detector->detectAndCompute(leftRectified, cv::Mat(), keypoints_query, descriptors_query);
    Detector->detectAndCompute(rightRectified, cv::Mat(), keypoints_train, descriptors_train);

    // Match descriptors using BruteForce-Hamming
    std::vector<cv::DMatch> matches;
    auto matcher = cv::DescriptorMatcher::create("BruteForce-Hamming");
    matcher->match(descriptors_query, descriptors_train, matches, cv::Mat());

    // Filter matches based on horizontal lines
    // ... (Implement filtering based on horizontal lines)
    std::vector<cv::DMatch> filtered_matches;
    for (const auto &match : matches) {
        // Implement filtering based on horizontal lines
        // For example, you can check the y-coordinate of the matched keypoints
        // and only keep matches that are close to each other vertically
        if (std::abs(keypoints_query[match.queryIdx].pt.y - keypoints_train[match.trainIdx].pt.y) < 10.0) {
            filtered_matches.push_back(match);
        }
    }

    // Draw matches before and after filtering
    cv::Mat img_matches_before, img_matches_after;
    cv::drawMatches(leftRectified, keypoints_query, rightRectified, keypoints_train, matches, img_matches_before);
    cv::drawMatches(leftRectified, keypoints_query, rightRectified, keypoints_train, filtered_matches, img_matches_after);

    // Show or save the images if needed
    cv::imshow("Matches Before Filtering", img_matches_before);
    cv::imshow("Matches After Filtering", img_matches_after);
    cv::waitKey(0);

    // Triangulate matches
    std::vector<cv::Point3f> points;
    for (const auto &match : filtered_matches) {
        float d = match.distance; // You may use other criteria for depth calculation
        float Z = stParams.Trns.at<double>(0, 0) * stParams.mtxL.at<double>(0, 0) / d;
        float X = (keypoints_query[match.queryIdx].pt.x - stParams.mtxL.at<double>(0, 2)) * Z / stParams.mtxL.at<double>(0, 0);
        float Y = (keypoints_query[match.queryIdx].pt.y - stParams.mtxL.at<double>(1, 2)) * Z / stParams.mtxL.at<double>(1, 1);
        points.push_back(cv::Point3f(X, Y, Z));
    }

    // Save in OBJ object
    writeToOBJ(argv[3], points);

    return 0;
}