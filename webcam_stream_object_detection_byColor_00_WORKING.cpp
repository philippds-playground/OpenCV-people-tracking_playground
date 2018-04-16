// https://www.youtube.com/watch?v=0FG3zMdASz0

#include <iostream> // for standard I/O
#include <string>   // for strings
#include <opencv2/imgproc.hpp>  // Gaussian Blur
#include <opencv2/highgui.hpp>  // OpenCV window I/O

using namespace std;
using namespace cv;

int theObject[2] = { 0, 0 };
// bounding rectangle of the object, we will use the center of this as its position
Rect ojectBoundingRectangle = Rect(0, 0, 0, 0);

void searchForMovement(Mat thresholdImage, Mat& cameraFeed)
{
	// notice how we use the & operator for the cameraFeed. This is because we wish
	// to take the values passed into the function and manupulate them, rather than just working with a copy.
	// eg. we draw the cameraFeed in this function which is then displayed in the main() function.

	bool objectDetected = false;
	Mat temp;
	thresholdImage.copyTo(temp);

	// these two vectors needed for output of findContours
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	// find the contours of filtered image using openCV findContours function
	// findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); // retrieves all contours
	findContours(temp, contours, hierarchy, 0, 1);

	// if contours vector is not empty, we have found some objects
	if (contours.size() > 0) objectDetected = true;
	else objectDetected = false;

	if (objectDetected)
	{
		// the largest contour is foudn at the end of the contours vector
		// we will simply assume that the biggest contour is the object we are looking for
		vector<vector<Point>> largestContourVec;
		largestContourVec.push_back(contours.at(contours.size() - 1));

		// make a bounding rectangle around the largest ontour then find its centroid
		// this will be the objects final estimated position
		ojectBoundingRectangle = boundingRect(largestContourVec.at(0));
		int xpos = ojectBoundingRectangle.x + ojectBoundingRectangle.width / 2;
		int ypos = ojectBoundingRectangle.y + ojectBoundingRectangle.height / 2;

		// update the objects positions by changing the "theObject" array values
		theObject[0] = xpos;
		theObject[1] = ypos;
	}

	// make some temp x and y variables so we dont have to type out so much
	int x = theObject[0];
	int y = theObject[1];

	// draw some crosshairs on the object
	circle(cameraFeed, Point(x, y), 20, Scalar(0, 255, 0), 2);
	line(cameraFeed, Point(x, y), Point(x, y - 25), Scalar(0, 255, 0), 2);
	line(cameraFeed, Point(x, y), Point(x, y + 25), Scalar(0, 255, 0), 2);
	line(cameraFeed, Point(x, y), Point(x - 25, y), Scalar(0, 255, 0), 2);
	line(cameraFeed, Point(x, y), Point(x + 25, y), Scalar(0, 255, 0), 2);

	// putText(cameraFeed, "Tracking object at ("+ x, Point(x, y), 1, 1, Scalar(0, 0, 255), 2);
}

int main()
{
	// capture the video from web cam
	VideoCapture cap(0); 

	// if not success, exit program
	if (!cap.isOpened()) 
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}

	// create a window called "Control"
	namedWindow("Control", 1);

	int iLowH = 0;
	int iHighH = 179;

	int iLowS = 0;
	int iHighS = 255;

	int iLowV = 0;
	int iHighV = 255;

	// create trackbars in "Control" window
	// Hue (0 - 179)
	createTrackbar("LowH", "Control", &iLowH, 179);
	createTrackbar("HighH", "Control", &iHighH, 179);

	// Saturation (0 - 255)
	createTrackbar("LowS", "Control", &iLowS, 179);
	createTrackbar("HighS", "Control", &iHighS, 179);

	// Value (0 - 255)
	createTrackbar("LowV", "Control", &iLowV, 179);
	createTrackbar("HighV", "Control", &iHighV, 179);

	while (true)
	{
		Mat imgOriginal;

		// read a new frame from video
		bool bSuccess = cap.read(imgOriginal);

		// if not sucess, break loop
		if (!bSuccess) 
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		Mat imgHSV;

		// convert the captured frame from BGR to HSV
		cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);

		Mat imgThresholded;

		// Threshold the image
		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded);

		// morphological opening (remove small objects from foreground)
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		// morphological closing (fill small holes in the foreground)
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		searchForMovement(imgThresholded, imgOriginal);

		// show the thresholded image
		imshow("Thresholded Image", imgThresholded);

		// show the original image
		imshow("Original Image", imgOriginal);

		// wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
		if (waitKey(30) == 27)
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
}