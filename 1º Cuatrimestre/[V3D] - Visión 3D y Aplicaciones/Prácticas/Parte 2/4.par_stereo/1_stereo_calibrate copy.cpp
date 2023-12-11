#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

// Defining the dimensions of checkerboard
int CHECKERBOARD[2]{5,7}; // rows, cols
//int CHECKERBOARD[2]{6,4}; // esquinas interiores

double SquareSize=0.02875;//size of each square

//opt criteria = cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 60, 1e-6)

int main(int argc,char *argv[])
{

    if(argc==1){
        std::cerr<<"Usage: folder_with_images"<<std::endl;
        return -1;
    }

    // Creating vector to store vectors of 3D points for each checkerboard image
    std::vector<std::vector<cv::Point3f> > objpoints_left;
    std::vector<std::vector<cv::Point3f> > objpoints_right;
    // Creating vector to store vectors of 2D points for each checkerboard image
    std::vector<std::vector<cv::Point2f> > imgpoints_left;
    std::vector<std::vector<cv::Point2f> > imgpoints_right;

    // Defining the world coordinates for 3D points
    std::vector<cv::Point3f> objp;
    for(int i{0}; i<CHECKERBOARD[1]; i++)
    {
        for(int j{0}; j<CHECKERBOARD[0]; j++)
            objp.push_back(cv::Point3f(j,i,0));
    }

    // Extracting path of individual image stored in a given directory
    std::vector<cv::String> images;
    // Path of the folder containing checkerboard images
    std::string path = std::string(argv[1])+"/*.jpg";

    cv::glob(path, images);

    cv::Mat frame, gray_left, gray_right;
    // vector to store the pixel coordinates of detected checker board corners
    std::vector<cv::Point2f> corner_pts_left;
    std::vector<cv::Point2f> corner_pts_right;

    bool success_left;
    bool success_right;

    // Looping over all the images in the directory
    for(size_t i=0; i<images.size(); i++)
    {
        std::cout<<"Reading.."<<images[i]<<std::endl;
        frame = cv::imread(images[i]);

        cv::Mat left = frame.colRange(0, frame.cols/2 - 1);
        cv::Mat right = frame.colRange(frame.cols/2,frame.cols - 1);

        cv::cvtColor(left,gray_left,cv::COLOR_BGR2GRAY);
        cv::cvtColor(right,gray_right,cv::COLOR_BGR2GRAY);      

        // Finding checker board corners
        // If desired number of corners are found in the image then success = true
        success_left = cv::findChessboardCorners(gray_left, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts_left, cv::CALIB_CB_ADAPTIVE_THRESH |  cv::CALIB_CB_FAST_CHECK |  cv::CALIB_CB_NORMALIZE_IMAGE);

        /*
        * If desired number of corner are detected,
        * we refine the pixel coordinates and display
        * them on the images of checker board
        */

        if(success_left)
        {
            cv::TermCriteria criteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 60, 1e-6);

            // refining pixel coordinates for given 2d points.
            cv::cornerSubPix(gray_left,corner_pts_left,cv::Size(11,11), cv::Size(-1,-1),criteria);

            // Displaying the detected corner points on the checker board
            cv::drawChessboardCorners(frame, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts_left, success_left);

            objpoints.push_back(objp);
            imgpoints_left.push_back(corner_pts_left);
        }

        // solvepnp - objectPoints puntos 3d, imagePoints - proyeccion de los puntos 3d, cameraMatrix - matriz de calibracion, 
        // distCoeffs[, rvec[, tvec[, useExtrinsicGuess[, flags]]]]

        //resize image to fit into screen
        cv::Mat resized;
        cv::resize(frame,resized,cv::Size(1920,1080));
        cv::imshow("Image",resized);
        cv::waitKey(0);
    }

    cv::destroyAllWindows();

    cv::Mat cameraMatrix,distCoeffs,R,T;

    /*
   * Performing camera calibration by
   * passing the value of known 3D points (objpoints)
   * and corresponding pixel coordinates of the
   * detected corners (imgpoints)
  */
    cv::calibrateCamera(objpoints, imgpoints, cv::Size(gray.rows,gray.cols), cameraMatrix, distCoeffs, R, T);

    std::cout << "cameraMatrix : " << cameraMatrix << std::endl;
    std::cout << "distCoeffs : " << distCoeffs << std::endl;
    std::cout << "Rotation vector : " << R << std::endl;
    std::cout << "Translation vector : " << T << std::endl;
    cv::FileStorage fs("calibration.yml", cv::FileStorage::WRITE);
    
    fs << "cameraMatrix" << cameraMatrix << "distCoeffs" << distCoeffs;


    return 0;
}