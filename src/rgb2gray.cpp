#include <iostream>
#include <cstdlib>
#include <cstdio>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void process(char* imsname, char* imdname) {
    Mat ims = imread(imsname);
    Mat imd;
    cvtColor(ims, imd, CV_BGR2GRAY);
    imwrite(imdname, imd);
}

void usage (const char *s)
{
  fprintf(stderr, "Usage: %s imsname\n", s);
  exit(EXIT_FAILURE);
}

#define param 2
int main( int argc, char* argv[] ) {
  if(argc != (param+1))
    usage(argv[0]);
  process(argv[1], argv[2]);
  return EXIT_SUCCESS;
}
