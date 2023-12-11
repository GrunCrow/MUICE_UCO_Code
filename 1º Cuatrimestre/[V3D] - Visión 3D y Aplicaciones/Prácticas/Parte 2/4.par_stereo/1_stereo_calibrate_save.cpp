#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>

// Defining the dimensions of checkerboard
int CHECKERBOARD[2]{7, 5};
double SquareSize = 0.02875; // size of each square

int main(int argc, char *argv[])
{

    if (argc == 1)
    {
        std::cerr << "Usage: folder_with_images calibration.yml" << std::endl;
        return -1;
    }

    // Creating vector to store vectors of 3D points for each checkerboard image
    std::vector<std::vector<cv::Point3f>> objpoints;

    // Creating vector to store vectors of 2D points for each checkerboard image
    std::vector<std::vector<cv::Point2f>> imgpointsLeft;
    std::vector<std::vector<cv::Point2f>> imgpointsRight;

    // Defining the world coordinates for 3D points
    std::vector<cv::Point3f> objp;
    for (int i = 0; i < CHECKERBOARD[1]; i++)
    {
        for (int j = 0; j < CHECKERBOARD[0]; j++)
            objp.push_back(cv::Point3f(j * SquareSize, i * SquareSize, 0));
    }

    // Extracting path of individual image stored in a given directory
    std::vector<cv::String> images;
    // Path of the folder containing checkerboard images
    std::string path = std::string(argv[1]) + "/*.jpg";

    cv::glob(path, images);

    cv::Mat frame, gray;
    // vector to store the pixel coordinates of detected checkerboard corners
    std::vector<cv::Point2f> corner_ptsLeft, corner_ptsRight;
    bool success;

    // Looping over all the images in the directory
    for (size_t i = 0; i < images.size(); i++)
    {
        std::cout << "Reading.. " << images[i] << std::endl;

        // Read the image
        frame = cv::imread(images[i]);
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        // Split the image into left and right halves
        cv::Mat left = frame.colRange(0, frame.cols / 2);
        cv::Mat right = frame.colRange(frame.cols / 2, frame.cols);

        success = false;

        // Process left image
        cv::Mat grayLeft;
        cv::cvtColor(left, grayLeft, cv::COLOR_BGR2GRAY);
        success = cv::findChessboardCorners(grayLeft, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_ptsLeft, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FAST_CHECK | cv::CALIB_CB_NORMALIZE_IMAGE);

        // Process right image
        cv::Mat grayRight;
        cv::cvtColor(right, grayRight, cv::COLOR_BGR2GRAY);
        success = success && cv::findChessboardCorners(grayRight, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_ptsRight, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FAST_CHECK | cv::CALIB_CB_NORMALIZE_IMAGE);

        // If both left and right images have valid checkerboard corners
        if (success)
        {
            cv::TermCriteria criteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 60, 1e-6);

            // Refining pixel coordinates for left image
            cv::cornerSubPix(grayLeft, corner_ptsLeft, cv::Size(11, 11), cv::Size(-1, -1), criteria);

            // Refining pixel coordinates for right image
            cv::cornerSubPix(grayRight, corner_ptsRight, cv::Size(11, 11), cv::Size(-1, -1), criteria);

            // Displaying the detected corner points on the checkerboard
            cv::drawChessboardCorners(left, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_ptsLeft, true);
            cv::drawChessboardCorners(right, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_ptsRight, true);

            objpoints.push_back(objp);
            imgpointsLeft.push_back(corner_ptsLeft);
            imgpointsRight.push_back(corner_ptsRight);
        }

        // Resize image to fit into screen
        cv::Mat resized;
        cv::resize(frame, resized, cv::Size(1920, 1080));
        cv::imshow("Image", resized);
        cv::waitKey(27);
    }

    cv::destroyAllWindows();

    cv::destroyAllWindows();

    // Stereo calibration
    cv::Mat cameraMatrixLeft, distCoeffsLeft, RLeft, TLeft;
    cv::Mat cameraMatrixRight, distCoeffsRight, RRight, TRight;

    // Performing camera calibration for the left camera
    cv::calibrateCamera(objpoints, imgpointsLeft, cv::Size(gray.cols / 2, gray.rows), cameraMatrixLeft, distCoeffsLeft, RLeft, TLeft);

    // Performing camera calibration for the right camera
    cv::calibrateCamera(objpoints, imgpointsRight, cv::Size(gray.cols / 2, gray.rows), cameraMatrixRight, distCoeffsRight, RRight, TRight);

    // Stereo calibration
    cv::Mat R, T, E, F;

    cv::stereoCalibrate(objpoints, imgpointsLeft, imgpointsRight,
                        cameraMatrixLeft, distCoeffsLeft,
                        cameraMatrixRight, distCoeffsRight,
                        cv::Size(gray.cols / 2, gray.rows), R, T, E, F);

    std::cout << "Left Camera Matrix : " << cameraMatrixLeft << std::endl;
    std::cout << "Right Camera Matrix : " << cameraMatrixRight << std::endl;
    std::cout << "Left Distortion Coefficients : " << distCoeffsLeft << std::endl;
    std::cout << "Right Distortion Coefficients : " << distCoeffsRight << std::endl;
    std::cout << "Rotation Matrix : " << R << std::endl;
    std::cout << "Translation Vector : " << T << std::endl;
    std::cout << "Essential Matrix : " << E << std::endl;
    std::cout << "Fundamental Matrix : " << F << std::endl;

    // Save calibration parameters to a YAML file
    cv::FileStorage fs("calibration.yml", cv::FileStorage::WRITE);
    fs << "LEFT_K" << cameraMatrixLeft << "LEFT_D" << distCoeffsLeft 
       << "RIGHT_K" << cameraMatrixRight << "RIGHT_D" << distCoeffsRight
       << "R" << R << "T" << T << "E" << E << "F" << F;

    return 0;
}
