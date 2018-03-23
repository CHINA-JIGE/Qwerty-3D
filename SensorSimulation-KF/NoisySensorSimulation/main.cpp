#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <opencv2\opencv.hpp>

class Sensor
{
public:

	Sensor():
		mPos(1.0f,0,0),
		mVelocity(0,0,1.0f),
		mAcceleration(-1.0f,0,0)
		{}

	void Init(float initAccX, float initAccY, float initAccZ)
	{
		mKF_Acc.init(3, 3, 0, CV_32F);
		cv::setIdentity(mKF_Acc.transitionMatrix, cv::Scalar(1.0f));//just for init
		cv::setIdentity(mKF_Acc.errorCovPost, cv::Scalar(0.005f));// P
		cv::setIdentity(mKF_Acc.measurementMatrix, cv::Scalar(1.0f));//H
		cv::setIdentity(mKF_Acc.measurementNoiseCov, cv::Scalar(0.005f));//R
		cv::setIdentity(mKF_Acc.processNoiseCov, cv::Scalar(0.001f));//Q

		mKF_Acc.statePost = cv::Mat(3, 1, CV_32F, cv::Scalar(0));
		mKF_Acc.statePost.at<float>(0) = initAccX;
		mKF_Acc.statePost.at<float>(1) = initAccY;
		mKF_Acc.statePost.at<float>(2) = initAccZ;

	}

	//Let's assume that sensor are moving along a perfect circle on XZ plane
	void Update(cv::Vec3f measuredAcc,float dt)
	{
		//use kalman filter to ease the white noise of measure Acceleration
		mAcceleration = mFunction_KalmanFilterForAcc(mKF_Acc, dt, measuredAcc);
		//mAcceleration = measuredAcc;
		mVelocity += mAcceleration*dt;
		mPos += mVelocity * dt;
	};

	cv::Vec3f GetPos()
	{
		return mPos;
	}

private:

	cv::Vec3f mFunction_KalmanFilterForAcc(cv::KalmanFilter& kf, float dt,const cv::Vec3f& acc)
	{
		//x,y,z of acceleration
		cv::setIdentity(kf.transitionMatrix, cv::Scalar(1.0f));

		//predict stage
		kf.predict();

		//update according to measurement
		//x,y,z of acceleration
		cv::Mat measureMat(3, 1, CV_32F);
		measureMat.at<float>(0) = acc(0);
		measureMat.at<float>(1) = acc(1);
		measureMat.at<float>(2) = acc(2);
		kf.correct(measureMat);
		//cv::Vec3f correctStatePre = { kf.statePre.at<float>(0),kf.statePre.at<float>(1),kf.statePre.at<float>(2) };
		cv::Vec3f correctStatePost = { kf.statePost.at<float>(0),kf.statePost.at<float>(1),kf.statePost.at<float>(2) };

		return correctStatePost;
	}

	cv::KalmanFilter mKF_Acc;
	cv::Vec3f mAcceleration;
	cv::Vec3f mVelocity;
	cv::Vec3f mPos;
};

int main()
{
	Sensor sensor;
	const float noiseAmp = 1.0f;
	std::uniform_real_distribution<float> noiseDistribution(-noiseAmp, noiseAmp);
	std::default_random_engine noiseGenerator;
	std::vector<cv::Vec3f> predictedPosList;
	std::vector<float> distanceList;//it should be constantly 1.0f in ideal situation

	sensor.Init(-1.0f,0,0);
	for (int frameId = 0; frameId < 1000; ++frameId)
	{
		cv::Vec3f noisyAcc;
		const float dt = 0.01f;
		noisyAcc(0) = -cos(float(frameId) * dt) + noiseDistribution(noiseGenerator);
		noisyAcc(1) = noiseDistribution(noiseGenerator);
		noisyAcc(2) = -sin(float(frameId) * dt) + noiseDistribution(noiseGenerator);
		
		sensor.Update(noisyAcc, dt);
		cv::Vec3f predictedPos = sensor.GetPos();

		//kf result evaluation
		predictedPosList.push_back(predictedPos);
		distanceList.push_back(sqrt(predictedPos(0)*predictedPos(0) + predictedPos(1)*predictedPos(1) + predictedPos(2)*predictedPos(2)));
	}

	return 0;
}