/*
 * PracticalOpenCV.cpp
 *
 *  Created on: Apr 23, 2014
 *      Author: whtet
 */
#include <PracticalOpenCV.hpp>
using namespace std;
using namespace cv;

static const int REGION_OF_INTEREST = 0;
static const String WINDOW_NAME = "PracticalOpenCV Window";

int PracticalOpenCV::entry(int argc, char** argv) {
	Mat image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	if (!image.data) {
		printf("No image data");
		return -1;
	}
	int methodId = REGION_OF_INTEREST;
	switch (methodId) {
	case REGION_OF_INTEREST:
		return regionOfInterest(image);
	default:
		printf("No such method %d %s \n", methodId, argv[1]);
		break;
	}
	return 0;
}

//Global variables
//Flags updated according to left mouse button activity
bool ldown = false, lup = false;

//Original image
Mat origImage;
//Starting and ending points of the user's selection
Point corner1, corner2;

//ROI
Rect box;
static void onMouse(int event, int x, int y, int, void *) {
	// When the left mouse button is pressed, record its position and save it in corner1
	if (event == EVENT_LBUTTONDOWN) {
		ldown = true;
		corner1.x = x;
		corner1.y = y;
		cout << "Corner 1 recorded at " << corner1 << endl;
	}

	if (event == EVENT_LBUTTONUP) {
		if (abs(x - corner1.x) > 20 && abs(y - corner1.y) > 20) {
			lup = true;
			corner2.x = x;
			corner2.y = y;
			cout << "Corner 2 recorded at " << corner2 << endl;
		} else {
			cout << "Please select a bigger region." << endl;
			ldown = false;
		}
	}

	if (ldown && !lup) {
		Point pt;
		pt.x = x;
		pt.y = y;
		Mat local_img = origImage.clone();
		rectangle(local_img, corner1, pt, Scalar(0, 0, 255));
		imshow(WINDOW_NAME, local_img);
	}

	// Define ROI and crop it out when both corners have been selected
	if (ldown && lup) {
		box.width = abs(corner1.x - corner2.x);
		box.height = abs(corner1.y - corner2.y);
		box.x = min(corner1.x, corner2.x);
		box.y = min(corner1.y, corner2.y);

		//Make an image out of just the selected ROI and display it in the a new window
		Mat crop(origImage, box);
		//namedWindow("Crop");
		//imshow("Crop", crop);
		DisplayImage::notSoSimpleTransform(crop);
		ldown = false;
		lup = false;
	}
}

int regionOfInterest(Mat image) {
	namedWindow(WINDOW_NAME, CV_WINDOW_AUTOSIZE);
	origImage = image;
	imshow(WINDOW_NAME, origImage);
	setMouseCallback(WINDOW_NAME, onMouse);
	waitKey(0);
	return 0;
}
