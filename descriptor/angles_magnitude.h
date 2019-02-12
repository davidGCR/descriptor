
#ifndef angles_magnitude_h
#define angles_magnitude_h

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace std;

class Angles_Magnitude{

private:
    /* data */
public:
    Mat_<int> angles;
    Mat_<int> magnitudes;
    Angles_Magnitude(/* args */);
    ~Angles_Magnitude();
};

Angles_Magnitude::Angles_Magnitude(/* args */)
{
}

Angles_Magnitude::~Angles_Magnitude()
{
}
#endif