//http://funvision.blogspot.co.uk/2016/03/opencv-31-people-detection-at-13-fps-by.html

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/video/tracking.hpp"


#include <opencv2/videoio.hpp>
#include <iomanip>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/highgui.hpp>

#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>

#include <vector>
#include <stdio.h>
#include <Windows.h>
#include <iostream>
#include <time.h>
#include <ctime>



using namespace cv;
using namespace std;



int main()
{


	// prepare video input
	VideoCapture cap(0);

	// prepare video output
	//VideoWriter outputVideo;

	//outputVideo.open("video4.wmv", CV_FOURCC('W', 'M', 'V', '2'), cap.get(CV_CAP_PROP_FPS), Size(640, 480), true);

	// prepare cascadeClassifier
	CascadeClassifier detectorBody;
	CascadeClassifier detectorUpper;
	// !! Put your cascade or opencv cascede into project folder !!
	string cascadeName1 = "cascadeName.xml";
	string cascadeName2 = "cascadeName.xml";

	// Load cascade into CascadeClassifier
	bool loaded1 = detectorBody.load(cascadeName1);
	bool loaded3 = detectorUpper.load(cascadeName2);





	// Basic video input loop
	for (;;)
	{

		bool Is = cap.grab();
		if (Is == false) {

			cout << "Video Capture Fail" << endl;
			break;
		}
		else {
			// Just for measure time   
			const clock_t begin_time = clock();

			// Store results in these 2 vectors
			vector<Rect> human;
			vector<Rect> upperBody;

			// prepare 2 Mat container
			Mat img;
			Mat original;

			// capture frame from video file
			cap.retrieve(img, CV_CAP_OPENNI_BGR_IMAGE);
			// Resize image if you want with same size as your VideoWriter
			resize(img, img, Size(640, 480));
			// Store original colored image
			img.copyTo(original);
			// color to gray image
			cvtColor(img, img, CV_BGR2GRAY);

			// detect people, more remarks in performace section  
			detectorBody.detectMultiScale(img, human, 1.02, 2, 0 | 1, Size(40, 70), Size(80, 300));
			detectorUpper.detectMultiScale(img, upperBody, 1.02, 2, 0 | 1, Size(40, 70), Size(80, 300));



			// Draw results from detectorBody into original colored image
			if (human.size() > 0) {
				for (int gg = 0; gg < human.size(); gg++) {

					rectangle(original, human[gg].tl(), human[gg].br(), Scalar(0, 0, 255), 2, 8, 0);

				}
			}


			// Draw results from detectorUpper into original colored image
			if (upperBody.size() > 0) {
				for (int gg = 0; gg < upperBody.size(); gg++) {

					rectangle(original, upperBody[gg].tl(), upperBody[gg].br(), Scalar(255, 0, 0), 2, 8, 0);

				}
			}
			// measure time as current - begin_time
			clock_t diff = clock() - begin_time;
			// convert time into string
			char buffer[126];
			printf(buffer, "%d", diff);
			// display TIME ms on original image
			putText(original, buffer, Point(100, 20), 1, 2, Scalar(255, 255, 255), 2, 8, 0);
			putText(original, "ms", Point(150, 20), 1, 2, Scalar(255, 255, 255), 2, 8, 0);
			// draw results
			namedWindow("prew", WINDOW_AUTOSIZE);
			imshow("prew", original);
			// make video output
			//outputVideo << original;

			int key1 = waitKey(20);

		}
	}
}