#include <iostream>
#include <exception>

//OpenCV includes
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
//#include <opencv2/calib3d.hpp> //Uncomment when it was appropiated.
//#include <opencv2/ml.hpp> //Uncomment when it was appropiated.


#include "common_code.hpp"

const char * keys =
    "{help h usage ? |      | print this message}"
    "{w              |20    | Wait time (miliseconds) between frames.}"
    "{v              |      | the input is a video file.}"
    "{c              |      | the input is a camera index.}"    
    "{@input         |<none>| input <fname|int>}"
    ;


int
main (int argc, char* const* argv)
{
  int retCode=EXIT_SUCCESS;
  
  try {    

      cv::CommandLineParser parser(argc, argv, keys);
      parser.about("Show the extremes values and their locations.");
      if (parser.has("help"))
      {
          parser.printMessage();
          return 0;
      }
      bool is_video = parser.has("v");
      bool is_camera = parser.has("c");
      int wait = parser.get<int>("w");
      cv::String input = parser.get<cv::String>("@input");
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
