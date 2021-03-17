#include "opencv_operation.hpp"

classifier_type::classifier_type(String input_name, String input_path, double input_scale_factor, int input_min_neighbours, 
									int input_flags, Size input_min_size, Size input_max_size, Scalar input_display_color)
{
	name = input_name;
	classifier_path = input_path;
	scale_factor = input_scale_factor;
	min_neighbours = input_min_neighbours;
	flags = input_flags;
	min_size = input_min_size;
	max_size = input_max_size;
	display_color = input_display_color;
}

bool classifier_type::search(Mat input_frame)
{
	classifier.detectMultiScale(input_frame, areas, scale_factor, min_neighbours, flags, min_size, max_size);
}

bool classifier_type::load_classifier(void)
{
	if (classifier.load(classifier_path))							//Load cascade
	{ 
		cout << name << " classifier loaded successfully!" << endl;
	}
	else
	{
		cerr << "--(ERROR)" << name << " classifier loaded failed, exit..." << endl;
		exit(2);
	}
}

bool classifier_type::mark_objects(Mat input_frame)
{
	for (int i = 0; i < areas.size(); i++)
	{
		if (areas[i].height > 0 && areas[i].width > 0)
		{
			Point origin(areas[i].x, areas[i].y);
			Point center(areas[i].x + areas[i].width / 2, areas[i].y + areas[i].height / 2);
			
			circle(input_frame, center, 1, display_color);
			rectangle(input_frame, areas[i], display_color);
			putText(input_frame, name, origin, FONT_HERSHEY_COMPLEX, 1, display_color);

			cout << endl << "Object" << i + 1 << ":" << endl << endl;
			cout << "Origin of the object is: " << origin.x << ", " << origin.y << endl; 
			cout << "Center of the object is: " << center.x << ", " << center.y << endl; 
			cout << "Height and width: " << areas[i].height << "   " << areas[i].width << endl;
		}
	}
}

// int main()
// {
// 	vector<classifier_type> type = 
// 	{
// 		// {"bowl", "../databases/samples/datasets/bowl/xml/cascade.xml", 8, 3, 0, GENERAL_MIN_SIZE, GENERAL_MAX_SIZE, BLUE},
// 		{"bottle", "../databases/samples/datasets/bottle/xml/cascade.xml", 8, 10, 0, GENERAL_MIN_SIZE, GENERAL_MAX_SIZE, GREEN},
// 		// {"cup", "../databases/samples/datasets/cup/xml/cascade.xml", 8, 3, 0, GENERAL_MIN_SIZE, GENERAL_MAX_SIZE, RED},
// 		// {"fork", "../databases/samples/datasets/fork/xml/cascade.xml", 8, 1, 0, GENERAL_MIN_SIZE, GENERAL_MAX_SIZE, BLUE},
// 		// {"knife", "../databases/samples/datasets/knife/xml/cascade.xml", 8, 50, 0, GENERAL_MIN_SIZE, GENERAL_MAX_SIZE, GREEN},
// 	};

// 	// Open and check camera if in USE_CAMERA mode
// 	#ifdef USE_CAMERA
// 		VideoCapture capture(0);
// 		if (!capture.isOpened())
// 		{ 
// 			cerr << "--(ERROR)Error opening video capture!" << endl;
// 			exit(1);
// 		}
// 	#endif

// 	Mat frame, gray;
// 	bool stop = false;

// 	if(type.size() < 1)
// 	{
// 		cerr << "--(ERROR)Please select at least one classifier!" << endl;
// 		exit(2);
// 	}
// 	else
// 	{
// 		// Load classifier of each type of objects
// 		for(int i = 0; i < type.size(); i++)
// 			type[i].load_classifier();
// 	}
		
// 	// Recognition loop
// 	while (!stop)
// 	{
// 		// Load image to frame
// 		#ifdef USE_CAMERA
// 			capture >> frame;
// 		#else
// 			String filename = "../databases/samples/s1/pos/test_30.jpg";
// 			frame = imread(filename);
// 		#endif			
		
// 		// Pre-process the frame
// 		resize(frame, frame, RECOGNITION_FRAME_SIZE, 0, 0, INTER_AREA);
// 		cvtColor(frame, gray, CV_BGR2GRAY);
// 		equalizeHist(gray, gray);

// 		// Search and mark objects for each type
// 		for(int i = 0; i < type.size(); i++)
// 		{
// 			type[i].search(gray);
// 			type[i].mark_objects(frame);
// 		}

// 		// Display in window
// 		imshow(GUI_WINDOW_NAME, frame);

// 		// Press ESC to exit		
// 		if(waitKey(10) == 27)
// 			stop = true;
// 	}
 
// 	return 0;
// }