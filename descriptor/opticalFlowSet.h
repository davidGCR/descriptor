#ifndef optical_flow_set_h
#define optical_flow_set_h

#include<iostream>
#include<vector>
#include"angles_magnitude.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "opencv2/videoio.hpp"
#include "opencv2/video/tracking.hpp"
using namespace cv;
using namespace std;

using namespace std;


class OpticalFlowSet{

private:
    /* data */
    int** map_img_img; //guarda indice de matri
    

public:
    vector<Angles_Magnitude> angles_magnitudes;


    OpticalFlowSet();
    OpticalFlowSet(int no_frames);
    ~OpticalFlowSet();
    void setOpticalFlow(int image1, int image, Angles_Magnitude a_m);
    void calculateOpticalFlow(vector<Mat>& frames);
    void select_important_pixels(vector<Point2f> &vecPoints, Mat frameB, Mat frameA, int thr);
    void create_angles_magnitudes_from_LK(vector<Point2f> &vecPoints,vector<Point2f> &initial_positions,Angles_Magnitude & AMmat);
    void plot_optical_flow(Mat input_img, Mat output_img, vector<Point2f> points1,vector<Point2f> points2);
};



/* ========================== Definitions ==============================*/
void OpticalFlowSet::plot_optical_flow(Mat input_img, Mat output_img, vector<Point2f> points1,vector<Point2f> points2){
    output_img = input_img.clone();
    for (int i = 0; i < points2.size(); i++) {
        // line(output_img, points1[i], points2[i], Scalar(0, 255, 255),1, 1, 0);
        if (abs(points2[i].x - points1[i].x) > 0) {
            line(output_img, points1[i], points2[i], Scalar(0, 255, 255),1, 1, 0);
            // circle(output_img, points1[i], 2, Scalar(255, 0, 0), 1, 1,0);
            // line(output_img, points1[i], points2[i], Scalar(0, 0, 255),1, 1, 0);
            // circle(output_img, points1[i], 1, Scalar(255, 0, 0), 1, 1,0);
        } 
        // else {
        //     line(output_img, points1[i], points2[i], Scalar(0, 255, 0),1, 1, 0);
        //     circle(output_img, points1[i], 2, Scalar(255, 0, 0), 1, 1,0);
        //     line(output_img, points1[i], points2[i], Scalar(0, 255, 0),1, 1, 0);
        //     circle(output_img, points1[i], 1, Scalar(255, 0, 0), 1, 1,0);
        // }
    }
    imshow("optical flow",output_img);
    waitKey(1500);
}
OpticalFlowSet::OpticalFlowSet(int no_frames)
{
    map_img_img = new int*[no_frames];
    for (int i = 0; i < no_frames; i++) {
        map_img_img[i] = new int[no_frames];
        memset(map_img_img[i], 0, sizeof(int) * no_frames);
    }
}

OpticalFlowSet::~OpticalFlowSet()
{
}
void OpticalFlowSet::setOpticalFlow(int image1, int image2, Angles_Magnitude a_m){
    angles_magnitudes.push_back(a_m);
    map_img_img[image1][image2] = angles_magnitudes.size()-1;
     map_img_img[image2][image1] = angles_magnitudes.size()-1;
}

void OpticalFlowSet::calculateOpticalFlow(vector<Mat>& frames) {
    // data.clear();
    vector<Point2f> points[2];
    // vector<Point2f> basePoints;
    vector<uchar> status;
    vector<float> err;
    Size winSize(31, 31);
    TermCriteria termcrit(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 20,0.3);
    int rows, cols;
    
    rows = frames[0].rows;
    cols = frames[0].cols;
    
    int numImgs = (int)frames.size();
    

    vector<int> temporalScales;
    temporalScales.push_back(1);
    // temporalScales.push_back(2);
    for (int t : temporalScales) {
        
        for (int i = 0; i <frames.size(); i++) {
            
            int j = i + t;  // image to process with i

            if (j < frames.size()) {
                //aplicar mascara (segun paper)
                select_important_pixels(points[0], frames[j], frames[i],100);
                
                Angles_Magnitude angles_magni;
                angles_magni.angles = Mat(rows, cols, CV_16SC1, -1);  // angles
                angles_magni.magnitudes = Mat(rows, cols, CV_16SC1, -1);  // magnitude
                
                if (points[0].size() > 0) {
                    Mat image1 = frames[i];
                    Mat image2 = frames[j];
                    // cvtColor(image1, image1, CV_RGB2GRAY);
                    // cvtColor(image2, image2, CV_RGB2GRAY);
                    calcOpticalFlowPyrLK(image1, image2, points[0], points[1],status, err, winSize, 3, termcrit, 0, 0.001);
                    

                    Mat opt_flow;
//                    cout<<"chanels: "<<image1.channels()<<endl;
//                    cout<<"points0: "<<points[0].size()<<", points1: "<<points[1].size()<<endl;
                    
                    plot_optical_flow(frames[i],opt_flow,points[0],points[1]);
                    
                    //calcular angulos y magnitudes 
                    // for(int i = 0; i < points[0].size(); i++)
                    // {
                    //     // cout<<"pixel1: "<<points[0][i]<<"pixel2:"<<points[1][i]<<endl;
                    // }
                    
                    create_angles_magnitudes_from_LK(points[1], points[0], angles_magni);
                    // //guardar indices en matriz y guardar matrices de angulos y magnitudes
                    setOpticalFlow(i,j,angles_magni);
                }
            } else {
                break;
            }
        }
    }
}

void OpticalFlowSet::select_important_pixels(vector<Point2f> &vecPoints, Mat frameB, Mat frameA, int thr) {
  vecPoints.clear();
  cvtColor(frameB, frameB, CV_BGR2GRAY);
  cvtColor(frameA, frameA, CV_BGR2GRAY);

  cv::Mat frameDif = cv::abs(frameB - frameA);

  for (int i = 0; i < frameDif.rows; ++i) {
    for (int j = 0; j< frameDif.cols; ++j) {
        
        // cout<<"diference: "<<(int)(frameDif.at<uchar>(i, j))<<endl;
      
        if (frameDif.at<uchar>(i, j) > thr){
            vecPoints.push_back(Point2f(static_cast<float>(j),static_cast<float>(i)));
        }
    }
  }
  cout<<"real_pixels: "<<frameDif.rows*frameDif.cols<<", final pixels: "<<vecPoints.size()<<endl;
}

void OpticalFlowSet::create_angles_magnitudes_from_LK(vector<Point2f> &vecPoints,vector<Point2f> &initial_positions,Angles_Magnitude & AMmat) {
    
    int x, y, valAngle, valMagnitude;
    float magnitude, angle, catetoOposto, catetoAdjacente;

    float logQuantization = 1;
    float maxAngle = 361.0;
    int nBinsAngle = 8;
    int nBinsMagnitude = 8;
    int maxMagnitude = 15;
  // max val of Angle is 360. We sum 1 more to fit on nBinsAngle.
  // If 360 / (360 / 4) it will go to the bin 4, but it is from 0 to 3.
  // (int)floor(angle / (this->maxAngle / this->nBinsAngle));


    for (int i = 0; i < initial_positions.size(); ++i){
        catetoOposto = vecPoints[i].y - initial_positions[i].y;
        catetoAdjacente = vecPoints[i].x - initial_positions[i].x;

        magnitude = sqrt((catetoAdjacente * catetoAdjacente) + (catetoOposto * catetoOposto));

        angle = atan2f(catetoOposto, catetoAdjacente) * 180 /CV_PI;

        if (angle < 0){
            // cout<<"negative angle: "<<angle;
            angle += 360;
        }

        valAngle = floor(angle / (maxAngle /nBinsAngle));

        // cout<<"raw_angle: "<<angle<<", valAngle: "<<valAngle<<", magnitude: "<<magnitude<<endl;

         if (logQuantization == 1){
             valMagnitude = floor(log2(magnitude));
         }
         else{
             valMagnitude = static_cast<int>(floor(magnitude / (maxMagnitude /nBinsMagnitude)));
         }
        
        if (valMagnitude < 0){// e.g., log2(0)
            valMagnitude = 0;  // send to the first bin
        }  

        if (valMagnitude >= nBinsMagnitude){
            valMagnitude = nBinsMagnitude - 1;  // send to the last bin
        }
        y = initial_positions[i].y;
        x = initial_positions[i].x;

        //cout<<"angle: "<<valAngle<<", magnitude: "<<valMagnitude<<endl;
        AMmat.angles(y, x) = valAngle;
        AMmat.magnitudes(y, x) = valMagnitude;
    }
}

#endif
