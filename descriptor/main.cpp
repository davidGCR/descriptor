//
//  main.cpp
//  descriptor
//
//  Created by David Choqueluque Roman on 2/11/19.
//  Copyright © 2019 David Choqueluque Roman. All rights reserved.
//
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "opencv2/videoio.hpp"
#include "opencv2/video/tracking.hpp"
using namespace cv;
using namespace std;
#include <iostream>
#include "sample_cube.h"
#include "opticalFlowSet.h"
#include "utils.h"
#include "haralick.h"

// typedef std::pair<cv::Mat_<int>, cv::Mat_<int>> ParMat;
// void setOpticalFlowData(vector<Mat>& frames);
// void VecDesp2Mat(vector<Point2f> &vecPoints,vector<Point2f> &positions,ParMat & AMmat);

// vector<ParMat> data;
const string DATA_FOLDER = "data/";
const string FRAMES_FOLDER = "data/frames/";
const unsigned int  N_FRAMES_TEST = 5;

Mat co_ocurrence_magnitud(Mat matrix, int orientation);
void print_haralick_features(Mat haralick_features);
void generate_cuboids(vector<Mat> &listCuboidAngles, vector<Mat> &listCuboidMagnitudes, Mat angles, Mat magnitudes,int frame_size);
Mat normalize_matriz(Mat matriz);

int main(int argc, const char * argv[]) {
    // insert code here...
    int step = 5;
    // string path = "/Users/davidchoqueluqueroman/Desktop/CURSOS-MASTER/IMAGENES/descriptor/data/person23.mp4";
    string path = "data/person23_running_d4_uncomp.avi";
    VideoCapture capture;
    capture.open(path);
    if (!capture.isOpened())
        std::cerr << "Error processing file. Can't read video " << path;
    
    int videoLength = (capture.get(CV_CAP_PROP_FRAME_COUNT));
    int videoWidth = (capture.get(CV_CAP_PROP_FRAME_WIDTH));
    int videoHeight = (capture.get(CV_CAP_PROP_FRAME_HEIGHT));
    Mat image;
    vector<Mat> frames;
    for (int64 frameStep = 0; frameStep < videoLength; frameStep += step) {
        capture.set(CV_CAP_PROP_POS_FRAMES, frameStep);
        capture.read((image));
        if (image.empty())
            std::cerr << "Error processing file. Can't read frame " << frameStep
            << "from video %s" << path;
        
        if(frames.size()<N_FRAMES_TEST){
            Mat img = image.clone();
            frames.push_back(img);
            save_frame("data/frames/",to_string(frameStep),img);
        }
        else
        {
            break;
        }
        imshow("imagen", image);
        waitKey(100);
    }


    
    int w = 48;
    int h = 48;
    int l = 10;
    int strideX = 24;
    int strideY = 24;
    int strideL = 5;
    vector<Sample_Cube> cubos;
    // video.size() instead of videoLength since used a frameStep different from 1
    for (int t = 0; t <= static_cast<int>(0 + frames.size() - l);t += strideL){
        for (int y = 0; y <= static_cast<int>(0 + videoHeight - h);y += strideY){
            for (int x = 0; x <= static_cast<int>(0 + videoWidth - w);x += strideX){
//cout<<"x: "<<x<<endl;
                cubos.push_back(Sample_Cube(x, y, t, w, h, l));
            }
        }
    }
    
    cout<<"frames Size: "<<frames.size()<<endl;
    cout<<"video Height: "<<videoHeight<<endl;
    cout<<"video Width: "<<videoWidth<<endl;
    cout<<"N cubos: "<<cubos.size()<<endl;
    
    vector<Point2f> points[2];

    OpticalFlowSet osf(frames.size());
    osf.calculateOpticalFlow(frames);

    cout<<"total magnitudes pair: "<<osf.angles_magnitudes.size()<<endl;

    //export_mat_excel(osf.angles_magnitudes[1].angles, "angles");
    //export_mat_excel(osf.angles_magnitudes[1].magnitudes, "magnitudes");


//Seccion of Haralick
    Haralick har;
    int size_frame = 20;

    vector<Mat> listCuboidAngles;
    vector<Mat> listCuboidMagnitudes;

    Mat zeros = Mat::zeros(size_frame, size_frame, CV_32FC1);
    Mat sampling_angles, sampling_magnitudes;

    generate_cuboids(listCuboidAngles, listCuboidMagnitudes, osf.angles_magnitudes[3].angles, osf.angles_magnitudes[3].magnitudes,size_frame);

    cout << listCuboidAngles.size() << endl;
    cout << listCuboidMagnitudes.size() << endl;

    int orientation = 0;

    vector<Mat> list_haralick_angles;
    vector<Mat> list_haralick_magnitudes;
//Step 3 Calculate de co_ocurrence matrix of angles and magnitudes
    for(int i=0; i<listCuboidAngles.size(); i++)
    {
        Mat co_ocurrence_angle = co_ocurrence_magnitud(listCuboidAngles[i], orientation);
        //export_mat_excel(co_ocurrence_angle, "co_ocurrence_angle" );
        Mat co_ocurrence_magnitude = co_ocurrence_magnitud(listCuboidMagnitudes[i], orientation);

//Normalize matrix to input to haralick
        Mat co_ocurrence_angle_n = normalize_matriz(co_ocurrence_angle);
        Mat co_ocurrence_magnitude_n = normalize_matriz(co_ocurrence_magnitude);

        //export_mat_excel(listCuboidAngles[0], "cuboid_angle");
        //export_mat_excel(co_ocurrence_angle_n,"co_ocurrence_angle_n");

//Step 4 Extract haralick features
        Mat haralick_angles = har.calculate(co_ocurrence_angle_n);
        Mat haralick_magnitudes = har.calculate(co_ocurrence_magnitude_n);

        list_haralick_angles.push_back(haralick_angles);
        list_haralick_magnitudes.push_back(haralick_magnitudes);   
    }

    //print_haralick_features(haralick_angles);
    //print_haralick_features(haralick_magnitudes);

    cout << "list: " << list_haralick_angles.size() << endl;
    
    //cout << "cols: " << osf.angles_magnitudes[1].angles.cols << endl;    


    // setOpticalFlowData(frames);
    
    // Mat ld_image;
    // vector<string> names(2);
    // names[0] = to_string(15);
    // names[0] = to_string(20);
    // load_frames_to_vector(FRAMES_FOLDER, frames,names);
    // cout<<"points: "<<points[0].size()<<endl;
    
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
            co_magnitud.at<float>(matrix.at<int>(i,k), matrix.at<int>(i+i_orient,k+j_orient))++;
        }
    }

    return co_magnitud;
}

void print_haralick_features(Mat haralick_features)
{
    cout << "f1: "<< haralick_features.at<float>(0,0) << endl;
    cout << "f2: "<< haralick_features.at<float>(0,1) << endl;
    cout << "f3: "<< haralick_features.at<float>(0,2) << endl;
    cout << "f4: "<< haralick_features.at<float>(0,3) << endl;
    cout << "f5: "<< haralick_features.at<float>(0,4) << endl;
    cout << "f6: "<< haralick_features.at<float>(0,5) << endl;
    cout << "f7: "<< haralick_features.at<float>(0,6) << endl;
    cout << "f8: "<< haralick_features.at<float>(0,7) << endl;
    cout << "f9: "<< haralick_features.at<float>(0,8) << endl;
    cout << "f10: "<< haralick_features.at<float>(0,9) << endl;
    cout << "f11: "<< haralick_features.at<float>(0,10) << endl;
    cout << "f12: "<< haralick_features.at<float>(0,11) << endl;
    cout << "f13: "<< haralick_features.at<float>(0,12) << endl;
    cout << "f14: "<< haralick_features.at<float>(0,13) << endl;
}

void generate_cuboids(vector<Mat> &listCuboidAngles, vector<Mat> &listCuboidMagnitudes, Mat angles, Mat magnitudes,int frame_size)
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

Mat normalize_matriz(Mat matriz)
{
    int sum=0;
    for(int i=0; i<matriz.rows; i++)
    {
        for(int j=0; j<matriz.cols; j++)
        {
            sum += matriz.at<float>(i,j);
        }
    }

    return matriz/sum;
}