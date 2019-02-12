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

// typedef std::pair<cv::Mat_<int>, cv::Mat_<int>> ParMat;
// void setOpticalFlowData(vector<Mat>& frames);
// void VecDesp2Mat(vector<Point2f> &vecPoints,vector<Point2f> &positions,ParMat & AMmat);

// vector<ParMat> data;
const string DATA_FOLDER = "data/";
const string FRAMES_FOLDER = "data/frames/";
const unsigned int  N_FRAMES_TEST = 3;

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
    for (int64 frameStep = 20; frameStep < videoLength; frameStep += step) {
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

    cout<<"total matirces pair: "<<osf.angles_magnitudes.size()<<endl;
    export_mat_excel(osf.angles_magnitudes[1].angles, "angles");
    export_mat_excel(osf.angles_magnitudes[1].magnitudes, "magnitudes");


    // setOpticalFlowData(frames);
    
    // Mat ld_image;
    // vector<string> names(2);
    // names[0] = to_string(15);
    // names[0] = to_string(20);
    // load_frames_to_vector(FRAMES_FOLDER, frames,names);
    // cout<<"points: "<<points[0].size()<<endl;
    
    return 0;
}






