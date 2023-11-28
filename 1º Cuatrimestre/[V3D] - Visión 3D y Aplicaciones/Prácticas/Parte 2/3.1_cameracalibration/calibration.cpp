#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>

// Defining the dimensions of checkerboard
int CHECKERBOARD[2]{5,8};
//int CHECKERBOARD[2]{6,4}; // esquinas interiores

int main(int argc,char *argv[])
{

    if(argc==1){
        std::cerr<<"Usage: folder_with_images"<<std::endl;
        return -1;
    }
    // Creating vector to store vectors of 3D points for each checkerboard image
    std::vector<std::vector<cv::Point3f> > objpoints;

    // Creating vector to store vectors of 2D points for each checkerboard image
    std::vector<std::vector<cv::Point2f> > imgpoints;

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
    std::string path = std::string(argv[1])+"/*.png";

    cv::glob(path, images);

    cv::Mat frame, gray;
    // vector to store the pixel coordinates of detected checker board corners
    std::vector<cv::Point2f> corner_pts;
    bool success;

    // Looping over all the images in the directory
    for(size_t i=0; i<images.size(); i++)
    {
        std::cout<<"Reading.."<<images[i]<<std::endl;
        frame = cv::imread(images[i]);
        cv::cvtColor(frame,gray,cv::COLOR_BGR2GRAY);

        // Finding checker board corners
        // If desired number of corners are found in the image then success = true
        success = cv::findChessboardCorners(gray, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts, cv::CALIB_CB_ADAPTIVE_THRESH |  cv::CALIB_CB_FAST_CHECK |  cv::CALIB_CB_NORMALIZE_IMAGE);

        /*
     * If desired number of corner are detected,
     * we refine the pixel coordinates and display
     * them on the images of checker board
    */
        if(success)
        {
            cv::TermCriteria criteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.001);

            // refining pixel coordinates for given 2d points.
            cv::cornerSubPix(gray,corner_pts,cv::Size(11,11), cv::Size(-1,-1),criteria);

            // Displaying the detected corner points on the checker board
            cv::drawChessboardCorners(frame, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts, success);

            objpoints.push_back(objp);
            imgpoints.push_back(corner_pts);
        }

        // solvepnp - objectPoints puntos 3d, imagePoints - proyeccion de los puntos 3d, cameraMatrix - matriz de calibracion, 
        // distCoeffs[, rvec[, tvec[, useExtrinsicGuess[, flags]]]]

        //resize image to fit into screen
        cv::Mat resized;
        cv::resize(frame,resized,cv::Size(1920,1080));
        cv::imshow("Image",resized);
        cv::waitKey(10);
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

/*

Reading..images/cal02.png
Reading..images/cal03.png
Reading..images/cal04.png
Reading..images/cal05.png
Reading..images/cal06.png
Reading..images/cal07.png
cameraMatrix : [1072.273634298496, 0, 643.7922806443585;
 0, 1071.656368976598, 353.0155697521899;
 0, 0, 1]
distCoeffs : [-0.01080060333721249, -0.09131745095169139, -0.003052291976085676, 0.0009623812052658284, 0.07472478848400599]
Rotation vector : [-0.2582101617279525, -0.7761768830744923, 1.324122665108325;
 -0.02065848183304236, -0.5750767847752318, 1.40375397306569;
 0.3791320764139204, -0.1561347253273741, 1.506077533239406;
 0.6634044157803364, 0.0414887685480811, 1.521609780784004;
 0.6999427799551863, -0.4356307200814128, 1.517103861691612;
 0.303694739172273, -0.4286308300329283, 1.515615169368669;
 0.1815552198760571, -0.328963570311144, 1.535609155258819]
Translation vector : [-0.1049910186996945, -4.68477962460028, 23.85457148407008;
 3.190866257270221, -2.647349895577722, 17.71341410227143;
 2.79884877643999, -1.5839259354676, 14.97343472361765;
 2.364189668636462, -1.402433532269158, 13.71836577044511;
 4.095329759182556, -0.5431006404991702, 13.95293936521964;
 5.212584052199031, -1.034767956567747, 12.01445281045776;
 4.481788165852007, -1.972005192153121, 9.701043480287769]

*/