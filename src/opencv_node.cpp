#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv_operation.hpp"

Mat opencv_operation(Mat frame_raw);
void opencv_init(void);

vector<classifier_type> type = 
{
    // {"bowl", "/media/Data/Technology/Robotics/OpenCV/ELEC340/databases/samples/datasets/bowl/xml/classifier_2/cascade.xml", 8, 1, 0, GENERAL_MIN_SIZE, GENERAL_MAX_SIZE, BLUE},
    // {"bottle", "/media/Data/Technology/Robotics/OpenCV/ELEC340/databases/samples/datasets/bottle/xml/classifier_1/cascade.xml", 6, 2, 0, GENERAL_MIN_SIZE, GENERAL_MAX_SIZE, BLUE},
    {"cup", "/media/Data/Technology/Robotics/OpenCV/ELEC340/databases/samples/datasets/cup/xml/classifier_1/cascade.xml", 6, 3, 1, GENERAL_MIN_SIZE, GENERAL_MAX_SIZE, GREEN},
    // {"fork", "../databases/samples/datasets/fork/xml/cascade.xml", 8, 1, 0, GENERAL_MIN_SIZE, GENERAL_MAX_SIZE, BLUE},
    // {"knife", "../databases/samples/datasets/knife/xml/cascade.xml", 8, 50, 0, GENERAL_MIN_SIZE, GENERAL_MAX_SIZE, GREEN},
    {"oven", "/media/Data/Technology/Robotics/OpenCV/ELEC340/databases/samples/datasets/oven/xml/classifier_1/cascade.xml", 7, 30, 0, GENERAL_MIN_SIZE, GENERAL_MAX_SIZE, RED},
    // {"refrigerator", "/media/Data/Technology/Robotics/OpenCV/ELEC340/databases/samples/datasets/refrigerator/xml/classifier_1/cascade.xml", 7, 2, 0, GENERAL_MIN_SIZE, GENERAL_MAX_SIZE, BLUE},
    {"diningtable", "/media/Data/Technology/Robotics/OpenCV/ELEC340/databases/samples/datasets/diningtable/xml/classifier_1/cascade.xml", 7, 15, 0, GENERAL_MIN_SIZE, GENERAL_MAX_SIZE, BLUE},
    // {"couch", "/media/Data/Technology/Robotics/OpenCV/ELEC340/databases/samples/datasets/couch/xml/classifier_1/cascade.xml", 6, 30, 0, GENERAL_MIN_SIZE, GENERAL_MAX_SIZE, BLUE},
};

class ImageConverter
{
    ros::NodeHandle nh_;
    ros::Subscriber image_sub_;

    public:
        ImageConverter()
        {
            // Subscrive to input video feed and publish output video feed
            image_sub_ = nh_.subscribe("/camera/image_raw", 1, &ImageConverter::imageCb, this);
            
            // Subscribe this topic for PR2 robot
            //image_sub_ = nh_.subscribe("/narrow_stereo/right/image_color", 1, &ImageConverter::imageCb, this);

            namedWindow(GUI_WINDOW_NAME);
            moveWindow(GUI_WINDOW_NAME, 1400, 100);
        }

        ~ImageConverter()
        {
            destroyWindow(GUI_WINDOW_NAME);
        }

        void imageCb(const sensor_msgs::ImageConstPtr& msg)
        {
            cv_bridge::CvImageConstPtr cv_ptr;
            Mat proceed_image;

            try
            {
                cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
            }
            catch (cv_bridge::Exception& e)
            {
                ROS_ERROR("cv_bridge exception: %s", e.what());
                exit(1);
            }

            proceed_image = opencv_operation(cv_ptr->image);

            // Update GUI Window
            imshow(GUI_WINDOW_NAME, proceed_image);
            
            // Press ESC to exit
            if(waitKey(3) == 27)
                exit(0);
        }
};

Mat opencv_operation(Mat frame)
{
    Mat gray;

		// Pre-process the frame
		resize(frame, frame, RECOGNITION_FRAME_SIZE, 0, 0, INTER_AREA);
		cvtColor(frame, gray, CV_BGR2GRAY);
		equalizeHist(gray, gray);

		// Search and mark objects for each type
		for(int i = 0; i < type.size(); i++)
		{
        type[i].search(gray);
        type[i].mark_objects(frame);
		}

    return frame;
}

void opencv_init(void)
{
    if(type.size() < 1)
    {
        cerr << "--(ERROR)Please select at least one classifier!" << endl;
        exit(2);
    }
    else
    {
        // Load classifier of each type of objects
        for(int i = 0; i < type.size(); i++)
            type[i].load_classifier();
    }
}

int main(int argc, char** argv)
{
    opencv_init();
    ros::init(argc, argv, "opencv_node");
    ImageConverter ic;
    ros::spin();
    return 0;
}
