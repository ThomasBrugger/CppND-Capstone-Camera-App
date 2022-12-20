#include "Camera.h"

#include <thread>
#include <chrono>
#include <mutex>

/*-------------------------------------------------------------------------------------------------------------------*/
/////////////////////////////////////Constructor///////////////////////////////////////////////////////////////////////
/*-------------------------------------------------------------------------------------------------------------------*/
Camera::Camera(MyFrame* myFrame) : m_pMyFrame(myFrame) {};

bool Camera::startCameraThread(){
	if(isCameraAvailable()){
		cameraThread = std::thread{&Camera::openCamera, this};
		return true;
	}
	return false;
}
/*-------------------------------------------------------------------------------------------------------------------*/
/////////////////////////////Camera executing Functions////////////////////////////////////////////////////////////////
/*-------------------------------------------------------------------------------------------------------------------*/
void Camera::openCamera(){
	cv::Mat image;
	m_bCameraClosed = false;

	m_sWindowName = "Image";
	cv::namedWindow(m_sWindowName);
	
	cv::VideoCapture cap(0);
	CameraOpeneEvent();

	while (!isCameraClosed()) {
		cap >> image;
		if (isSnapping()) {
			//In the future it should passed by pointer to the app, not saved as image
			cv::imwrite("../files/image.jpg", image);
		    ImageReadyEvent();
			snap(false);
		}
		else {
			imshow(m_sWindowName, image);
		}
		cv::waitKey(25);
	}
	cv::destroyWindow(m_sWindowName);
	CameraClosedEvent();
}

void Camera::closeCamera() {
	//std::unique_lock<std::mutex> lock(mtxClosed);
	m_bCameraClosed = true;
	//std::unique_lock<std::mutex> unlock(mtxClosed);
	cameraThread.join();
}

void Camera::snap(bool input) {
	std::scoped_lock<std::mutex> lock(mtxSnap);
	m_bSnap = input;
}
/*-------------------------------------------------------------------------------------------------------------------*/
/////////////////////////////Camera Status Functions///////////////////////////////////////////////////////////////////
/*-------------------------------------------------------------------------------------------------------------------*/
bool Camera::isCameraClosed(){
	std::scoped_lock<std::mutex> lock(mtxClosed);
	return m_bCameraClosed;
}

bool Camera::isSnapping(){
	std::scoped_lock<std::mutex> lock(mtxSnap);
	return m_bSnap;
}
/*-------------------------------------------------------------------------------------------------------------------*/
/////////////////////////////Video Display Functions///////////////////////////////////////////////////////////////////
/*-------------------------------------------------------------------------------------------------------------------*/
std::string Camera::getWindowName() {
	return m_sWindowName;
}

void Camera::setWindowName(std::string s) {
	m_sWindowName = s;
}
/*-------------------------------------------------------------------------------------------------------------------*/
/////////////////////////////Event to Gui Functions///////////////////////////////////////////////////////////////////
/*-------------------------------------------------------------------------------------------------------------------*/
void Camera::ImageReadyEvent(){
	wxCommandEvent event(IMAGEREADY); 
	wxPostEvent(m_pMyFrame, event);
}

void Camera::CameraClosedEvent(){
	wxCommandEvent event(CAMERACLOSED); 
	wxPostEvent(m_pMyFrame, event);
}

void Camera::CameraOpeneEvent(){
	wxCommandEvent event(CAMERAOPEN); 
	wxPostEvent(m_pMyFrame, event);
}
/*-------------------------------------------------------------------------------------------------------------------*/
////////////////////////Auxiliary Camera Functions/////////////////////////////////////////////////////////////////////
/*-------------------------------------------------------------------------------------------------------------------*/
bool Camera::isCameraAvailable(){
	if(isCameraClosed()){
		cv::VideoCapture cap(0);

		if (!cap.isOpened()) {
			return false;
		}
	}
	return true;
}