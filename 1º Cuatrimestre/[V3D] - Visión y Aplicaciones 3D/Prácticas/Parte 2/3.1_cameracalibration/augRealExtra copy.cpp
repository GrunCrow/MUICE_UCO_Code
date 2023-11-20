#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

// Definiendo las dimensiones del tablero de ajedrez
int CHECKERBOARD_SIZE = 2;

/*
    rows: numero de filas
    cols: numero de columnas
    size: tamaño de la arista
    intrinsics.yml: fichero con los parametros intrinsecos
    <input video-file|cam-idx>: video de entrada
*/

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: augReal size intrinsics.yml <input video-file|cam-idx>" << std::endl;
        return -1;
    }

    int size = std::stoi(argv[1]);
    std::string intrinsics_yml = argv[2];
    std::string input_video = argv[3];

    int CHECKERBOARD[CHECKERBOARD_SIZE] = {5, 8};

    // Load intrinsic parameters from the provided .yml file
    cv::FileStorage fs(intrinsics_yml, cv::FileStorage::READ);
    if (!fs.isOpened())
    {
        std::cerr << "Failed to open file: " << intrinsics_yml << std::endl;
        return -1;
    }

    cv::Mat cameraMatrix, distCoeffs;
    fs["cameraMatrix"] >> cameraMatrix;
    fs["distCoeffs"] >> distCoeffs;
    fs.release();

    // Create vector to store 3D points of the board
    std::vector<cv::Point3f> objp;
    for (int i = 0; i < CHECKERBOARD[1]; i++)
    {
        for (int j = 0; j < CHECKERBOARD[0]; j++)
            objp.push_back(cv::Point3f(j * size, i * size, 0)); // Use size as the scaling factor
    }

    // Create video capture object
    cv::VideoCapture cap(input_video);
    if (!cap.isOpened())
    {
        std::cerr << "Error opening video file or camera" << std::endl;
        return -1;
    }

    cv::Mat frame, gray;

    while (cap.read(frame))
    {
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        // Detect the board
        std::vector<cv::Point2f> corner_pts;
        bool success = cv::findChessboardCorners(gray, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FAST_CHECK | cv::CALIB_CB_NORMALIZE_IMAGE);

        if (success)
        {
            // Refine the corners
            cv::TermCriteria criteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.001);
            cv::cornerSubPix(gray, corner_pts, cv::Size(11, 11), cv::Size(-1, -1), criteria);

            // Estimate the camera pose with respect to the board
            cv::Mat rvec, tvec;
            cv::solvePnP(objp, corner_pts, cameraMatrix, distCoeffs, rvec, tvec);

            // Project cube points
            std::vector<cv::Point3f> cubePoints{
                // BACK   
                cv::Point3f(0, 0, 0),           // Bottom left corner of the cube
                cv::Point3f(size, 0, 0),        // Bottom right corner of the cube
                cv::Point3f(size, size, 0),     // Top right corner of the cube
                cv::Point3f(0, size, 0),        // Top left corner of the cube

                // FRONT
                cv::Point3f(0, 0, -size),       // Bottom left corner of the cube
                cv::Point3f(size, 0, -size),    // Bottom right corner of the cube
                cv::Point3f(size, size, -size), // Top right corner of the cube
                cv::Point3f(0, size, -size)     // Top left corner of the cube
            };



            std::vector<cv::Point2f> imagePoints;
            cv::projectPoints(cubePoints, rvec, tvec, cameraMatrix, distCoeffs, imagePoints);

            // Draw cubes on the black squares of the calibration pattern
            for (int i = 0; i < corner_pts.size(); i++)
            {
                if (gray.at<uchar>(corner_pts[i]) < 128)
                {
                    // Draw cube edges
                    for (int j = 0; j < 4; j++)
                        cv::line(frame, imagePoints[j], imagePoints[(j + 1) % 4], cv::Scalar(255, 0, 0), 2);
                    
                    for (int j = 0; j < 4; j++)
                        cv::line(frame, imagePoints[j + 4], imagePoints[(j + 1) % 4 + 4], cv::Scalar(255, 0, 0), 2);
                    
                    for (int j = 0; j < 4; j++)
                        cv::line(frame, imagePoints[j], imagePoints[j + 4], cv::Scalar(255, 0, 0), 2);
                }
            }

            // Project axis points
            std::vector<cv::Point3f> axisPoints{
                cv::Point3f(0, 0, 0),
                cv::Point3f(size, 0, 0),    // X
                cv::Point3f(0, size, 0),    // Y
                cv::Point3f(0, 0, size)};   // Z

            std::vector<cv::Point2f> axisImagePoints;
            cv::projectPoints(axisPoints, rvec, tvec, cameraMatrix, distCoeffs, axisImagePoints);

            // Draw the axis on the image -> red (X axis), green (Y axis), and blue (Z axis)
            cv::line(frame, axisImagePoints[0], axisImagePoints[1], cv::Scalar(0, 0, 255), 5); // X-axis (red)
            cv::line(frame, axisImagePoints[0], axisImagePoints[2], cv::Scalar(0, 255, 0), 5); // Y-axis (green)
            cv::line(frame, axisImagePoints[0], axisImagePoints[3], cv::Scalar(255, 0, 0), 5); // Z-axis (blue)
        }

        cv::imshow("Augmented Reality", frame);

        if (cv::waitKey(10) == 27) // Press ESC to exit
            break;
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}
