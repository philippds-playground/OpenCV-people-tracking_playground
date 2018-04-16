#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <string>
#include <cmath>
#include <vector>

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/highgui/highgui_c.h"
#include <stdio.h>

using namespace std;
using namespace cv;



int main()
{
	string imageSource = "sample_output.jpg";


	Mat img = imread(imageSource, IMREAD_GRAYSCALE);
	bool found = findChessboardCorners(img, boardSize, ptvec, CALIB_CB_ADAPTIVE_THRESH);
	

}