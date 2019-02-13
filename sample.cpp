#include <iostream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <time.h>
#include <fstream>
#include "descriptor/haralick.h"

using namespace std;
using namespace cv;

void generate_cuboid(vector<Mat> &listCuboidAngles, vector<Mat> &listCuboidMagnitudes, Mat angles, Mat magnitudes, int frame_size);

int main()
{
	cout << "Print " << endl;
	Mat matriz = Mat::zeros(120, 160, CV_32FC1);

	int frame_size = 20;

	vector<Mat> listCuboidAngles;
	vector<Mat> listCuboidMagnitudes;

	generate_cuboid(listCuboidAngles, listCuboidMagnitudes, matriz, matriz,frame_size);

	cout << listCuboidAngles.size() << endl;
	cout << listCuboidMagnitudes.size() << endl;
	

	return 0;
}

void generate_cuboid(vector<Mat> &listCuboidAngles, vector<Mat> &listCuboidMagnitudes, Mat angles, Mat magnitudes,int frame_size)
{	
	for(int h=0; h<(magnitudes.rows/frame_size); h++)
	{		
		for(int k=0; k<(magnitudes.cols/frame_size); k++)
		{		
			Mat angles_cuboid = Mat::zeros(frame_size, frame_size, CV_32FC1);
			Mat magnitude_cuboid = Mat::zeros(frame_size, frame_size, CV_32FC1);
			for(int i=h*frame_size; i<(h+1)*frame_size; i++)
			{
				for(int j=k*frame_size; j<(k+1)*frame_size; j++)
				{
					angles_cuboid.at<float>(i-h*frame_size,j-k*frame_size) = angles.at<float>(i,j);
					magnitude_cuboid.at<float>(i-h*frame_size,j-k*frame_size) = magnitudes.at<float>(i,j);
				}
			}
			listCuboidAngles.push_back(angles_cuboid);
			listCuboidMagnitudes.push_back(magnitude_cuboid);
		}
	}
}