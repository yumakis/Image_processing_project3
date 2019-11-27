#include <iostream>
#include <cstdlib>

#include "morphology.hpp"

using namespace cv;
using namespace std;


void process(const int thmin, const int thmax, const char* imsname, const char* imdname)
{
  Mat ims = imread(imsname);
  cvtColor(ims, ims, CV_BGR2GRAY);


  if(!ims.data){
    cerr<<"Image not found, exit"<<endl;
    exit(EXIT_FAILURE);
  }

  Mat imd = ims.clone();
  int val;

  for(int i = 0; i < ims.rows; i++){
    for(int j = 0; j < ims.cols; j++){
      val=ims.at<uchar>(i,j);
      if (val >= thmin && val < thmax){
        imd.at<uchar>(i,j) = 255;
      }
      if(val < thmin){
        imd.at<uchar>(i,j) = 0;
      }
    }
  }

  imshow(imdname, imd);
  waitKey(0);
  imwrite(imdname, imd);
}

void usage (const char *s)
{
  std::cerr<<"Usage: "<<s<<" thmin thmax ims imd"<<std::endl;
  exit(EXIT_FAILURE);
}

#define param 4

int main( int argc, char* argv[] )
{
  if(argc != (param+1))
    usage(argv[0]);
  process(atoi(argv[1]), atoi(argv[2]), argv[3], argv[4]);
  return EXIT_SUCCESS;
}
