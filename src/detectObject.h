/*****************************************************************************
*   Face Recognition using Eigenfaces or Fisherfaces
******************************************************************************
*   by Shervin Emami, 5th Dec 2012
*   http://www.shervinemami.info/openCV.html
******************************************************************************
*   Ch8 of the book "Mastering OpenCV with Practical Computer Vision Projects"
*   Copyright Packt Publishing 2012.
*   http://www.packtpub.com/cool-projects-with-opencv/book
*****************************************************************************/

//////////////////////////////////////////////////////////////////////////////////////
// detectObject.cpp, by Shervin Emami (www.shervinemami.info) on 30th May 2012.
// Easily detect objects such as faces or eyes (using LBP or Haar Cascades).
//////////////////////////////////////////////////////////////////////////////////////

#pragma once


#include <stdio.h>
#include <iostream>
#include <vector>

// Include OpenCV's C++ Interface
#include "opencv2/opencv.hpp"


using namespace cv;
using namespace std;



// Search for just a single object in the image, such as the largest face, storing the result into 'largestObject'.
// Can use Haar cascades or LBP cascades for Face Detection, or even eye, mouth, or car detection.
// Input is temporarily shrunk to 'scaledWidth' for much faster detection, since 240 is enough to find faces.
// Note: detectLargestObject() should be faster than detectManyObjects().
void detectLargestObject(const Mat &img, CascadeClassifier &cascade, Rect &largestObject, int scaledWidth = 320);


// Search for many objects in the image, such as all the faces, storing the results into 'objects'.
// Can use Haar cascades or LBP cascades for Face Detection, or even eye, mouth, or car detection.
// Input is temporarily shrunk to 'scaledWidth' for much faster detection, since 240 is enough to find faces.
// Note: detectLargestObject() should be faster than detectManyObjects().
void detectManyObjects(const Mat &img, CascadeClassifier &cascade, vector<Rect> &objects, int scaledWidth = 320);

void initDetectors(CascadeClassifier &faceCascade, CascadeClassifier &eyeCascade1, CascadeClassifier &eyeCascade2);
// C++ conversion functions between integers (or floats) to std::string.
template <typename T> string toString(T t)
{
	ostringstream out;
	out << t;
	return out.str();
}

template <typename T> T fromString(string t)
{
	T out;
	istringstream in(t);
	in >> out;
	return out;
}

// Load the face and 1 or 2 eye detection XML classifiers.
void initDetectors(CascadeClassifier &faceCascade, CascadeClassifier &eyeCascade1, CascadeClassifier &eyeCascade2);

// Get access to the webcam.
void initWebcam(VideoCapture &videoCapture, int cameraNumber);

Rect returnFacePosition(VideoCapture &videoCapture, CascadeClassifier &faceCascade, CascadeClassifier &eyeCascade1, CascadeClassifier &eyeCascade2);