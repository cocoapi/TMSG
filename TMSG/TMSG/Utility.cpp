#include "Headers.h"

GT::GT() :mSecondsPerCount(0.0), mDeltaTime(-1.0), mBaseTime(0), mPausedTime(0), mPrevTime(0), mCurrTime(0), mStopped(false) {
	_int64 CountsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&CountsPerSec);
	mSecondsPerCount = 1.0 / (double)CountsPerSec;
}


//매프레임마다 걸린 시간 dTime을 연산
void GT::Tick() {
	if (mStopped) {
		mDeltaTime = 0.0;
		return;
	}
	else {
		QueryPerformanceFrequency((LARGE_INTEGER*)&mCurrTime);

		mDeltaTime = (mCurrTime - mPrevTime) * mSecondsPerCount;

		//다음프레임에 대비하여, 현재값을 이전값으로 초기화
		mPrevTime = mCurrTime;

		//프로세서가 절전모드가 되거나, 다른 프로세서와 엉키는 경우 DeltaTime이 음수가 될수있으므로
		//음수를 방지함.
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