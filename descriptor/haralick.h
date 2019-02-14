#ifndef haralick_h
#define haralick_h

#include <iostream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;

#define HARALICK_EPSILON 0.00001

class Haralick{
private:

public:
	Haralick();
	Mat calculate(Mat matriz);
	float f1ASM(Mat matriz);
	float f2Constrast(Mat matriz);
	float f3Correlation(Mat matriz);
	float f4Variance(Mat matriz);
	float f5IDM(Mat matriz);
	float f6SumAverage(Mat matriz);
	float f7SumVariance(Mat matriz);
	float f8SumEntropy(Mat matriz);
	float f9Entropy(Mat matriz);
	float f10DifferenceVariance(Mat matriz);
	float f11DifferenceEntropy(Mat matriz);
	float f12InformationCorrelation(Mat matriz);
	float f13InformationCorrelation(Mat matriz);
	float f15Directionality(Mat matriz);
};



Haralick::Haralick()
{

}

Mat Haralick::calculate(Mat matriz)
{
	Mat features = Mat::zeros(1,15, CV_32FC1);
	features.at<float>(0,0) = Haralick::f1ASM(matriz);
	features.at<float>(0,1) = Haralick::f2Constrast(matriz);
	features.at<float>(0,2) = Haralick::f3Correlation(matriz);
	features.at<float>(0,3) = Haralick::f4Variance(matriz);
	features.at<float>(0,4) = Haralick::f5IDM(matriz);
	features.at<float>(0,5) = Haralick::f6SumAverage(matriz);
	features.at<float>(0,6) = Haralick::f7SumVariance(matriz);
	features.at<float>(0,7) = Haralick::f8SumEntropy(matriz);
	features.at<float>(0,8) = Haralick::f9Entropy(matriz);
	features.at<float>(0,9) = Haralick::f10DifferenceVariance(matriz);
	features.at<float>(0,10) = Haralick::f11DifferenceEntropy(matriz);
	features.at<float>(0,11) = Haralick::f12InformationCorrelation(matriz);
	features.at<float>(0,12) = Haralick::f13InformationCorrelation(matriz);
	features.at<float>(0,13) = Haralick::f15Directionality(matriz);

	for(int i=0; i<features.cols; i++)
	{
		if(isnan(features.at<float>(0,i)))
			features.at<float>(0,i) = 0.0f;
	}

	return features;
}

/*
Angular Second Momento(ASM)
Medida del segundo momento angular, medidad de homogeneidad de la imagen
*/
float Haralick::f1ASM(Mat matriz)
{
	float sum = 0.0;
	for(int i=0; i<matriz.rows; i++)
	{
		for(int j=0; j<matriz.cols; j++)
		{
			sum += matriz.at<float>(i,j)*matriz.at<float>(i,j);
		}
	}

	return sum;
}


/*
Contraste(CON)
*/
float Haralick::f2Constrast(Mat matriz)
{
	float sum = 0.0;
	float bigsum = 0.0;

	for(int k=0; k < matriz.rows; k++)
	{
		for(int i=0; i<matriz.cols; i++)
		{
			for(int j=0; j<matriz.rows; j++)
			{
				if((i-j) == k || (j-i) == k)
					sum += matriz.at<float>(i,j);
			}
		}
		bigsum += k*k*sum;
		sum = 0;
	}

	return bigsum;
}

/*
Correlation
*/
float Haralick::f3Correlation(Mat matriz)
{
	vector<float> px(matriz.rows, 0);

	for(int i=0; i<matriz.rows; i++)
	{
		for(int j=0; j<matriz.cols; j++)
		{
			px[i] += matriz.at<float>(i,j);
		}
	}

	float meanX = 0.0;
	float sumSqrX = 0.0;

	for(int i=0; i<matriz.rows; i++)
	{
		meanX += px[i]*i;
		sumSqrX += px[i]*i*i;
	}

	float meanY = meanX;

	//cout << "sumSqrX: " << sumSqrX << endl;
	//cout << "meanX: " << meanX << endl;
	//cout << "meanY: " << meanY << endl;

	float stdDevX = sqrt(sumSqrX-(meanX*meanX));
	stdDevX += static_cast<float>(static_cast<float>(HARALICK_EPSILON));
	float stdDevY = stdDevX;

	float sum = 0.0;
	for(int i=0; i<matriz.rows; i++)
	{
		for(int j=0; j<matriz.cols; j++)
		{
			sum += i*j*matriz.at<float>(i,j);
		}
	}


	//cout << "sum: " << sum << endl;
	//cout << "stdDevX: " << stdDevX << endl;
	//cout << "stdDevY: " << stdDevY << endl;

	return (sum-meanX*meanY)/(stdDevX*stdDevY);

}

float Haralick::f4Variance(Mat matriz)
{
	float mean = 0.0;

	for(int i=0; i<matriz.rows; i++)
	{
		for(int j=0; j<matriz.cols; j++)
		{
			mean += i*matriz.at<float>(i,j);
		}
	}


	float variance = 0.0;

	for(int i=0; i<matriz.rows; i++)
	{
		for(int j=0; j<matriz.cols; j++)
		{
			variance += (i+1-mean)*(i+1-mean)*matriz.at<float>(i,j);
		}
	}

	return variance;

}

float Haralick::f5IDM(Mat matriz)
{
	float idm = 0.0;
	for(int i=0; i<matriz.rows; i++)
	{
		for(int j=0; j<matriz.cols; j++)
		{
			idm += matriz.at<float>(i,j)/(1+(i-j)*(i-j));
		}
	}
}

float Haralick::f6SumAverage(Mat matriz)
{
	vector<float> pXY(matriz.rows+matriz.cols+1, 0);

	for(int i=0; i<matriz.rows; i++)
	{
		for(int j=0; j<matriz.cols; j++)
		{
			pXY[i+j+2] += matriz.at<float>(i,j);
		}
	}

	float sumAvg = 0;

	for(int i=2; i<matriz.rows+matriz.cols; i++)
	{	
		sumAvg +=i*pXY[i];
	}

	return sumAvg;
}

float Haralick::f7SumVariance(Mat matriz)
{
	vector<float>pXY(matriz.rows+matriz.cols+1,0);
	for(int i=0; i<matriz.rows; i++)
	{
		for (int j = 0; j < matriz.cols; j++)
		{
			pXY[i+j+2] += matriz.at<float>(i,j);
		}
	}

	float sumAvg = 0.0;

	for(int i=2; i<=matriz.rows+matriz.cols; i++)
	{
		sumAvg += i*pXY[i];
	}

	float sumVariance = 0.0;
	for(int i=2; i<=matriz.rows+matriz.cols; i++)
	{
		sumVariance += (i-sumAvg)*(i-sumAvg)*pXY[i];
	}

	return sumVariance;
}

float Haralick::f8SumEntropy(Mat matriz)
{
	vector<float>pXY(matriz.rows+matriz.cols+1,0);
	for(int i=0; i<matriz.rows; i++)
	{
		for (int j = 0; j < matriz.cols; j++)
		{
			pXY[i+j+2] += matriz.at<float>(i,j);
		}
	}

	float sumEntropy = 0.0;

	for(int i=2; i<=matriz.rows+matriz.cols; i++)
	{
		sumEntropy += pXY[i]*log10(pXY[i]+static_cast<float>(HARALICK_EPSILON));
	}

	return -sumEntropy;

}

float Haralick::f9Entropy(Mat matriz)
{
	float entropy = 0.0;

	for(int i=0; i<matriz.rows; i++)
	{
		for(int j=0; j<matriz.cols; j++)
		{
			entropy += matriz.at<float>(i,j)*log10(matriz.at<float>(i,j)+static_cast<float>(HARALICK_EPSILON));
		}
	}

	return -entropy;
}

float Haralick::f10DifferenceVariance(Mat matriz)
{
	vector<float>pXY(matriz.rows+matriz.cols+1,0);
	for(int i=0; i<matriz.rows; i++)
	{
		for (int j = 0; j < matriz.cols; j++)
		{
			pXY[abs(i-j)] += matriz.at<float>(i,j);
		}
	}

	float sum = 0.0;
	float sumSqr = 0.0;

	for(int i=0; i<matriz.rows; i++)
	{
		sum += pXY[i];
		sumSqr += pXY[i]*pXY[i];
	}

	float tmp = static_cast<float>(matriz.rows*matriz.cols);
	float diffVar = (((tmp*sumSqr)-(sum*sum))/(tmp*tmp));

	return diffVar;
}

float Haralick::f11DifferenceEntropy(Mat matriz)
{
	vector<float>pXY(matriz.rows+matriz.cols+1,0);
	for(int i=0; i<matriz.rows; i++)
	{
		for (int j = 0; j < matriz.cols; j++)
		{
			pXY[abs(i-j)] += matriz.at<float>(i,j);
		}
	}

	float sum = 0.0;

	for(int i=0; i<matriz.rows; i++)
	{
		sum += pXY[i]*log10(pXY[i]+static_cast<float>(HARALICK_EPSILON));
	}

	return -sum;
}

float Haralick::f12InformationCorrelation(Mat matriz)
{
	vector<float> pX(matriz.rows, 0);
	vector<float> pY(matriz.cols, 0);

	for(int i=0; i<matriz.rows; i++)
	{
		for (int j = 0; j < matriz.cols; j++)
		{
			pX[i] += matriz.at<float>(i,j);
			pY[i] += matriz.at<float>(i,j);
		}
	}

	float hx = 0, hy = 0, hxy = 0, hxy1 = 0, hxy2 = 0;
	for(int i=0; i<matriz.rows; i++)
	{
		for (int j = 0; j < matriz.cols; j++)
		{
			hxy1 -= matriz.at<float>(i,j)*log10(pX[i]*pY[j]+static_cast<float>(HARALICK_EPSILON));
			hxy2 -= pX[i]*pY[j]*log10(pX[i]*pY[j]+static_cast<float>(HARALICK_EPSILON));
			hxy -= matriz.at<float>(i,j)*log10(matriz.at<float>(i,j)+static_cast<float>(HARALICK_EPSILON));			
		}
	}

	for(int i=0; i<matriz.rows; i++)
	{
		hx -= pX[i]*log10(pX[i]+static_cast<float>(HARALICK_EPSILON));
		hy -= pY[i]*log10(pY[i]+static_cast<float>(HARALICK_EPSILON));
	}

	return ((hxy-hxy1)/(hx > hy ? hx : hy));

}

float Haralick::f13InformationCorrelation(Mat matriz)
{
	vector<float> pX(matriz.rows, 0);
	vector<float> pY(matriz.cols, 0);

	for(int i=0; i<matriz.rows; i++)
	{
		for (int j = 0; j < matriz.cols; j++)
		{
			pX[i] += matriz.at<float>(i,j);
			pY[i] += matriz.at<float>(i,j);
		}
	}

	float hx = 0, hy = 0, hxy = 0, hxy1 = 0, hxy2 = 0;
	for(int i=0; i<matriz.rows; i++)
	{
		for (int j = 0; j < matriz.cols; j++)
		{
			hxy1 -= matriz.at<float>(i,j)*log10(pX[i]*pY[j]+static_cast<float>(HARALICK_EPSILON));
			hxy2 -= pX[i]*pY[j]*log10(pX[i]*pY[j]+static_cast<float>(HARALICK_EPSILON));
			hxy -= matriz.at<float>(i,j)*log10(matriz.at<float>(i,j)+static_cast<float>(HARALICK_EPSILON));			
		}
	}

	for(int i=0; i<matriz.rows; i++)
	{
		hx -= pX[i]*log10(pX[i]+static_cast<float>(HARALICK_EPSILON));
		hy -= pY[i]*log10(pY[i]+static_cast<float>(HARALICK_EPSILON));
	}

	return static_cast<float>(sqrt(abs(1-exp(-2.0*(hxy2-hxy)))));
}

float Haralick::f15Directionality(Mat matriz)
{
	float sum=0.0;
	for(int i=0; i<matriz.rows; i++)
	{
		sum += matriz.at<float>(i,i);
	}

	return sum;
}

#endif
