# CPPND: Capstone Hello World Repo

This is a starter repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213).

The Capstone Project gives you a chance to integrate what you've learned throughout this program. This project will become an important part of your portfolio to share with current and future colleagues and employers.

In this project, you can build your own C++ application starting with this repo, following the principles you have learned throughout this Nanodegree Program. This project will demonstrate that you can independently create applications using a wide range of C++ features.

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./CameraApp`.

## Rubric Points
## Loops, Functions, I/O
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
 
## Object Oriented Programming
* The project uses Object Oriented Programming techniques.
  * File Camera.h, App.h, Imageprocessor.h
* Classes use appropriate access specifiers for class members.
  * File Camera.h, App.h, Imageprocessor.h
* Classes encapsulate behavior.
  Getter: i.e. Camera.cpp Line 75-77 Camera::getWindowName
  Setter: i.e. Camera.cpp Line 79-81 Camera::setWindowName

## Memory Management
* The project makes use of references in function declarations.
  Examples: App.cpp Line 439-443 MyFrame::checkExistance and all Eventhandler Functions

## Concurrency
* The project uses multithreading.
  Example: Camera.cpp Line 12-17 Camera::startCameraThread
* A mutex or lock is used in the project.
  Example: Camera.cpp Camera::isSnapping
