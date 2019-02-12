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
#include "utils.h"

typedef std::pair<cv::Mat_<int>, cv::Mat_<int>> ParMat;
void setOpticalFlowData(vector<Mat>& frames);

vector<ParMat> data;
const string DATA_FOLDER = "data/";
const string FRAMES_FOLDER = "data/frames/";
const unsigned int  N_FRAMES_TEST = 2;

int main(int argc, const char * argv[]) {
    // insert code here...
    int step = 5;
    // string path = "/Users/davidchoqueluqueroman/Desktop/CURSOS-MASTER/IMAGENES/descriptor/data/person23.mp4";
    string path = "data/person23.mp4";
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

    setOpticalFlowData(frames);
    
    // Mat ld_image;
    // vector<string> names(2);
    // names[0] = to_string(15);
    // names[0] = to_string(20);
    // load_frames_to_vector(FRAMES_FOLDER, frames,names);
    // cout<<"points: "<<points[0].size()<<endl;
    
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
void FillPoints(vector<Point2f> &vecPoints, Mat frameB, Mat frameA, int thr) {
  vecPoints.clear();
  cvtColor(frameB, frameB, CV_BGR2GRAY);
  cvtColor(frameA, frameA, CV_BGR2GRAY);

  cv::Mat frameDif = cv::abs(frameB - frameA);
  for (int i = 0; i < frameDif.rows; ++i) {
    for (int j = 0; j< frameDif.cols; ++j) {
        cout<<"diference: "<<(int)(frameDif.at<uchar>(i, j))<<endl;
      if (frameDif.at<uchar>(i, j) > thr)
        vecPoints.push_back(Point2f(static_cast<float>(j),static_cast<float>(i)));
    }
  }
  cout<<"real_pixels: "<<frameDif.rows*frameDif.cols<<", final pixels: "<<vecPoints.size()<<endl;
}

void setOpticalFlowData(vector<Mat>& frames) {
    // data.clear();
    vector<Point2f> points[2];
    vector<Point2f> basePoints;
    vector<uchar> status;
    vector<float> err;
    Size winSize(31, 31);
    TermCriteria termcrit(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 20,0.3);
    int rows, cols;
    
    rows = frames[0].rows;
    cols = frames[0].cols;
    
    int numImgs = (int)frames.size();
    
    
    int ** mapToOpticalFlows = allocateMapToOpticalFlowsMatrix(numImgs);
    vector<int> temporalScales;
    temporalScales.push_back(1);
    temporalScales.push_back(2);
    for (int t : temporalScales) {
        
        for (int i = 0; i <frames.size(); i++) {
            
            int j = i + t;  // image to process with i

            if (j < frames.size()) {
                FillPoints(points[0], frames[j], frames[i],5);
                
                ParMat angles_magni;
                angles_magni.first = cv::Mat(rows, cols, CV_16SC1, -1);  // angles
                angles_magni.second = cv::Mat(rows, cols, CV_16SC1, -1);  // magnitude
                
                if (points[0].size() > 0) {
                    calcOpticalFlowPyrLK(frames[i], frames[j], points[0], points[1],
                                             status, err, winSize, 3, termcrit, 0, 0.001);

                    VecDesp2Mat(points[1], points[0], angles_magni);
                }
                
                // this->data.push_back(angles_magni);
                // this->mapToOpticalFlows[i][j] = static_cast<int>(this->data.size()) - 1;
                // this->mapToOpticalFlows[j][i] = static_cast<int>(this->data.size()) - 1;
            } else {
                break;
            }
        }
    }
}

void VecDesp2Mat(vector<Point2f> &vecPoints,vector<Point2f> &positions,ParMat & AMmat) {
  
    int x, y, valAngle, valMagnitude;
    float magnitude, angle, catetoOposto, catetoAdjacente;

    for (int i = 0; i < positions.size(); ++i){
        catetoOposto = vecPoints[i].y - positions[i].y;
        catetoAdjacente = vecPoints[i].x - positions[i].x;

        magnitude = sqrt((catetoAdjacente * catetoAdjacente) + (catetoOposto * catetoOposto));

        angle = atan2f(catetoOposto, catetoAdjacente) * 180 /CV_PI;

        if (angle < 0){
            angle += 360;
        }

        valAngle = floor(angle / (this->maxAngle /this->nBinsAngle)));

        if (logQuantization == 1)
        valMagnitude = static_cast<int>(floor(log2(magnitude)));
        else
        valMagnitude = static_cast<int>(floor(magnitude / (this->maxMagnitude /
        this->nBinsMagnitude)));

        if (valMagnitude < 0)  // e.g., log2(0)
        valMagnitude = 0;  // send to the first bin

        if (valMagnitude >= this->nBinsMagnitude)
        valMagnitude = this->nBinsMagnitude - 1;  // send to the last bin

        y = static_cast<int>(positions[i].y);
        x = static_cast<int>(positions[i].x);

        AMmat.first(y, x) = valAngle;
        AMmat.second(y, x) = valMagnitude;
    }
}
