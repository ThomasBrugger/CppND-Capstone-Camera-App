# CppND: Capstone Hello World Repo

This is my Repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213).

For this project I made my first steps in gui programming with wxWidgets and image captruing/porcessing with openCV.

You can take pictures with your webcam and change your face to a "a.w.e.s.o.m-o" RobotFace or test face recognition and RobotFace on other pictures.

![RobotFace](https://user-images.githubusercontent.com/42523312/209194287-9b032bb5-e7a6-4d6e-a8dc-155821bf5121.PNG)

## Dependencies for Running Locally
* cmake >= 3.25.0
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* wxWidgets >= 3.0
  * Linux: sudo apt-get install libwxgtk3.0-gtk3-dev libwxgtk3.0-gtk3-0v5. If you are facing unmet dependency issues, refer to the [official page](https://www.wxwidgets.org/) for installing the unmet dependencies.
  * Installation instructions can be found [here](https://wiki.wxwidgets.org/Install). Some version numbers may need to be changed in instructions to install v3.0 or greater.
* OpenCV >= 4.1 The OpenCV 4.1.0 source code can be found [here](https://github.com/opencv/opencv/tree/4.1.0)


## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./CameraApp`.

## Howto

* with webcam:
  1. press "Start Camera"
  2. * press "Snap" to take a picture without imageprocessing
     * press "RobotFace" to take a picture with imageprocessing
  3. press "Close Camera"
  4. choose "Save" in menue "File" to save the image in the "file"-directory

* without webcam:
  1. choose "Detect Face" or "Make Robot Face"-Mode in menu "OpenCv"
  2. Load a file that is located in the "file"-directory of this programm by typing its name without file extension.
  3. See the result and press any key if you want to close the window
     Note: By now only .jpg can be used in this programm! Also processed images can't be saved

## Rubric Points
### Loops, Functions, I/O
* The project demonstrates an understanding of C++ functions and control structures.
  * While-Loop: i.e. Camera.cpp Line 32-44
  * For-Loop: i.e. Imageprocessor.cpp Line Line 23-34
  * Function: Project is full of examples
* The project reads data from a file and process the data, or the program writes data to a file.
  * File Reading: i.e. App.cpp Line 257-276 MyFrame::OnLoad
  * File Processing: i.e. Imageprocessor.cpp Line 46-72 functio Imageprocessor::makeRobotFace
  * File Writing:  i.e. App.cpp Line 235-255 MyFrame::OnSave
* The project accepts user input and processes the input.
 * File Reading: i.e. App.cpp Line 346-366 MyFrame::OnMakeRobotFace
 
### Object Oriented Programming
* The project uses Object Oriented Programming techniques.
  * File Camera.h, App.h, Imageprocessor.h
* Classes use appropriate access specifiers for class members.
  * File Camera.h, App.h, Imageprocessor.h
* Classes encapsulate behavior.
  * Getter: i.e. Camera.cpp Line 75-77 Camera::getWindowName
  * Setter: i.e. Camera.cpp Line 79-81 Camera::setWindowName

### Memory Management
* The project makes use of references in function declarations.
  * Examples: App.cpp Line 439-443 MyFrame::checkExistance and all Eventhandler Functions

### Concurrency
* The project uses multithreading.
  * Example: Camera.cpp Line 12-17 Camera::startCameraThread
* A mutex or lock is used in the project.
  * Example: Camera.cpp Camera::isSnapping


## Improvements
* Finde a way to embed the openCV video window to the gui window
* passing image around without saving and loading it in other parts of the code
* Make more image types usable
* use a file handling provided by wxWidgets for improved loading and saving
