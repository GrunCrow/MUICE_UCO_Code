/**
 * @file boilerplate.cpp
 * @author Francisco José Madrid Cuevas (fjmadrid@uco.es)
 * @brief Boilerplate to code practical assignment's.
 * @version 0.1
 * @date 2023-11-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <exception>

//OpenCv includes.
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
//#include <opencv2/calib3d.hpp> //Uncomment when it was appropiated.
//#include <opencv2/ml.hpp> //Uncomment when it was appropiated.

const char * keys =
    "{help h usage ? |      | print this message   }"
    "{path           |.     | path to file         }"
    "{fps            | -1.0 | fps for output video }"
    "{N count        |100   | count of objects     }"
    "{ts timestamp   |      | use time stamp       }"
    "{@image1        |      | image1 for compare   }"
    "{@image2        |<none>| image2 for compare   }"
    "{@repeat        |1     | number               }"
    ;

int
main (int argc, char* const* argv)
{
  int retCode=EXIT_SUCCESS;
  
  try {    

      cv::CommandLineParser parser(argc, argv, keys);
      parser.about("Application name v1.0.0");
      if (parser.has("help"))
      {
          parser.printMessage();
          return 0;
      }
      int N = parser.get<int>("N");
      double fps = parser.get<double>("fps");
      cv::String path = parser.get<cv::String>("path");
      bool use_time_stamp = parser.has("timestamp");
      cv::String img1 = parser.get<cv::String>("@image1");
      cv::String img2 = parser.get<cv::String>("@image2");
      int repeat = parser.get<int>("@repeat");
      if (!parser.check())
      {
          parser.printErrors();
          return 0;
      }

    // TODO


    //    
  }
  catch (std::exception& e)
  {
    std::cerr << "Caught exception: " << e.what() << std::endl;
    retCode = EXIT_FAILURE;
  }
  catch (...)
  {
    std::cerr << "Caught unknown exception!" << std::endl;
    retCode = EXIT_FAILURE;
  }
  return retCode;
}
