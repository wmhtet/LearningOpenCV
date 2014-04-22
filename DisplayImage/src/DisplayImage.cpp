#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <stdio.h>
#include <DisplayImage.h>

using namespace cv;

int main(int argc, char** argv) {
	bool movie = true;
	if (movie)
		displayMovie(argc, argv);
	else
		displayImage(argc, argv);
}

int displayImage(int argc, char** argv) {

	printf("Hello World \n");
	Mat image;
	printf("Hello World 1\n");
	image = imread(argv[1], 1);
	printf("Hello World 2 \n");

	if (argc != 2 || !image.data) {
		printf("No image data \n");
		return -1;
	}

	printf("Hello World 3\n");
	namedWindow("Display Image", CV_WINDOW_AUTOSIZE);
	printf("Hello World 4\n");
	imshow("Display Image", image);

	printf("Hello World 5\n");
	waitKey(0);

	return 0;
}

int displayMovie(int argc, char** argv) {
	VideoCapture *vcap = new VideoCapture(argv[1]);
	Mat image;

	namedWindow("Display Image", CV_WINDOW_AUTOSIZE);
	int count = 0;

	while (1) {
		//vcap->grab();
		vcap->read(image);
		if (!image.data) {
			printf("No video data %s %d \n", argv[1], count);
			break;
		}
		imshow("Display Image", image);
		if (waitKey(33) == 27)
			break;
		count++;
	}
	vcap->release();
	destroyWindow("Display Image");
	return 0;
}
