#include <iostream>
#include <ctime>
#include "Headers.h"
using namespace std;

class RatedPoint;

class Character {
private:
	//Models
	XMVECTOR Postion;
	RatedPoint HP;
	RatedPoint Stamina;
	int NormalDefendRate;
	int SpecialDefendRate;

public:
	Character();
	Character(const int&, const int&, const int&, const int&);
	double getHPRate();
	double getStaminaRate();
};

Character::Character() {
	HP = 0;
	Stamina = 0;
	NormalDefendRate = 0;
	SpecialDefendRate = 0;
}

Character::Character(const int& newHP, const int& newStamina, const int& newNormal = 10, const int& newSpecial = 10) {
	HP = newHP;
	Stamina = newStamina;
	NormalDefendRate = newNormal;
	SpecialDefendRate = newSpecial;
}

double Character::getHPRate() {
	return HP.getNowRate();
}

double Character::getStaminaRate() {
	return Stamina.getNowRate();
}


class RatedPoint {
private:
	int MaxPoint;
	int CurrentPoint;
	bool MaxCheck();

public:
	const RatedPoint& operator=(const int&);
	RatedPoint();
	RatedPoint(const int&);
	RatedPoint(const int&, const int&);
	void setMaxPoint(const int&);
	void setCurPoint(const int&);
	double getNowRate();
};

RatedPoint::RatedPoint() {
	MaxPoint = 0;
	CurrentPoint = 0;
}

RatedPoint::RatedPoint(const int& newMax) {
	MaxPoint = newMax;
	CurrentPoint = newMax;
}

RatedPoint::RatedPoint(const int& newMax, const int& newCur) {
	MaxPoint = newMax;
	CurrentPoint = newCur;
}

void RatedPoint::setMaxPoint(const int& newMax) {
	MaxPoint = newMax;
	if (!MaxCheck()) {
		CurrentPoint = newMax;
	}
}

void RatedPoint::setCurPoint(const int& newCur) {
	CurrentPoint = newCur;
	if (!MaxCheck()) {
		CurrentPoint = MaxPoint;
	}
}

double RatedPoint::getNowRate() {
	return CurrentPoint / MaxPoint;
}

bool RatedPoint::MaxCheck() {
	if (MaxPoint < CurrentPoint) {
		return false;
	} else {
		return true;
	}
}

const RatedPoint& RatedPoint::operator=(const int& newMax) {
	RatedPoint *newPoints = new RatedPoint(newMax);
	return *newPoints;
}