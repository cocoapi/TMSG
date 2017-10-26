#include "Headers.h"

GT::GT() :mSecondsPerCount(0.0), mDeltaTime(-1.0), mBaseTime(0), mPausedTime(0), mPrevTime(0), mCurrTime(0), mStopped(false) {
	_int64 CountsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&CountsPerSec);
	mSecondsPerCount = 1.0 / (double)CountsPerSec;
}


//�������Ӹ��� �ɸ� �ð� dTime�� ����
void GT::Tick() {
	if (mStopped) {
		mDeltaTime = 0.0;
		return;
	}
	else {
		QueryPerformanceFrequency((LARGE_INTEGER*)&mCurrTime);

		mDeltaTime = (mCurrTime - mPrevTime) * mSecondsPerCount;

		//���������ӿ� ����Ͽ�, ���簪�� ���������� �ʱ�ȭ
		mPrevTime = mCurrTime;

		//���μ����� ������尡 �ǰų�, �ٸ� ���μ����� ��Ű�� ��� DeltaTime�� ������ �ɼ������Ƿ�
		//������ ������.
		if (mDeltaTime < 0.0) { mDeltaTime = 0.0; }
	}
}

void GT::Reset() {
	_int64 resetTime;
	QueryPerformanceFrequency((LARGE_INTEGER*)&resetTime);

	mBaseTime = resetTime;
	mPrevTime = resetTime;
	mStopTime = 0;
	mStopped = false;
}