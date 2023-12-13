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
        std::cerr << "Usage: stereo_disparity stereo_image.jpg calibration.yml out.obj" << std::endl;
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


     // Calcular la matriz Q utilizando cv::stereoRectify
    cv::Mat R1, R2, P1, P2, Q;
    cv::stereoRectify(stParams.mtxL, stParams.distL, stParams.mtxR, stParams.distR,
                      leftRectified.size(), stParams.Rot, stParams.Trns, R1, R2, P1, P2, Q);

    
    cv::Mat rectifiedImage;
    cv::hconcat(left, right, rectifiedImage); 

    // Disparity calculatio
    int numDisparities = 16 * 4; // Set the number of disparities 16 * x = 64 ?
    int blockSize = 15;      // Set the block size 15

    cv::Mat disparity;
    // Stereo Disparity
    cv::Ptr<cv::StereoBM> stereo = cv::StereoBM::create(numDisparities, blockSize);
    stereo->compute(leftRectified, rightRectified, disparity);

    // Converting disparity values to CV_32F from CV_16S
    disparity.convertTo(disparity,CV_32F, 1.0);
    disparity=disparity/16.f;

    float leftCx = stParams.mtxL.at<double>(0, 2); // Extracting cx for left camera
    float leftCy = stParams.mtxL.at<double>(1, 2); // Extracting cy for left camera

    float leftFocalLengthX = stParams.mtxL.at<double>(0, 0); // Extracting fx for left camera
    float leftFocalLengthY = stParams.mtxL.at<double>(1, 1); // Extracting fy for left camera
    // Calculate average focal length for left camera
    float focalLegthLeft = (leftFocalLengthX + leftFocalLengthY) / 2.0;

    // Calculate L1 norm of the matrix ///L1 norm (sum of absolute values).
    double normL1Tras = cv::norm(stParams.Trns, cv::NORM_L1);

    // Triangulate
    std::vector<cv::Point3f> points;
    for (int row = 0; row < disparity.rows; row++) {
        for (int col = 0; col < disparity.cols; col++) {
            float d = disparity.at<float>(row, col);
            if (d > 10) {
                float Z = normL1Tras * focalLegthLeft / d;
                float X = (col - leftCx) * Z / focalLegthLeft;
                float Y = (row - leftCy) * Z / focalLegthLeft;
                points.push_back(cv::Point3f(X, Y, Z));
            }
        }
    }

    // Save in OBJ objetct
    writeToOBJ(argv[3], points);

    return 0;
}
