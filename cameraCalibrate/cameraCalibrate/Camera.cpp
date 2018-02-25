/***********************************************************************

							qwertyCamera:
		encapsulation of virtual camera and physical camera

*************************************************************************/

#include "Camera.h"

const cv::Size qwerty::Camera::cDefaultChessboardSize = { 7, 5 };
const cv::Size qwerty::Camera::cDefaultChessboardGridWorldSpaceSize = { 20.0f,20.0f };//20mm x 20mm

qwerty::Camera::Camera()
{
}

bool qwerty::Camera::RecognizeChessboard(const cv::String & imagePath, bool showImage)
{
	cv::Mat cbView1 = cv::imread(imagePath);
	if (cbView1.data == nullptr)
	{
		std::cout << "RecognizeChessboard : failed to open target image!" << std::endl;
		return false;
	}
	
	return qwerty::Camera::RecognizeChessboard(cbView1, showImage);
}

bool qwerty::Camera::RecognizeChessboard(const cv::Mat & image, bool showImage)
{
	//temp recognized corners list
	std::vector<cv::Point3f> objectPointList;
	std::vector<cv::Point2f> cornerList;

	bool isPatternFound = cv::findChessboardCorners(image, cDefaultChessboardSize, cornerList, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);
	if (!isPatternFound)
	{
		std::cout << "RecognizeChessboard : failed to find chessboard pattern!" << std::endl;
		return false;
	}
	else
	{
		//sub-pixel corners
		cv::Mat cbGrayView1;
		cv::cvtColor(image, cbGrayView1, CV_BGR2GRAY);
		cv::cornerSubPix(cbGrayView1, cornerList, cv::Size(10, 10), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.1));
		mImagePoints.push_back(std::move(cornerList));

		//corresponding world space object points
		for (int j = 0; j < cDefaultChessboardSize.height; ++j)
		{
			for (int i = 0; i < cDefaultChessboardSize.width; ++i)
			{
				//should Z coordinate keeps as 0? (2018.2.25) i think maybe it's not necessary.
				const float gridWidth = cDefaultChessboardGridWorldSpaceSize.width;
				const float gridHeight = cDefaultChessboardGridWorldSpaceSize.height;
				const int cornerCountX = cDefaultChessboardSize.width;
				const int cornerCountY = cDefaultChessboardSize.height;
				objectPointList.push_back(cv::Point3f((i - cornerCountX/2) * gridWidth , 0, (cornerCountY/2 - j) * gridHeight));
			}
		}
		mObjectPointsW.push_back(std::move(objectPointList));

		//draw markers to image and present to window
		if (showImage)
		{
			cv::drawChessboardCorners(image, cDefaultChessboardSize, cornerList, isPatternFound);
			cv::imshow("qwerty chessboard", image);
			cv::waitKey(20150901);
		}

		return true;
	}
}
