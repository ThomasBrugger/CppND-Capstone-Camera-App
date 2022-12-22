#include"Imageprocessor.h"   

#include<string>
#include<vector>
#include<iostream>
#include<thread> 

std::vector<FacePosition> Imageprocessor::detectFace(std::string fileName, bool showWnd){
   cv::Mat image_with_humanface;
   image_with_humanface = cv::imread(fileName);
   
   //Pre Trained by OpenCV
   std::string trained_classifier_location = "../files/haarcascade_frontalface_alt.xml";
   cv::CascadeClassifier faceDetector;
   faceDetector.load(trained_classifier_location);

   std::vector<cv::Rect>faces;
   std::vector<cv::Rect>boundary;
   std::vector<FacePosition> FacePosContainer;

   faceDetector.detectMultiScale(image_with_humanface, faces, 1.1, 4, cv::CASCADE_SCALE_IMAGE, cv::Size(20, 20));

   for (size_t i = 0; i < faces.size(); i++){ 
      FacePosition facePos;
      cv::Mat faceROI = image_with_humanface(faces[i]);
      facePos.x = faces[i].x;
      facePos.y= faces[i].y;
      int point_w  = facePos.y + faces[i].height;
      int point_h = facePos.x + faces[i].width;
      facePos.height = faces[i].height;
      facePos.width = faces[i].width; 
      FacePosContainer.push_back(facePos);
      cv::rectangle(image_with_humanface, cv::Point(facePos.x, facePos.y), cv::Point(point_h, point_w), cv::Scalar(255, 0, 255), 2, 8, 0);
   }
   //Function can be used with shown window or without.
   if (showWnd){
    cv::namedWindow("Face Detection");
    cv::imshow("Face Detection", image_with_humanface);
    cv::waitKey(0); //Window will only be closed by "x"
    cv::destroyWindow("Face Detection");
   }

   return FacePosContainer;
}

//Places a resized Image of a robot over an detected Face. (Facedetection does not always work!)
void Imageprocessor::makeRobotFace(std::string fileName, bool showWnd, bool saveImg){
    std::vector<FacePosition> FacePosContainer = detectFace(fileName,false);

    cv::Mat bigImage = cv::imread(fileName, -1);
    const cv::Mat smallImage = cv::imread("../files/Robot.jpg", -1);
    
    for(FacePosition facePos : FacePosContainer){
        cv::Mat resizedImg;
        cv::resize(smallImage, resizedImg,cv::Size(facePos.height, facePos.width));

        const int x = facePos.x;
        const int y = facePos.y;

        cv::Mat destRoi = bigImage(cv::Rect(x, y, resizedImg.cols, resizedImg.rows));
        resizedImg.copyTo(destRoi);
    }
    //Function can be used with shown window or without.
    if (showWnd){
        cv::namedWindow("Make Robot Face");
        cv::imshow("Make Robot Face", bigImage);
        cv::waitKey(0); //Window will only be closed by "x"
        cv::destroyWindow("Make Robot Face");
    }
    //Will be removed, when image date is passed by pointer
    if (saveImg){
        cv::imwrite("../files/image.jpg", bigImage);
    }
}
