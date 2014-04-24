/*
 * PracticalOpenCV.hpp
 *
 *  Created on: Apr 23, 2014
 *      Author: whtet
 */

#ifndef PRACTICALOPENCV_HPP_
#define PRACTICALOPENCV_HPP_
#include <LocalEnv.h>
#include <DisplayImage.h>
using namespace cv;

class PracticalOpenCV {
public:
	static int entry(int argc, char** argv);
};
int regionOfInterest(Mat image);
#endif /* PRACTICALOPENCV_HPP_ */
