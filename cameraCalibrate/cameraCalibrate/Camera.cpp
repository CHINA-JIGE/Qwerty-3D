/***********************************************************************

							qwertyCamera:
		encapsulation of virtual camera and physical camera

*************************************************************************/

#include "Camera.h"

const cv::Size2i qwerty::Camera::cDefaultChessboardSize = { 7, 5 };
const cv::Size2f qwerty::Camera::cDefaultChessboardGridWorldSpaceSize = { 20.0f,20.0f };//20mm x 20mm

qwerty::Camera::Camera():
	mIsOkToCalibrate(false),
	mIntrinsicMatrix(3,3,CV_32FC1),
	mExtrinsicMatrix(3,3,CV_32FC1)
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
	//pixel size of input image
	mImagePixelSize = cv::Size(image.size().width,image.size().height);

	//temp recognized corners list
	std::vector<cv::Point3f> objectPointList;
	std::vector<cv::Point2f> cornerList;

	bool isPatternFound = cv::findChessboardCorners(image, cDefaultChessboardSize, cornerList, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);
	if (isPatternFound)
	{
		//sub-pixel level corners fine-tuning
		cv::Mat cbGrayView1;
		cv::cvtColor(image, cbGrayView1, CV_BGR2GRAY);
		cv::cornerSubPix(cbGrayView1, cornerList, cv::Size(10, 10), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.1));
		
		//draw markers to image and present to window
		if (showImage)
		{
			cv::drawChessboardCorners(image, cDefaultChessboardSize, cornerList, isPatternFound);
			cv::imshow("qwerty chessboard", image);
			cv::waitKey(20150901);
		}

		//add to image points list (array of arrays)
		mImagePoints.push_back(std::move(cornerList));

		//corresponding World Space object points
		for (int j = 0; j < cDefaultChessboardSize.height; ++j)
		{
			for (int i = 0; i < cDefaultChessboardSize.width; ++i)
			{
				//should Z coordinate keeps as 0? (2018.2.25) i think maybe it's not necessary.
				const float gridWidth = cDefaultChessboardGridWorldSpaceSize.width;
				const float gridHeight = cDefaultChessboardGridWorldSpaceSize.height;
				const int cornerCountX = cDefaultChessboardSize.width;
				const int cornerCountY = cDefaultChessboardSize.height;
				cv::Point3f correspondingObjectPointW;
				correspondingObjectPointW.x = (i - cornerCountX / 2) * gridWidth;
				correspondingObjectPointW.y = (cornerCountY / 2 - j) * gridHeight;
				correspondingObjectPointW.z = 0;
				objectPointList.push_back(correspondingObjectPointW);
			}
		}
		mObjectPointsW.push_back(std::move(objectPointList));

		//update "Is it OK to calibrate?"
		//K images,N corners, constrains: 2NK >= 6K + 4 
		//only the inequality satisfy that calibration can be performed
		//but taken noise into consideration, MORE images will be better
		if (mImagePoints.size() > 5)
		{
			mIsOkToCalibrate = true;
		}

		return true;
	}
	else
	{
		std::cout << "RecognizeChessboard : failed to find chessboard pattern!" << std::endl;
		return false;
	}
}

bool qwerty::Camera::ReadyToCalibrate()
{
	return mIsOkToCalibrate;
}

//camera calibration based on [Zhang00]'s method
//several images taken from different view points are needed
//distortion coeffecient & intrinsic parameters & extrinsic parameters
//will be solved simultaneously with those 
//points information in given images.(3d object points and its projective 2d coord)
bool qwerty::Camera::Calibrate()
{
	if (!mIsOkToCalibrate)
	{
		std::cout << "Camera::Calibrate : not ready to calibrate!!" << std::endl;
		return false;
	}

	//(3,3,CV_32FC1) CV_32FC1 : 32 bits float Channel 1
	cv::Mat intrinsicMatrix;//fx,fy,cx,cy, param which describes projection matrix/camera matrix
	cv::Mat extrinsicMatrix(3, 3, CV_32FC1);//posture description
	std::vector<float> distortionCoeff;//un-ideal feature(radial/tangential distortion) of real-world lens
	std::vector<cv::Mat> rotationVectors;
	std::vector<cv::Mat> translationVectors;
	cv::calibrateCamera(
		mObjectPointsW,
		mImagePoints,
		mImagePixelSize,
		intrinsicMatrix,
		distortionCoeff,
		rotationVectors,
		translationVectors,
		NULL//cv::CALIB_FIX_ASPECT_RATIO
	);

	//..
	mDistortionCoeff = std::move(distortionCoeff);
	mIntrinsicMatrix = std::move(intrinsicMatrix);
	
	//emmm, actually i only need one extrinsic matrix as initiative posture
	//but to test the calibration correctness, all calibrated result will be saved to file
	std::ofstream outFile("calib_posture.txt");
	for (int i = 0; i < rotationVectors.size(); ++i)
	{
		cv::Mat rotationMatrix;
		cv::Rodrigues(rotationVectors.at(i), rotationMatrix);
		cv::Mat transposeRotM = rotationMatrix.t();

		outFile << "calibration result for image " << i + 1 << ":" << std::endl;
		outFile << "Rotation matrix: " << std::endl;
		outFile << "    " << transposeRotM.at<double>(0, 0) << " " << transposeRotM.at<double>(0, 1) << " " << transposeRotM.at<double>(0, 2) << std::endl;
		outFile << "    " << transposeRotM.at<double>(1, 0) << " " << transposeRotM.at<double>(1, 1) << " " << transposeRotM.at<double>(1, 2) << std::endl;
		outFile << "    " << transposeRotM.at<double>(2, 0) << " " << transposeRotM.at<double>(2, 1) << " " << transposeRotM.at<double>(2, 2) << std::endl;

		//for how to convert axis-angle to euler-angles, refer to wikipedia:
		//https://en.wikipedia.org/wiki/Rotation_formalisms_in_three_dimensions#Rotation_matrix
		//https://en.wikipedia.org/wiki/Euler_angles
		//1. use Rodrigues formula to convert axis-angle to rotation matrix
		//		(note that the rvec and tvec are defined in VIEW SPACE ,
		//		with the pos of camera centered at origin, not WORLD SPACE)
		//2. extract euler angles from the rotation matrix
		//the rotation matrix can be interpreted in differnet ways (6 ways for yaw-pitch-roll decomposition)
		//(it depends on the definition of Euler Angle(Yaw-pitch-roll)


		//		R=Z1Y2X3 (right-handed, z-up) x~roll, y-pitch, z-yaw
		//			[c1c2		c1s2s3-c3s1		s1s3+c1c3s2	]
		//		=	[c2s1		c1c3+s1s2s3	c3s1s2-c1s3		]
		//			[-s2			c2s3					c2c3					]
		//initial extraction(right-handed)
		//double eulerAngleX = atan2(rotationMatrix.at<double>(1, 0), rotationMatrix.at<double>(0, 0));
		//double eulerAngleY = asin(-rotationMatrix.at<double>(2, 0));
		//double eulerAngleZ = atan2(rotationMatrix.at<double>(2, 1), rotationMatrix.at<double>(2, 2));

		//		R=Y1X2Z3 (right-handed, z-up) z~roll, x-pitch, y-yaw
		//			[c1c3+s1s2s3		c3s1s2-c1s3		c2s1	]
		//		=	[c2s3					c2c3					-s2	]
		//			[c1s2s3-c3s1		c1c3s2+s1s3	c1c2	]
		//initial extraction(right-handed)
		//because Chessboard has AMBIGUITY because chessboard is rotational-asymmetric
		//so in calibration/init stage, roll angle will be limited to [-pi/2,pi/2], thus asin() is adopted
		const double PI = 3.141592653589;
		double s2 = transposeRotM.at<double>(1, 2);
		double eulerAngleY = atan2(transposeRotM.at<double>(0, 2), transposeRotM.at<double>(2, 2));
		double eulerAngleX = asin(-transposeRotM.at<double>(1, 2));
		double eulerAngleZ = (s2 == 1.0 ? PI / 2.0 : asin(transposeRotM.at<double>(1, 0) / sqrt(1.0 - s2*s2)));
		//double eulerAngleZ= atan2(transposeRotM.at<double>(1, 0), transposeRotM.at<double>(1, 1));

		//but for Noise3D's convenience, handness will be converted
		double noise3dEulerX = eulerAngleX;
		double noise3dEulerY = -eulerAngleY+ PI;//z-axis inverse, rotate direction inverse
		double noise3dEulerZ = eulerAngleZ;
		outFile << "Euler Angle X,Y,Z for cv(right-hand system):" << eulerAngleX << ", " << eulerAngleY << ", " << eulerAngleZ << std::endl;
		outFile << "Euler Angle X,Y,Z for Noise3D (left-hand system):" << noise3dEulerX << ", " << noise3dEulerY << ", " << noise3dEulerZ << std::endl;


		/*outFile << "Rotation matrix from extracted Euler Angle:" << std::endl;
		double c1 = cos(eulerAngleY), c2 = cos(eulerAngleX), c3 = cos(eulerAngleZ);
		double s1 = sin(eulerAngleY), s2 = sin(eulerAngleX), s3 = sin(eulerAngleZ);
		outFile << "    "<< c1*c3 + s1*s2*s3 << ", " << c3*s1*s2 - c1*s3 << ", " << c2*s1 << std::endl;
		outFile << "    " << c2*s3 << ", " << c2*c3 << ", " << -s2 << std::endl;
		outFile << "    " << c1*s2*s3 - c3*s1 << ", " << c1*c3*s2 + s1*s3 << ", " << c1*c2 << std::endl;*/

		//translation of camera (tvec output by cv::Rodrigues is chessboard pos in VIEW SPACE)
		//inv([R,T]) = [R', -R'T]
		//https://stackoverflow.com/questions/18637494/camera-position-in-world-coordinate-from-cvsolvepnp
		//ortho matrix: inverse=transpose
		cv::Mat cameraWorldPos = -transposeRotM *translationVectors.at(i);
		double posX = cameraWorldPos.at<double>(0);
		double posY = cameraWorldPos.at<double>(1);
		double posZ = cameraWorldPos.at<double>(2);

		//in noise3d, screen lies in YZ plane
		double noise3dPosX = posX;
		double noise3dPosY = posY;
		double noise3dPosZ = -posZ;
		outFile << "translation for cv camera: " << posX << ", " << posY << ", " << posZ << std::endl;
		outFile << "translation for noise3d camera: " << noise3dPosX << ", " << noise3dPosY << ", " << noise3dPosZ << std::endl << std::endl;
	}

	outFile.close();

	return true;
}
