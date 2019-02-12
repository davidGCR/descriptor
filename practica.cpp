#include <iostream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <time.h>
#include <fstream>
#include "descriptor/haralick.h"

using namespace std;
using namespace cv;

Mat co_ocurrence_magnitud(Mat matrix, int orientation);
Mat convert_to_0_7(Mat matrix);
void export_mat_excel(Mat img, string name);

int main()
{
	srand(time(NULL));
	Mat magnitude = Mat::zeros(20, 20, CV_32FC1);
	
	for(int i=0; i<magnitude.rows; i++)
	{
		for(int j=0; j<magnitude.cols; j++)
		{
			magnitude.at<float>(i,j) = rand()%256;
		}
	}
	Mat co_magnitude = convert_to_0_7(magnitude);

	int orientation = 3;
	Mat co_ocurrence_matrix = co_ocurrence_magnitud(co_magnitude, orientation);
	
	Haralick har;
	Mat haralick_features = har.calculate(co_ocurrence_matrix);
	cout << haralick_features.at<float>(0,0) << endl;

	export_mat_excel(co_magnitude, "magnitude");
	export_mat_excel(co_ocurrence_matrix, "co_magnitude");

	return 0;
}

//Convert a Magnitud maitrx to 0 to 7 depende of the value

//This a matrix 8 x 8, of the co_ocurrence matrix of de magnitud input
//Orientation 0º 0 45º 1 90º 2 135º 3
Mat co_ocurrence_magnitud(Mat matrix, int orientation)
{
	int i_orient;
	int j_orient;
	int i, j;
	int cols, rows;

	Mat co_magnitud = Mat::zeros(8,8,CV_32FC1);

	if(orientation == 0)
	{
		i_orient = 0;
		j_orient = 1;
		i=0; j=0;
		rows = matrix.rows;
		cols = matrix.cols-1;

	}
	else
	{
		if(orientation == 1)
		{
			i_orient = -1;
			j_orient = 1;
			i=1; j=0;
			rows = matrix.rows;
			cols = matrix.cols-1;
		}
		else
		{
			if(orientation == 2)
			{
				i_orient = -1;
				j_orient = 0;
				i=1; j=0;
				rows = matrix.rows;
				cols = matrix.cols;
			}
			else
			{
				i_orient = -1;
				j_orient = -1;
				i=1; j=1;
				rows = matrix.rows;
				cols = matrix.cols;
			}
		}
	}

	
	for(; i<rows; i++)
	{
		for(int k=j; k<cols; k++)
		{
			co_magnitud.at<float>(int(matrix.at<float>(i,k)), int(matrix.at<float>(i+i_orient,k+j_orient)))++;
		}
	}

	return co_magnitud;
}

void export_mat_excel(Mat img, string name)
{
	ofstream myfile;
  	myfile.open(name+".csv");
	for(int i=0; i<img.rows; i++)
	{
		for(int j=0; j<img.cols; j++)
		{
			myfile << ((Scalar)img.at<float>(i, j))[0];
			myfile << ",";
		}
		myfile << "\n";

	}
	myfile.close();
	//waitKey(5000);
}

Mat convert_to_0_7(Mat matrix)
{
	Mat co_matrix = Mat::zeros(matrix.rows, matrix.cols, CV_32FC1);
	for(int i=0; i<matrix.rows; i++)
	{
		for(int j=0; j<matrix.cols; j++)
		{
			if(matrix.at<float>(i,j) < pow(2,1))
				co_matrix.at<float>(i,j) = 0;
			else
				if(matrix.at<float>(i,j) < pow(2,2))
					co_matrix.at<float>(i,j) = 1;
				else
					if(matrix.at<float>(i,j) < pow(2,3))
						co_matrix.at<float>(i,j) = 2;
					else
						if(matrix.at<float>(i,j) < pow(2,4))
							co_matrix.at<float>(i,j) = 3;
						else
							if(matrix.at<float>(i,j) < pow(2,5))
								co_matrix.at<float>(i,j) = 4;
							else
								if(matrix.at<float>(i,j) < pow(2,6))
									co_matrix.at<float>(i,j) = 5;
								else
									if(matrix.at<float>(i,j) < pow(2,7))
										co_matrix.at<float>(i,j) = 6;
									else
										co_matrix.at<float>(i,j) = 7;

		}
	}

	return co_matrix;
}