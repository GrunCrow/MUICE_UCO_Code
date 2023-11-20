#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

// Defining the dimensions of checkerboard
int CHECKBOARD_SIZE = 2;
int NUMBER_ROWS = 5;
int NUMBER_COLS = 8;

/*
    rows: numero de filas
    cols : numero de columnas
    size: tamaño de la arista
    intrinsics.yml: fichero con los parametros intrinsecos
    <input video-file|cam-idx>: video de entrada
*/

void drawCube(cv::Mat& frame, const cv::Mat& rvec, const cv::Mat& tvec, const cv::Mat& cameraMatrix, const cv::Mat& distCoeffs, int size, int row, int col)
{
    // Define cube points in the world coordinate system
    float cubeSize = 1.0f;
    std::vector<cv::Point3f> cubePoints{
        cv::Point3f(0 + cubeSize * row, 0 + cubeSize * col, 0),
        cv::Point3f(cubeSize + cubeSize * row, 0 + cubeSize * col, 0),
        cv::Point3f(cubeSize + cubeSize * row, cubeSize + cubeSize * col, 0),
        cv::Point3f(0 + cubeSize * row, cubeSize + cubeSize * col, 0),
        cv::Point3f(0 + cubeSize * row, 0 + cubeSize * col, -cubeSize),
        cv::Point3f(cubeSize + cubeSize * row, 0 + cubeSize * col, -cubeSize),
        cv::Point3f(cubeSize + cubeSize * row, cubeSize + cubeSize * col, -cubeSize),
        cv::Point3f(0 + cubeSize * row, cubeSize + cubeSize * col, -cubeSize)};

    // Project cube points onto the image plane
    std::vector<cv::Point2f> imagePoints;
    cv::projectPoints(cubePoints, rvec, tvec, cameraMatrix, distCoeffs, imagePoints);

    // Draw the cube
    for (int i = 0; i < 4; ++i)
    {
        cv::line(frame, imagePoints[i], imagePoints[(i + 1) % 4], cv::Scalar(0, 255, 0), 2); // Green lines in the XY plane
        cv::line(frame, imagePoints[i + 4], imagePoints[(i + 1) % 4 + 4], cv::Scalar(0, 255, 0), 2); // Green lines in the XY plane
        cv::line(frame, imagePoints[i], imagePoints[i + 4], cv::Scalar(0, 255, 0), 2); // Green lines connecting the top and bottom of the cube
    }
}

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

    int CHECKERBOARD[CHECKBOARD_SIZE] = {NUMBER_ROWS, NUMBER_COLS};

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

    std::vector<cv::Point3f> objp;
    for (int i = 0; i < CHECKERBOARD[1]; i++)
    {
        for (int j = 0; j < CHECKERBOARD[0]; j++)
            objp.push_back(cv::Point3f(j, i, 0));
    }

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

        std::vector<cv::Point2f> corner_pts;
        bool success = cv::findChessboardCorners(gray, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FAST_CHECK | cv::CALIB_CB_NORMALIZE_IMAGE);

        if (success)
        {
            cv::TermCriteria criteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.001);
            cv::cornerSubPix(gray, corner_pts, cv::Size(11, 11), cv::Size(-1, -1), criteria);

            cv::Mat rvec, tvec;
            cv::solvePnP(objp, corner_pts, cameraMatrix, distCoeffs, rvec, tvec);

            // Axis Point projections
            std::vector<cv::Point3f> axisPoints{
                cv::Point3f(0, 0, 0),
                cv::Point3f(size, 0, 0),    // X
                cv::Point3f(0, size, 0),    // Y
                cv::Point3f(0, 0, size)};   // Z

            std::vector<cv::Point2f> axisImagePoints;
            cv::projectPoints(axisPoints, rvec, tvec, cameraMatrix, distCoeffs, axisImagePoints);

            // Axis
            cv::line(frame, axisImagePoints[0], axisImagePoints[1], cv::Scalar(0, 0, 255), size); // X-axis (red)
            cv::line(frame, axisImagePoints[0], axisImagePoints[2], cv::Scalar(0, 255, 0), size); // Y-axis (green)
            cv::line(frame, axisImagePoints[0], axisImagePoints[3], cv::Scalar(255, 0, 0), size); // Z-axis (blue)

            //for (int i = 0; i < corner_pts.size(); ++i)
            for (int row = 0;row<NUMBER_ROWS - 1; row++){
                for (int col = 0;col < NUMBER_COLS - 1; col++){
                    if (row % 2 == 0 && col % 2 == 0){ // if row par (0, 2, 4...) -> col par
                        // Draw cube
                        drawCube(frame, rvec, tvec, cameraMatrix, distCoeffs, size, row, col);
                    }
                    else if ((row + 1) % 2 == 0 && (col + 1) % 2 == 0){ // if row impar (1, 3, ...) -> col impar
                        // Draw cube
                        drawCube(frame, rvec, tvec, cameraMatrix, distCoeffs, size, row, col);
                    }
                    //else: nothing
                }
            }

            // Draw chessboard corners
            //cv::drawChessboardCorners(frame, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts, success);
        }

        cv::imshow("3D Information", frame);

        if (cv::waitKey(10) == 27)
            break;
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}
