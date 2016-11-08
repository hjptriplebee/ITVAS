
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstring>

using namespace std;
using namespace cv;

#ifdef _DEBUG
	#pragma comment(lib,"..\\x64\\Debug\\reid_tracking.lib")
#else
	#pragma comment(lib,"..\\x64\\Release\\reid_tracking.lib")
#endif

static Mat image;
static Rect2d boundingBox;
static bool paused;
static bool selectObject = false;
static bool startSelection = false;

static const char* keys =
{ "{@tracker_algorithm | | Tracker algorithm }"
"{@video_name      | | video name        }"
"{@start_frame     |0| Start frame       }"
"{@bounding_frame  |0,0,0,0| Initial bounding frame}" };

static void onMouse(int event, int x, int y, int, void*)
{
	if (!selectObject)
	{
		switch (event)
		{
		case EVENT_LBUTTONDOWN://鼠标按下
			//set origin of the bounding box
			startSelection = true;
			boundingBox.x = x;
			boundingBox.y = y;
			break;
		case EVENT_LBUTTONUP://鼠标松开
			//sei with and height of the bounding box
			boundingBox.width = std::abs(x - boundingBox.x);
			boundingBox.height = std::abs(y - boundingBox.y);
			paused = false;
			selectObject = true;
			break;
		case EVENT_MOUSEMOVE://鼠标移动

			if (startSelection && !selectObject)
			{
				//draw the bounding box
				Mat currentFrame;
				image.copyTo(currentFrame);
				rectangle(currentFrame, Point((int)boundingBox.x, (int)boundingBox.y), Point(x, y), Scalar(255, 0, 0), 2, 1);
				imshow("Tracking API", currentFrame);
			}
			break;
		}
	}
}

static void help()
{
	cout << "\nThis example shows the functionality of \"Long-term optical tracking API\""
		"-- pause video [p] and draw a bounding box around the target to start the tracker\n"
		"Example of <video_name> is in opencv_extra/testdata/cv/tracking/\n"
		"Call:\n"
		"./tracker <tracker_algorithm> <video_name> <start_frame> [<bounding_frame>]\n"
		"tracker_algorithm can be: MIL, BOOSTING, MEDIANFLOW, TLD\n"
		<< endl;

	cout << "\n\nHot keys: \n"
		"\tq - quit the program\n"
		"\tp - pause video\n";
}

int main(int argc, char** argv){
	CommandLineParser parser(argc, argv, keys);

	String tracker_algorithm = parser.get<String>(0);
	String video_name = parser.get<String>(1);
	int start_frame = parser.get<int>(2);
	tracker_algorithm = "TLD";//  MIL,BOOSTING,MEDIANFLOW,TLD,KCF
	video_name = "E:\\opencv3.0\\opencv\\sources\\samples\\data\\768x576.avi";
	start_frame = 1;
	if (tracker_algorithm.empty() || video_name.empty())
	{
		help();
		return -1;
	}

	//int coords[4] = { 0, 0, 0, 0 };
	int coords[4] = { 240, 210, 60, 104 };//未使用到
	bool initBoxWasGivenInCommandLine = false;

	//open the capture
	VideoCapture cap;
	cap.open(video_name);
	cap.set(CAP_PROP_POS_FRAMES, start_frame);

	if (!cap.isOpened())
	{
		help();
		cout << "***Could not initialize capturing...***\n";
		cout << "Current parameter's value: \n";
		parser.printMessage();
		return -1;
	}

	Mat frame;
	paused = true;
	namedWindow("Tracking API", 1);
	setMouseCallback("Tracking API", onMouse, 0);

	//instantiates the specific Tracker
	Ptr<Tracker> tracker = Tracker::create(tracker_algorithm);
	if (tracker == NULL)
	{
		cout << "***Error in the instantiation of the tracker...***\n";
		return -1;
	}

	//get the first frame
	cap >> frame;
	frame.copyTo(image);
	if (initBoxWasGivenInCommandLine){//改程序设为false没有使用到
		selectObject = true;
		paused = false;
		boundingBox.x = coords[0];
		boundingBox.y = coords[1];
		boundingBox.width = std::abs(coords[2] - coords[0]);
		boundingBox.height = std::abs(coords[3] - coords[1]);
		printf("bounding box with vertices (%d,%d) and (%d,%d) was given in command line\n", coords[0], coords[1], coords[2], coords[3]);
		rectangle(image, boundingBox, Scalar(255, 0, 0), 2, 1);
	}
	imshow("Tracking API", image);

	bool initialized = false;
	int frameCounter = 0;

	for (;;)
	{
		if (!paused)
		{
			if (initialized){
				cap >> frame;
				if (frame.empty()){
					break;
				}
				frame.copyTo(image);
			}

			if (!initialized && selectObject)
			{
				//initializes the tracker
				if (!tracker->init(frame, boundingBox))
				{
					cout << "***Could not initialize tracker...***\n";
					return -1;
				}
				initialized = true;
			}
			else if (initialized)
			{
				//updates the tracker
				if (tracker->update(frame, boundingBox))
				{
					rectangle(image, boundingBox, Scalar(255, 0, 0), 2, 1);
				}
			}
			imshow("Tracking API", image);
			frameCounter++;
		}

		char c = (char)waitKey(2);
		if (c == 'q')
			break;
		if (c == 'p')
			paused = !paused;

	}

	return 0;
}


