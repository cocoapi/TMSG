#pragma once

class GT {
private:
	double mSecondsPerCount;
	double mDeltaTime;
	_int64 mBaseTime;
	_int64 mPausedTime;
	_int64 mStopTime;
	_int64 mPrevTime;
	_int64 mCurrTime;
	bool mStopped;

public:
	GT();
	void Tick();
	void Reset();
	void Stop();
	void Start();
	float TotalTime() const;
	float DeltaTime() const { return (float)mDeltaTime; }
};