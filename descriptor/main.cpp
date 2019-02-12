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
using namespace cv;
using namespace std;
#include <iostream>
#include "sample_cube.h"

typedef std::pair<cv::Mat_<int>, cv::Mat_<int>> ParMat;
vector<ParMat> data;


int main(int argc, const char * argv[]) {
    // insert code here...
    int step = 5;
    string path = "/Users/davidchoqueluqueroman/Desktop/CURSOS-MASTER/IMAGENES/descriptor/data/person23.mp4";
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
        
        frames.push_back(image.clone());
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
    
    
    cout<<"video Size: "<<frames.size()<<endl;
    cout<<"video Height: "<<videoHeight<<endl;
    cout<<"video Width: "<<videoWidth<<endl;
    
    cout<<"N cubos: "<<cubos.size()<<endl;
    
    
    return 0;
}

int ** allocateMapToOpticalFlowsMatrix(int numImgs) {
    int ** mapToOpticalFlows = new int*[numImgs];
    for (int i = 0; i < numImgs; i++) {
        mapToOpticalFlows[i] = new int[numImgs];
        memset(mapToOpticalFlows[i], 0, sizeof(int) * numImgs);
    }
    return mapToOpticalFlows;
}

void setOpticalFlowData(vector<Mat>& mImages) {
    data.clear();
    vector<Point2f> points[2], basePoints;
    vector<uchar> status;
    vector<float> err;
    Size winSize(31, 31);
    TermCriteria termcrit(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 20,0.3);
    int rows, cols;
    
    rows = mImages[0].rows;
    cols = mImages[0].cols;
    
    int numImgs = (int)mImages.size();
    
    
    int ** mapToOpticalFlows = allocateMapToOpticalFlowsMatrix(numImgs);
    
    for (int t : this->temporalScales) {
        for (int i = 0; i < static_cast<int>(mImages.size()); i++) {
            int j = i + t;  // image to process with i
            if (j < static_cast<int>(mImages.size())) {
                FillPoints(points[0], mImages[j], mImages[i]);
                
                ParMat angles_magni;
                angles_magni.first = cv::Mat(rows, cols, CV_16SC1, -1);  // angles
                angles_magni.second = cv::Mat(rows, cols, CV_16SC1, -1);  // magnitude
                
                if (points[0].size() > 0) {
                    cv::calcOpticalFlowPyrLK(mImages[i], mImages[j], points[0], points[1],
                                             status, err, winSize, 3, termcrit, 0, 0.001);
                    VecDesp2Mat(points[1], points[0], angles_magni);
                }
                
                this->data.push_back(angles_magni);
                this->mapToOpticalFlows[i][j] = static_cast<int>(this->data.size()) - 1;
                this->mapToOpticalFlows[j][i] = static_cast<int>(this->data.size()) - 1;
            } else {
                break;
            }
        }
    }
}
