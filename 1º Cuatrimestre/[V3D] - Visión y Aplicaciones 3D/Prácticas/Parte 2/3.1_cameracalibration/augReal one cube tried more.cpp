#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

// Defining the dimensions of checkerboard
int CHECKBOARD_SIZE = 2;

/*
    rows: numero de filas
    cols : numero de columnas
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

    // Obtener parámetros desde la línea de comandos
    //int rows = std::stoi(argv[1]);
    //int cols = std::stoi(argv[2]);
    int size = std::stoi(argv[1]);
    std::string intrinsics_yml = argv[2];
    std::string input_video = argv[3];

    int CHECKERBOARD[CHECKBOARD_SIZE] = {5, 8};

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
            objp.push_back(cv::Point3f(j, i, 0));
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

            // Project axis points
            std::vector<cv::Point3f> axisPoints{
                cv::Point3f(0, 0, 0),
                cv::Point3f(size, 0, 0),    // X
                cv::Point3f(0, size, 0),    // Y
                cv::Point3f(0, 0, size)};   // Z

            std::vector<cv::Point2f> imagePoints;
            cv::projectPoints(axisPoints, rvec, tvec, cameraMatrix, distCoeffs, imagePoints);

            // Draw the axis on the image -> red (X axis), green (Y axis), and blue (Z axis)
            cv::line(frame, corner_pts[0], imagePoints[1], cv::Scalar(0, 0, 255), 5); // X-axis (red)
            cv::line(frame, corner_pts[0], imagePoints[2], cv::Scalar(0, 255, 0), 5); // Y-axis (green)
            cv::line(frame, corner_pts[0], imagePoints[3], cv::Scalar(255, 0, 0), 5); // Z-axis (blue)
        }

        cv::imshow("Augmented Reality", frame);

        if (cv::waitKey(10) == 27) // Press ESC to exit
            break;
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}
