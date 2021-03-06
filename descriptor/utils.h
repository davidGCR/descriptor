#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "fstream"
#include <iostream>
#include <limits.h>
using namespace cv;
using namespace std;

float max_float = SHRT_MAX;
float min_float = SHRT_MIN;

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

void export_mat_excel_float(Mat img, string name)
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

void export_mat_excel_int(Mat img, string name)
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

//480 features
void export_listmat_excel(vector<Mat> list_angles,vector<Mat> list_magnitudes, string name, string activity)
{
    cout << "max: " << max_float << endl;
    ofstream myfile;
    myfile.open("data_generated/"+name+".csv");
    //myfile << "angles" << "," << "magnitudes" << "," << "activity" << "\n";
    //cout << "-->" << list_angles.size() << endl;
    //cout << "--->" << list_angles[0].cols << endl; 

    for(int i=0; i<961; i++)
    {
        if(i < 960)
            myfile << "value" << ",";
        else
            myfile << "activity" << "\n";
    }

    double suma =0;
    vector<float> angles;
    vector<float> magnitudes;
    for(int i=0; i<list_angles.size(); i++)
    {
        for(int j=0; j<list_angles[i].cols; j++)
        {
            suma += ((Scalar)list_angles[i].at<float>(0, j))[0]+((Scalar)list_magnitudes[i].at<float>(0, j))[0];

            angles.push_back(((Scalar)list_angles[i].at<float>(0, j))[0]);
            magnitudes.push_back(((Scalar)list_magnitudes[i].at<float>(0, j))[0]);

            
            //-4.34882e-06
           // if(list_angles[i].at<float>(0, j) != 0 && list_angles[i].at<float>(0, j) > -0.000004)
            //{
                //cout << ((Scalar)list_angles[i].at<float>(0, j))[0]*1000 << endl;

                //myfile << ((Scalar)list_angles[i].at<float>(0, j))[0] << ",";
                //myfile << ((Scalar)list_magnitudes[i].at<float>(0, j))[0] << ",";
            //}
        }

        if((i+1)%40 == 0)
        {   
            //cout << "-->" << angles.size()<< endl;

            if(suma != 0)
            {
                for(int k=0; k<angles.size(); k++)
                {
                    myfile << angles[k] << ",";
                    myfile << magnitudes[k] << ",";
                //myfile << ((Scalar)list_angles[i].at<float>(0, k))[0] << ",";
                //myfile << ((Scalar)list_magnitudes[i].at<float>(0, k))[0] << ",";
             
                }
                myfile << activity;
                myfile << "\n";
            }

            suma = 0;
            angles.clear();
            magnitudes.clear();

        }
    }
    myfile.close();
   
}