#include <opencv2/highgui.hpp>
#include <iostream>

int main(int argc,char **argv){
   std::string pathToVideo=argv[1];
   cv::VideoCapture video(pathToVideo);

   cv::Mat image;
   while(video.grab()){

       video.retrieve(image);
       cv::imshow("image",image);
       cv::waitKey(10);
   }

}