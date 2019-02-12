#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "fstream"
#include<iostream>
using namespace cv;
using namespace std;

void save_frame(String data_path, string name,Mat& frame){
    string s = data_path +name+".jpg";
    imwrite(s,frame);
}
void load_frame(string folder, Mat& image){
     string frames_file = folder;
    // string video_file = PATH_DATA+"cam2/anillos.avi";
    
    //initial frame for get size of frames
    vector<cv::String> fn;
    glob(folder +"*.jpg", fn, false);
    size_t count = fn.size(); //total number  files in images folder
    size_t n = 30;
    if (count>0) {
        for (size_t i=0; i<count; i++){
            cout<<"file: "<<fn[i]<<endl;
        }
    }
    else
    {
        cout<<"No access to folder..."<<endl;
    }
}
void load_frames_to_vector(string folder,vector<Mat>& frames,vector<string> names){
    frames.clear();
    string frames_file = folder;
    // string video_file = PATH_DATA+"cam2/anillos.avi";
    
    //initial frame for get size of frames
    vector<cv::String> fn;
    glob(folder +"*.jpg", fn, false);
    
    vector<Mat> images;
    Mat img;
    size_t count = fn.size(); //total number  files in images folder
    
    size_t n = 30;
    string d_name;
    if (count>0) {
        for (size_t i=0; i<count; i++){
            d_name = folder+names[i]+".jpg";
            cout<<"file: "<<fn[i]<<"deserve: "<<d_name<<endl;
            if(fn[i] == d_name){
                img = cv::imread(fn[i]);
                frames.push_back(img);
            }
        }
    }
    else
    {
        cout<<"No access to folder..."<<endl;
    }
}

void export_mat_excel(Mat img, string name)
{
    ofstream myfile;
    myfile.open(name+".csv");
    for(int i=0; i<img.rows; i++)
    {
        for(int j=0; j<img.cols; j++)
        {
            myfile << ((Scalar)img.at<int>(i, j))[0];
            myfile << ",";
        }
        myfile << "\n";

    }
    myfile.close();
    //waitKey(5000);
}