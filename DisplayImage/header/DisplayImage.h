/*
 * DisplayImage.h
 *
 *  Created on: Apr 17, 2014
 *      Author: whtet
 */
//http://www.ozbotz.org/opencv-installation/
//http://docs.opencv.org/doc/tutorials/introduction/linux_eclipse/linux_eclipse.html
//http://www.newty.de/fpt/index.html
//http://pages.cs.wisc.edu/~hasti/cs368/CppTutorial/
//http://www.cprogramming.com/java/c-and-c++-for-java-programmers.html
#ifndef DISPLAYIMAGE_H_
#define DISPLAYIMAGE_H_
#include <LocalEnv.h>

using namespace cv;

int displayPic(int argc, char** argv);

int displayMovie(int argc, char** argv);
int displayMovieBlurr(int argc, char** argv);
int displayMovieTracker(int argc, char** argv);
int displayTransform(int argc, char** argv, bool simple);
void simpleTransform(Mat image);
void notSoSimpleTransform(Mat image);
Mat doPyrDown(Mat image, int filter = IPL_GAUSSIAN_5x5);
Mat doCanny(Mat image, double lowTresh, double highTresh, double aperture);

#endif /* DISPLAYIMAGE_H_ */
