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
		QueryPerformanceCounter((LARGE_INTEGER*)&mCurrTime);

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
	QueryPerformanceCounter((LARGE_INTEGER*)&resetTime);

	mBaseTime = resetTime;
	mPrevTime = resetTime;
	mStopTime = 0;
	mStopped = false;
}

void GT::Stop() {
	if (!mStopped) {
		_int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		mStopTime = currTime;
		mStopped = true;
	}
}

void GT::Start() {
	_int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	
	if (mStopped) {
		//�Ͻ������ð� ����
		mPausedTime += (currTime - mStopTime);

		//������üũ�� ����� �����ð� ���� ������ϴ� ���� �ð����� �� �ʱ�ȭ
		mPrevTime = currTime;
		mStopTime = 0;
		mStopped = false;
	}
}

float GT::TotalTime() const {
	if (mStopped) {
		return (float)((mStopTime - mPausedTime)) - mBaseTime * mSecondsPerCount;
	}
	else {
		return (float)(((mCurrTime - mPausedTime) - mBaseTime)*mSecondsPerCount);
	}
}