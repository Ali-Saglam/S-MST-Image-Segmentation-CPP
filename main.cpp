/*
Saglam, Ali, & Baykan, N. A. (2017).
"Sequential image segmentation based on min- imum spanning tree representation".
Pattern Recognition Letters, 87 , 155–162.
https://doi.org/10.1016/j.patrec.2016.06.001 .
*/


#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "smst.h"
#include <fstream>

using namespace cv;
using namespace std;

int main(int argc, char *argv[]) {

	cv::Mat img, segcolors;

	/*----------------------------------*/

	img = imread("inputs/3096.jpg");
	if (img.empty()) return -1;

	/*----------------------------------*/

	GaussianBlur(img, img, Size(5, 5), 1.0);

	/*---------|--------------|----------*/
	/*---------| SEGMENTATION |---------*/
	/*---------V--------------V---------*/

	smst mysmst(img);
	mysmst.segmentation(200, 3);

	/*----------------------------------*/
	/*----------------------------------*/

	segcolors = mysmst.getColoredSegments();
	imwrite("outputs/3096_a.png", segcolors);

	/*----------------------------------*/

	cv::Mat labels = mysmst.getLabels();

	/*----------------------------------*/

	vector<vector<Node::Pixel>> segments = mysmst.segments;  ///// segments[segment_num][pixel_num].x  ,  segments[segment_num][pixel_num].y

	/*----------------------------------*/

	return 0;
}
