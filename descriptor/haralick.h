#ifndef haralick_h
#define haralick_h

#include <iostream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;

class Haralick{
private:

public:
	Haralick();
	Mat calculate(Mat matriz);
	float f1ASM(Mat matriz);

	//float f2Constrast(Mat matriz);
	//float f3Correlation(Mat matriz);
	//float f4Variance(Mat matriz);
	//float f5IDM(Mat matriz);
	//float f6SumAverage(Mat matriz);
	//float f7Variance(Mat matriz);
	//float f8SumEntropy(Mat matriz);
	//float f9Entropy(Mat matriz);
	//float f10DifferenceVariance(Mat matriz);
	//float f11DifferenceEntropy(Mat matriz);
	//float f12InformationCorrelation01(Mat matriz);
	//float f13InformationCorrelation02(Mat matriz);
	//float f15Directionality(Mat matriz);
};

/*
Angular Second Momento(ASM)
Medida del segundo momento angular, medidad de homogeneidad de la imagen
*/

Haralick::Haralick()
{

}

Mat Haralick::calculate(Mat matriz)
{
	Mat features = Mat::zeros(1,15, CV_32FC1);
	features.at<float>(0,0) = Haralick::f1ASM(matriz);
	return features;
}

float Haralick::f1ASM(Mat matriz)
{
	float sum = 0;
	for(int i=0; i<matriz.rows; i++)
	{
		for(int j=0; j<matriz.cols; j++)
		{
			sum += matriz.at<float>(i,j)*matriz.at<float>(i,j);
		}
	}

	return sum;
}

#endif
