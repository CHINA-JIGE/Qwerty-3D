#include <opencv2\opencv.hpp>
#include "Screen.h"
#include "Camera.h"

qwerty::Screen scr;
qwerty::Camera cam;

int main()
{
	std::vector<cv::Point2f> cornerList(25);
	cv::Mat cbGrayView1;
	cv::Mat chessboardView1 = cv::imread("..\\media\\group2\\6.jpg");
	bool isPatternFound = cv::findChessboardCorners(chessboardView1,cv::Size(7,5), cornerList,CV_CALIB_CB_ADAPTIVE_THRESH|CV_CALIB_CB_NORMALIZE_IMAGE);
	if (!isPatternFound)
	{
		std::cout << "failed to find chessboard pattern!" << std::endl;
	}
	else
	{
		//sub-pixel corners
		cv::cvtColor(chessboardView1, cbGrayView1, CV_BGR2GRAY);
		cv::cornerSubPix(cbGrayView1, cornerList, cv::Size(10, 10), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS +cv::TermCriteria::MAX_ITER, 30, 0.1));
		cv::drawChessboardCorners(chessboardView1, cv::Size(7, 5), cornerList, isPatternFound);
	}

	//cv::calibrateCamera(,)

	cv::imshow("testPicture", chessboardView1);
	cv::waitKey(20150901);
}