#include <DisplayImage.h>
//   /home/whtet/temp/archive/faceDetect_sample/lena.jpg
//   /home/whtet/temp/video/PPV.mp4
// /home/whtet/temp/image/15_01_33---Tree-Black-and-White_web.jpg
static const int DISPLAY_PICTURE = 0;
static const int DISPLAY_MOVIE = 1;
static const int DISPLAY_MOVIE_BLURR = 2;
static const int DISPLAY_MOVIE_TRACKER = 3;
static const int DISPLAY_SIMPLE_TRANSFORM = 4;
static const int DISPLAY_NOT_SO_SIMPLE_TRANSFORM = 5;

int main(int argc, char** argv) {
	printf("Open %s \n", argv[1]);
	int methodId = DISPLAY_NOT_SO_SIMPLE_TRANSFORM;
	switch (methodId) {
	case DISPLAY_PICTURE:
		return displayPic(argc, argv);
	case DISPLAY_MOVIE:
		return displayMovie(argc, argv);
	case DISPLAY_MOVIE_BLURR:
		return displayMovieBlurr(argc, argv);
	case DISPLAY_MOVIE_TRACKER:
		return displayMovieTracker(argc, argv);
	case DISPLAY_SIMPLE_TRANSFORM:
		return displayTransform(argc, argv, true);
	case DISPLAY_NOT_SO_SIMPLE_TRANSFORM:
		return displayTransform(argc, argv, false);
	default:
		printf("No such method %d %s \n", methodId, argv[1]);
		break;
	}
}

int displayTransform(int argc, char** argv, bool simple) {
	//http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html#imread
	Mat image = imread(argv[1], 1);
	if (argc != 2 || !image.data) {
		printf("No image data \n");
		return -1;
	}
	if (simple) {
		simpleTransform(image);
	} else {
		notSoSimpleTransform(image);
	}
	return 0;
}

// Oreiley's Learning openCV example 2-5
void notSoSimpleTransform(Mat image) {
	Mat processed = doPyrDown(image, IPL_GAUSSIAN_5x5);
	processed = doPyrDown(processed, IPL_GAUSSIAN_5x5);
	processed = doCanny(processed, 10, 100, 3);
	namedWindow("IN_IMAGE");
	namedWindow("OUT_IMAGE");
	imshow("IN_IMAGE", image);
	imshow("OUT_IMAGE", processed);
	processed.release();
	image.release();
	waitKey(0);
	destroyWindow("OUT_IMAGE");
	destroyWindow("IN_IMAGE");
}

Mat doPyrDown(Mat image, int filter) {
	assert(image.size().width % 2 == 0 && image.size().height % 2 == 0);
	Mat out = Mat(image.size(), image.depth(), image.channels());
	pyrDown(image, out);
	return out;
}

Mat doCanny(Mat image, double lowTresh, double highTresh, double aperture) {
	if (image.channels() != 1) {
		printf("Image channel %d \n",image.channels());
		String error = "Canny only handles gray scale images";
		// throw error; it is working even if it is not grayscale
	}
	Mat out = Mat(image.size(), CV_8UC1);
	Canny(image, out, lowTresh, highTresh, aperture);
	return out;
}

// Oreiley's Learning openCV example 2-4
void simpleTransform(Mat image) {
	//http://docs.opencv.org/modules/highgui/doc/user_interface.html?highlight=cvcreatetrackbar#namedwindow
	namedWindow("IN_IMAGE");
	namedWindow("OUT_IMAGE");
	imshow("IN_IMAGE", image);
	//http://docs.opencv.org/modules/core/doc/basic_structures.html#mat-mat
	Mat out = cv::Mat(image.size(), CV_8UC1);
	//http://docs.opencv.org/modules/imgproc/doc/filtering.html?highlight=cvsmooth#gaussianblur
	GaussianBlur(image, out, Size(3, 3), 1.5, 1.5);
	imshow("OUT_IMAGE", out);
	out.release();
	//http://docs.opencv.org/modules/highgui/doc/user_interface.html?highlight=cvcreatetrackbar#waitkey
	waitKey(0);
	//http://docs.opencv.org/modules/highgui/doc/user_interface.html?highlight=cvcreatetrackbar#destroywindow
	destroyWindow("OUT_IMAGE");
	destroyWindow("IN_IMAGE");
}

int gSliderCurrPosition = 0;
VideoCapture *gVcapTracker;

void onTrackbarChanged(int pos, void* _void) {
	//printf("on_trackbar %d %d \n", pos, gSliderCurrPosition);
	if (gSliderCurrPosition != pos) {
		//http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html#videocapture-set
		gVcapTracker->set(CV_CAP_PROP_POS_FRAMES, pos);
		//bool flag = //printf("on_trackbar %d %s \n", pos, flag ? "true" : "false");
	}
}

// Oreiley's Learning openCV example 2-3
int displayMovieTracker(int argc, char** argv) {
	Mat image;
	int gSlider_position = 0;
	gVcapTracker = new VideoCapture(argv[1]);
	namedWindow("Display Image", CV_WINDOW_AUTOSIZE);

	int frames = (int) gVcapTracker->get(CV_CAP_PROP_FRAME_COUNT);

	if (frames != 0) {
		//http://docs.opencv.org/modules/highgui/doc/user_interface.html?highlight=cvcreatetrackbar#createtrackbar
		createTrackbar("Position", "Display Image", &gSlider_position, frames,
				onTrackbarChanged);
	}

	while (1) {
		gVcapTracker->read(image);
		//http://docs.opencv.org/modules/highgui/doc/user_interface.html?highlight=cvcreatetrackbar#settrackbarpos
		setTrackbarPos("Position", "Display Image", gSlider_position);
		gSlider_position++;
		gSliderCurrPosition = gSlider_position;
		if (!image.data) {
			printf("Video data %s %d \n", argv[1], frames);
			break;
		}
		imshow("Display Image", image);
		if (waitKey(33) == 27)
			break;
	}
	gVcapTracker->release();
	destroyWindow("Display Image");
	return 0;
}

// Oreiley's Learning openCV example 2-2
int displayMovie(int argc, char** argv) {
	//http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html#videocapture-videocapture
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

//http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html#videocapture
int displayMovieBlurr(int argc, char** argv) {
	//VideoCapture *vcap = new VideoCapture(argv[1]);
	String path = "/home/whtet/temp/video/PPV.mp4";
	VideoCapture cap(path);
	Mat image;
	Mat edges;
	cap.open(path);
	if (!cap.isOpened()) {
		printf("Cannot open %s \n", path.c_str());
		return -1;
	}
	namedWindow("Display Image", CV_WINDOW_AUTOSIZE);
	int count = 0;

	while (1) {
		//vcap->grab();
		//vcap->read(image);
		Mat frame;
		cap >> frame;
		if (!frame.data) {
			printf("No video data %s %d \n", path.c_str(), count);
			break;
		}
		cvtColor(frame, edges, CV_BGR2GRAY);
		GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
		Canny(edges, edges, 0, 30, 3);
		imshow("Display Image", edges);
		if (waitKey(33) == 27)
			break;
		count++;
	}
	//vcap->release();
	cap.release();
	destroyWindow("Display Image");
	return 0;
}

// Oreiley's Learning openCV example 2-1
int displayPic(int argc, char** argv) {
	Mat image;
	image = imread(argv[1], 1);

	if (argc != 2 || !image.data) {
		printf("No image data \n");
		return -1;
	}

	namedWindow("Display Image", CV_WINDOW_AUTOSIZE);
	imshow("Display Image", image);

	waitKey(0);

	return 0;
}
