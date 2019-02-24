#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "angles_magnitude.h"
#include "opencv2/videoio.hpp"
#include "opencv2/video/tracking.hpp"
using namespace cv;
using namespace std;
#include <iostream>
#include "sample_cube.h"
#include "opticalFlowSet.h"
#include "utils.h"
#include "haralick.h"
#include <string>

// typedef std::pair<cv::Mat_<int>, cv::Mat_<int>> ParMat;
// void setOpticalFlowData(vector<Mat>& frames);
// void VecDesp2Mat(vector<Point2f> &vecPoints,vector<Point2f> &positions,ParMat & AMmat);

// vector<ParMat> data;
const string DATA_FOLDER = "data/";
const string FRAMES_FOLDER = "data/frames/";
const unsigned int  N_FRAMES_TEST = 10;

Mat co_ocurrence_magnitud(Mat matrix, int orientation);
void print_haralick_features(Mat haralick_features);
//void generate_cuboids(vector<Mat> &listCuboidAngles, vector<Mat> &listCuboidMagnitudes, vector<Angles_Magnitude> list_Angles_Magnitudes,int frame_size);
void generate_cuboids(vector<Mat> &listCuboidAngles, vector<Mat> &listCuboidMagnitudes, vector<Angles_Magnitude> frames_angles_magnitudes,int frame_size);
void generate_cuboids(vector<vector<Mat>> &listCuboidAngles, vector<vector<Mat>> &listCuboidMagnitudes, vector<Angles_Magnitude> frames_angles_magnitudes,int frame_size, int fr_start, int fr_end);
Mat normalize_matriz(Mat matriz);
int run_all_data(string, string, string);

struct Video
{
    string nameVideo;
    string exportName;
    string category;
};

int main(int argc, const char * argv[])
{
    vector<Video> listVideos;

    Video vid;
    
    //string action = "running"; int id = 0;
    //string action = "handclapping"; int id = 1;
    //string action = "handwaving"; int id = 2;
    //string action = "jogging"; int id = 3;
    //string action = "walking"; int id = 4;
    //string action = "boxing"; int id = 5;
    vector<string> list_actions;
    list_actions.push_back("running");
    list_actions.push_back("handclapping");
    list_actions.push_back("handwaving");
    list_actions.push_back("jogging");
    list_actions.push_back("walking");
    list_actions.push_back("boxing");

    for(int k=0; k<list_actions.size(); k++)
    {
        string action = list_actions[k];
        for(int i=1; i<=25; i++)
        {
            for(int j=1; j<=4; j++)
            {
                if(i > 9)
                {
                    vid.nameVideo = "data/"+action+"/person"+to_string(i)+"_"+action+"_d"+to_string(j)+"_uncomp.avi";
                    vid.exportName = action+"_person"+to_string(i)+"_s"+to_string(j);
                }
                else
                {
                    vid.nameVideo = "data/"+action+"/person0"+to_string(i)+"_"+action+"_d"+to_string(j)+"_uncomp.avi";
                    vid.exportName = action+"_person0"+to_string(i)+"_s"+to_string(j);
                }
                vid.category = action;
                listVideos.push_back(vid);
            }
        }
    }

    /*vid.nameVideo = "data/jogging/person02_jogging_d1_uncomp.avi";
    vid.exportName = "boxing_person02_s_prueba";
    listVideos.push_back(vid);*/

    /*vid.nameVideo = "data/person15_handclapping_d1_uncomp.avi";
    vid.exportName = "handclapping_person15_s1";
    vid.category = "handclapping";
    listVideos.push_back(vid);
    vid.nameVideo = "data/person15_handwaving_d1_uncomp.avi";
    vid.exportName = "handwaving_person15_s1";
    vid.category = "handwaving";
    listVideos.push_back(vid);
    vid.nameVideo = "data/person15_jogging_d1_uncomp.avi";
    vid.exportName = "jogging_person15_s1";
    vid.category = "jogging";
    listVideos.push_back(vid);
    vid.nameVideo = "data/person15_walking_d1_uncomp.avi";
    vid.exportName = "walking_person15_s1";
    vid.category = "walking";
    listVideos.push_back(vid);
    vid.nameVideo = "data/person15_running_d1_uncomp.avi";
    vid.exportName = "boxing_person15_s1";
    vid.category = "running";
    listVideos.push_back(vid);
    vid.nameVideo = "data/person15_boxing_d1_uncomp.avi";
    vid.exportName = "test_boxing_person15_s1";
    vid.category = "boxing";
    listVideos.push_back(vid);*/

    for(int i=0; i<listVideos.size(); i++)
        run_all_data(listVideos[i].nameVideo, listVideos[i].exportName, listVideos[i].category);



    return 0;
}

int run_all_data(string video_name, string activity, string id_activity) {
    // insert code here...
    int step = 5;
    //string video_name = "15_boxing_d4";
    //string activity = "running_23";
    //string activity = "boxing_15";
    //string video_name = "15_walking_d1";
    //int id_activity = 5;
    // string path = "/Users/davidchoqueluqueroman/Desktop/CURSOS-MASTER/IMAGENES/descriptor/data/person23.mp4";
    //string path = "data/person15_running_d1_uncomp.avi";
    //string path = "data/handclapping";
    //string path = "data/person15_running_d1_uncomp.avi";
    //string path = "data/person15_running_d1_uncomp.avi";


    //string path = "data/person18_boxing_d4_uncomp.avi";
     //string path = "data/boxing/person"+video_name+"_uncomp.avi";

    string path = video_name;

    VideoCapture capture;
    capture.open(path);
    if (!capture.isOpened())
        std::cerr << "Error processing file. Can't read video " << path;
    
    int videoLength = (capture.get(CV_CAP_PROP_FRAME_COUNT));
    int videoWidth = (capture.get(CV_CAP_PROP_FRAME_WIDTH));
    int videoHeight = (capture.get(CV_CAP_PROP_FRAME_HEIGHT));
    Mat image;
    vector<Mat> frames;
    for (int64 frameStep = 1; frameStep < videoLength; frameStep += step) {
        capture.set(CV_CAP_PROP_POS_FRAMES, frameStep);
        capture.read((image));
        if (image.empty())
            std::cerr << "Error processing file. Can't read frame " << frameStep
            << "from video %s" << path;
        
        /*if(frames.size()<N_FRAMES_TEST){
            Mat img = image.clone();
            Mat blur;
            GaussianBlur(img, blur, Size(5,5), 0, 0 );
            frames.push_back(blur);
            save_frame("data/frames/",to_string(frameStep),img);
        }
        else
        {
            break;
        }*/

        Mat img = image.clone();
        Mat blur;
        GaussianBlur(img, blur, Size(5,5), 0, 0 );
        frames.push_back(blur);
        save_frame("data/frames/",to_string(frameStep),img);

        imshow("imagen", image);
        waitKey(1);
    }


    
    /*int w = 48;
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
    }*/
    
    /*cout<<"frames Size: "<<frames.size()<<endl;
    cout<<"video Height: "<<videoHeight<<endl;
    cout<<"video Width: "<<videoWidth<<endl;*/
    //cout<<"N cubos: "<<cubos.size()<<endl;
    
    vector<Point2f> points[2];

    OpticalFlowSet osf(frames.size());
    osf.calculateOpticalFlow(frames,15);
    // osf.plot_angles_magnitudes();

    cout<<"total magnitudes pair: "<<osf.angles_magnitudes.size()<<endl;
    cout << "time per cuboides: " << osf.angles_magnitudes.size()/5 << endl;
    int longitud_cuboid = osf.angles_magnitudes.size()/5-1;
    int t_per_cuboid = 10;
    int t_overlapping = 5;

    //export_mat_excel(osf.angles_magnitudes[1].angles, "angles");
    //export_mat_excel(osf.angles_magnitudes[1].magnitudes, "magnitudes");

    vector<Mat> list_haralick_angles;
    vector<Mat> list_haralick_magnitudes;
//Seccion of Haralick
    Haralick har;
    int size_frame = 20;

    vector<Mat> listCuboidAngles;
    vector<Mat> listCuboidMagnitudes;
    
    vector<vector<Mat>> listAllCuboidAngles;
    vector<vector<Mat>> listAllCuboidMagnitudes;

    Mat zeros = Mat::zeros(size_frame, size_frame, CV_32FC1);
    Mat sampling_angles, sampling_magnitudes;

    //for(int i=0; i<osf.angles_magnitudes.size(); i++)
    //generate_cuboids(listCuboidAngles, listCuboidMagnitudes, osf.angles_magnitudes[i].angles, osf.angles_magnitudes[i].magnitudes,size_frame);
//    generate_cuboids(listCuboidAngles, listCuboidMagnitudes, osf.angles_magnitudes,size_frame);

    for(int i=0; i<longitud_cuboid; i++)
    {
        generate_cuboids(listAllCuboidAngles, listAllCuboidMagnitudes, osf.angles_magnitudes,size_frame, i*t_overlapping, i*t_overlapping+t_per_cuboid);
    }    
        

   // export_mat_excel(osf.angles_magnitudes[3].angles, "cuboid_angle");

    //cout << listCuboidAngles.size() << endl;
    //cout << listCuboidMagnitudes.size() << endl;

    /*for(int i=0; i<osf.angles_magnitudes.size(); i++)
    {
        export_mat_excel_int(osf.angles_magnitudes[i].angles, "frame_angles_"+to_string(i));
        export_mat_excel_int(osf.angles_magnitudes[i].magnitudes, "frame_magnitude_"+to_string(i));
    }*/

    //cout << "scangels: "<< listAllCuboidAngles.size() << endl;
    //cout << "scmagnitudes: "<< listAllCuboidMagnitudes.size() << endl;

    for(int i=0; i<listAllCuboidAngles.size(); i++)
    {
        for(int j=0; j<listAllCuboidAngles[i].size(); j++)
        {
            for(int orientation = 0; orientation<4; orientation++)
            {
                Mat co_ocurrence_angle = co_ocurrence_magnitud(listAllCuboidAngles[i][j], orientation);
                Mat co_ocurrence_magnitude = co_ocurrence_magnitud(listAllCuboidMagnitudes[i][j], orientation);

    //Normalize matrix to input to haralick
                Mat co_ocurrence_angle_n = normalize_matriz(co_ocurrence_angle);
                Mat co_ocurrence_magnitude_n = normalize_matriz(co_ocurrence_magnitude);
                
    //Step 4 Extract haralick features
                Mat haralick_angles = har.calculate(co_ocurrence_angle_n);
                //export_mat_excel(haralick_angles,"haralick_n");
                Mat haralick_magnitudes = har.calculate(co_ocurrence_magnitude_n);

                list_haralick_angles.push_back(haralick_angles);
                list_haralick_magnitudes.push_back(haralick_magnitudes);   
            }
        }
    }
    
//Step 3 Calculate de co_ocurrence matrix of angles and magnitudes
    /*for(int i=0; i<listCuboidAngles.size(); i++)
    {
        //export_mat_excel_int(listCuboidAngles[i], "cuboid_angle_"+to_string(i));
        //export_mat_excel_int(listCuboidMagnitudes[i], "cuboid_magnitude_"+to_string(i));
        //Create orientation
        for(int orientation = 0; orientation<4; orientation++)
        {
        //cout << "co_ocurrence" << endl;
            Mat co_ocurrence_angle = co_ocurrence_magnitud(listCuboidAngles[i], orientation);
            
            Mat co_ocurrence_magnitude = co_ocurrence_magnitud(listCuboidMagnitudes[i], orientation);

//Normalize matrix to input to haralick
            Mat co_ocurrence_angle_n = normalize_matriz(co_ocurrence_angle);
            Mat co_ocurrence_magnitude_n = normalize_matriz(co_ocurrence_magnitude);

            //export_mat_excel(listCuboidAngles[0], "cuboid_angle");
            
//Step 4 Extract haralick features
            Mat haralick_angles = har.calculate(co_ocurrence_angle_n);
            //export_mat_excel(haralick_angles,"haralick_n");
            Mat haralick_magnitudes = har.calculate(co_ocurrence_magnitude_n);

            list_haralick_angles.push_back(haralick_angles);
            list_haralick_magnitudes.push_back(haralick_magnitudes);   
        }
    }*/

    //print_haralick_features(haralick_angles);
    //print_haralick_features(haralick_magnitudes);

    cout << "list: " << list_haralick_angles.size() << endl;
    export_listmat_excel(list_haralick_angles, list_haralick_magnitudes,"list_"+activity, id_activity);
    //export_listmat_excel(list_haralick_magnitudes, "list_"+activity+"_magnitudes");
    
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
            if(!((matrix.at<int>(i,k) == -1) || (matrix.at<int>(i+i_orient,k+j_orient) == -1)))
            //if(((matrix.at<int>(i,k) != -1) && (matrix.at<int>(i+i_orient,k+j_orient) != -1)))
                //co_magnitud.at<float>(matrix.at<int>(i,k), matrix.at<int>(i+i_orient,k+j_orient))++;
                co_magnitud.at<float>(matrix.at<int>(i,k), matrix.at<int>(i+i_orient,k+j_orient)) += 1;
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
    //cout << "f13: "<< haralick_features.at<float>(0,12) << endl;
    //cout << "f14: "<< haralick_features.at<float>(0,13) << endl;
}

void generate_cuboids(vector<Mat> &listCuboidAngles, vector<Mat> &listCuboidMagnitudes, vector<Angles_Magnitude> frames_angles_magnitudes,int frame_size)
{   
    for(int h=0; h<(frames_angles_magnitudes[0].angles.rows/frame_size); h++)
    {       
        for(int k=0; k<(frames_angles_magnitudes[0].angles.cols/frame_size); k++)
        {   
            for(int fr=0; fr<frames_angles_magnitudes.size(); fr++)
            {
                Mat angles_cuboid = Mat::zeros(frame_size, frame_size, CV_32FC1);
                Mat magnitude_cuboid = Mat::zeros(frame_size, frame_size, CV_32FC1);
                for(int i=h*frame_size; i<(h+1)*frame_size; i++)
                {
                    for(int j=k*frame_size; j<(k+1)*frame_size; j++)
                    {
                        angles_cuboid.at<float>(i-h*frame_size,j-k*frame_size) = frames_angles_magnitudes[fr].angles.at<float>(i,j);
                        magnitude_cuboid.at<float>(i-h*frame_size,j-k*frame_size) = frames_angles_magnitudes[fr].magnitudes.at<float>(i,j);
                    }
                }
                listCuboidAngles.push_back(angles_cuboid);
                listCuboidMagnitudes.push_back(magnitude_cuboid);
            }
        }
    }
}

void generate_cuboids(vector<vector<Mat>> &listAllCuboidAngles, vector<vector<Mat>> &listAllCuboidMagnitudes, vector<Angles_Magnitude> frames_angles_magnitudes,int frame_size, int fr_start, int fr_end)
{
    //cout << "-> " << fr_start << " - " << fr_end << endl;
    vector<Mat> listCuboidAngles;
    vector<Mat> listCuboidMagnitudes;

    for(int h=0; h<(frames_angles_magnitudes[0].angles.rows/frame_size); h++)
    {       
        for(int k=0; k<(frames_angles_magnitudes[0].angles.cols/frame_size); k++)
        {   
            //for(int fr=0; fr<frames_angles_magnitudes.size(); fr++)
            for(int fr=fr_start; fr<fr_end; fr++)
            {
                Mat angles_cuboid = Mat::zeros(frame_size, frame_size, CV_32FC1);
                Mat magnitude_cuboid = Mat::zeros(frame_size, frame_size, CV_32FC1);
                for(int i=h*frame_size; i<(h+1)*frame_size; i++)
                {
                    for(int j=k*frame_size; j<(k+1)*frame_size; j++)
                    {
                        angles_cuboid.at<float>(i-h*frame_size,j-k*frame_size) = frames_angles_magnitudes[fr].angles.at<float>(i,j);
                        magnitude_cuboid.at<float>(i-h*frame_size,j-k*frame_size) = frames_angles_magnitudes[fr].magnitudes.at<float>(i,j);
                    }
                }
                listCuboidAngles.push_back(angles_cuboid);
                listCuboidMagnitudes.push_back(magnitude_cuboid);
            }
        }
    }

    listAllCuboidAngles.push_back(listCuboidAngles);
    listAllCuboidMagnitudes.push_back(listCuboidMagnitudes);

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

    if(sum != 0)
        return matriz/sum;
    else
        return matriz;
}

 /*if(i < 9)
                export_mat_excel_float(co_ocurrence_angle,"boxing_co_ocurrence_angle_orientation_"+to_string(orientation)+"_cuboid_00"+to_string(i));
            else
                if(i < 100)
                    export_mat_excel_float(co_ocurrence_angle,"boxing_co_ocurrence_angle_orientation_"+to_string(orientation)+"_cuboid_0"+to_string(i));
                else
                    export_mat_excel_float(co_ocurrence_angle,"boxing_co_ocurrence_angle_orientation_"+to_string(orientation)+"_cuboid_"+to_string(i));*/
