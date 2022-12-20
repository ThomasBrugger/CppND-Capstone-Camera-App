#ifndef CAMERA_H_
#define CAMERA_H_

#include "App.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui_c.h"

#include <string>
#include <thread>
#include <mutex>
#include <atomic>

//Forward Declaration
class MyFrame;

	class Camera {

	public:/*Constructor*/
		Camera(MyFrame* myFrame);
	
	public:/*Camera Status Functions*/
		bool isCameraOn();
		bool isCameraClosed();
		bool isSnapping();

	public:/*Camera execution functions*/
		bool startCameraThread();
		void openCamera();
		void closeCamera();
		void snap(bool input);

	public:/*Video Capture Gui Functions*/
		std::string getWindowName();
		void setWindowName(std::string s);

	private:/*general members*/
		std::string m_sWindowName;
		std::thread cameraThread;
		MyFrame* m_pMyFrame;
	
	private:/*Thread safety members*/
		std::atomic<bool> m_bSnap{ false };
		std::atomic<bool> m_bCameraClosed{ true };
		std::atomic<bool> m_bIsCameraOn{ false };
		std::mutex mtxSnap;
		std::mutex mtxClosed;

	private:/*Eventhandling Functions to Gui*/
		void ImageReadyEvent();
		void CameraClosedEvent();
		void CameraOpeneEvent();
	
	private:/*Auxilliary Memberfunction*/
		bool isCameraAvailable();
	};

#endif /* CAMERA_H_ */