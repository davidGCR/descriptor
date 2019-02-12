int ** allocateMapToOpticalFlowsMatrix(int numImgs) {
    int ** mapToOpticalFlows = new int*[numImgs];
    for (int i = 0; i < numImgs; i++) {
        mapToOpticalFlows[i] = new int[numImgs];
        memset(mapToOpticalFlows[i], 0, sizeof(int) * numImgs);
    }
    return mapToOpticalFlows;
}

void setOpticalFlowData(vector<Mat>& frames, vector<ParMat>& data) {
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
                FillPoints(points[0], frames[j], frames[i],-1);
                
                ParMat angles_magni;
                angles_magni.first = Mat(rows, cols, CV_16SC1, -1);  // angles
                angles_magni.second = Mat(rows, cols, CV_16SC1, -1);  // magnitude
                
                if (points[0].size() > 0) {
                    calcOpticalFlowPyrLK(frames[i], frames[j], points[0], points[1],status, err, winSize, 3, termcrit, 0, 0.001);

                    VecDesp2Mat(points[1], points[0], angles_magni);
                }
                
                data.push_back(angles_magni);
                mapToOpticalFlows[i][j] = data.size() - 1;
                mapToOpticalFlows[j][i] = data.size() - 1;
            } else {
                break;
            }
        }
    }
}

void select_important_pixels(vector<Point2f> &vecPoints, Mat frameB, Mat frameA, int thr) {
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