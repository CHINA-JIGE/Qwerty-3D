
/***********************************************************************

                           h��NoiseTimer

************************************************************************/

#pragma once


namespace Noise3D
{
	namespace Ut
	{
		enum NOISE_TIMER_TIMEUNIT
		{
			NOISE_TIMER_TIMEUNIT_MILLISECOND = 0,
			NOISE_TIMER_TIMEUNIT_SECOND = 1
		};

		class /*_declspec(dllexport)*/ ITimer
		{
		public:

			ITimer();

			ITimer(NOISE_TIMER_TIMEUNIT timeUnit);

			//�ں��������const��ʾ�˺��������޸����Ա
			//��ͣ���ټ�
			double GetTotalTimeElapsed()const;

			//��ȡ���ʱ��
			double GetInterval()const;

			//����ʱ�䵥λ
			void SetTimeUnit(NOISE_TIMER_TIMEUNIT timeUnit = NOISE_TIMER_TIMEUNIT_MILLISECOND);

			//������һ֡
			void NextTick();

			//....
			UINT GetFPS() const;

			//��ͣ��ʱ
			void Pause();

			void Continue();

			//������������
			void ResetAll();

			void ResetTotalTime();

			void	SetTimeIntervalClamp(double maxInterval);

			void	SetTimeScale(double scaleFactor);

			double GetTimeScale() const ;

		private:
			//���º����ʱ��(ms)
			double					mTotalTime;
			//����INTERVAL ��֡���ʱ����
			double					mDeltaTime;
			//how many MS does one COUNT takes
			double					mMilliSecondsPerCount;
			//how many ticks are in current second
			UINT						mCurrentSecondTickCount;
			//a rounded total time
			UINT						mCurrentSecondInteger;
			//frames per second
			UINT						mFPS;

			//ʱ����
			NOISE_TIMER_TIMEUNIT	 mTimeUnit;
			//
			bool					mIsPaused;
			//queryPerformanceò�Ʒ��ص��Ǿ���count����������query���count
			INT64					mPrevCount;
			//
			INT64					mCurrentCount;
			//
			INT64					mDeltaCount;
			//max time interval, it means that GetInterval will be CLAMPed;
			double						mMaxInterval;
			//time scale factor, which will be multiplied in Time Getter function
			double						mTimeScaleFactor;
		};
	}
}