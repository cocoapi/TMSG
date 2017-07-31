#include <iostream>
#include <ctime>
#include "Headers.h"
using namespace std;

class Bullet {
public:
	int curb;
	int maxb;
	char* effect;
	char* name;
	XMVECTOR speed;
};

class Weapon {
public:
	//	model;
	char* name;
	int def, pow;
	float firerate, reloadtime;
};

class Armor {
public:
	//	model;
	char* name;
	int def, pow;
	XMVECTOR movespeed;
};

class Bomb {
public:
	char* name;
	int pow;
	//	range;
};


class Character
{
private:
	int HP;
	int Pow;
	int Def;
	Bullet Bul;
	//	directx~~ *model;
	XMVECTOR position;
	Weapon Wea;
	Armor Arm;
	Bomb Bom;
	time_t cooltime;
	enum status {
		normal, reloading, evasion, died
	};
	status sta;
public:
	Character(int a, int b, int c) : HP(a), Pow(b), Def(c), cooltime(NULL), sta(normal) {}
	~Character() {}

	// 값 초기화 함수
	void SetHP(int nowHP)
	{
		HP = nowHP;
	}
	void SetDef(int nowDef)
	{
		Def = nowDef;
	}
	void SetPow(int nowPow)
	{
		Pow = nowPow;
	}
	//	void SetBul(Bullet B)
	//	{
	//		Bul = B;
	//	}
	void SetEquip_W(Weapon W)
	{
		Wea = W;
	}
	void SetEquip_A(Armor &Ar)
	{
		//Arm = A;
	}
	void SetEquip_B(Bomb Bo)
	{
		Bom = Bo;
	}

	// 전투시 함수

	void Damaged(int EnemyPow)
	{
		EnemyPow = EnemyPow - Def;
		if (sta == evasion)
			return;
		else if (EnemyPow < 1) // 최소데미지 1
			HP--;
		else
			HP -= EnemyPow;

		if (HP <= 0)
			sta = died;
	}

	void Shot()
	{
		if (cooltime < time(NULL) && Bul.curb != 0 && sta == normal) // 총에 따른 연사속도를 가지고 사격간격을 맞출 필요성이 있음. 
		{
			/* 총알을 쏘아내는 부분 구현 필요 */
			Bul.curb--;
			cooltime = time(NULL) + Wea.firerate;
		}
	}

	void Reload()
	{
		sta = reloading;
		cooltime = time(NULL) + Wea.reloadtime;
		/* 리로딩 모션 */
	}

	void Status_check() // reloading 상태일때 normal상태로 돌리기위해 만들었지만 이후 추가될 상태에 따라 더 추가될수도 있음.
	{
		if (sta == reloading)
		{
			if (cooltime < time(NULL))
				sta = normal;
		}
		else if (sta == evasion)
		{
			if (cooltime < time(NULL))
				sta = normal;
		}
	}

	void Move()
	{
		//	position = position + 1 + Arm.movespeed; // 기본 이동속도 1 + Armor의 movespeed
	}

	void Evasion()
	{
		sta = evasion;
		cooltime = time(NULL) + 1;
		//	position = position + 3
	}
};