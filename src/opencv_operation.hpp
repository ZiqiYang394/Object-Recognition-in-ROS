#ifndef _OPENCV_OPERATION_HPP_
#define _OPENCV_OPERATION_HPP_

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

using namespace std;
using namespace cv;

#define RECOGNITION_FRAME_SIZE 				Size(400, 300)
#define GUI_WINDOW_NAME 					"Object Recognition in ROS"

#define GENERAL_MIN_SIZE					Size(30, 30)
#define GENERAL_MAX_SIZE					Size(600, 600)

#define BLUE								Scalar(255, 0, 0)
#define GREEN								Scalar(0, 255, 0)
#define RED									Scalar(0, 0, 255)

class classifier_type
{
	public:
		classifier_type(String input_name, String input_path, double input_scale_factor, int input_min_neighbours, int input_flags,	Size input_min_size, Size input_max_size, Scalar input_display_color);

		bool load_classifier(void);
		bool search(Mat input_frame);
		bool mark_objects(Mat input_frame);

	private:
		String name;
		String classifier_path;
		CascadeClassifier classifier;
		
		vector<Rect> areas;
		double scale_factor;
		int min_neighbours;
		int flags;
		Size min_size;
		Size max_size;

		Scalar display_color;
};

#endif