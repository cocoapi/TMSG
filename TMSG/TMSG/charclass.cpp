#include <iostream>
#include "Headers.h"
using namespace std;

class Character
{
private:
	int HP;
	int Pow;
	int Def;
	Bullet Bul;
	directx~~ *model;
	XMVECTOR position;
	Equip Weapon;
	Equip Armor;
	Bomb Bom;
public:
	Character(int a, int b, int c) : HP(a), Pow(b), Def(c) {}
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
	void SetBul(Bullet B)
	{
		Bul = B;
	}
	void SetEquip_W(Equip E)
	{
		if (E.cat == 0)
			Weapon = E;
		else
			cout << "That's not Weapon." << endl;
	}
	void SetEquip_A(Equip E)
	{
		if (E.cat == 1)
			Armor = E;
		else
			cout << "That's not Armor." << endl;
	}
	void SetEquip_B(Bomb B)
	{
		Bom = B;
	}

	// 전투시 함수

	void Damaged(int EnemyPow)
	{
		EnemyPow = EnemyPow - Def;
		if (EnemyPow < 1) // 최소데미지 1
			HP--;
		else
			HP -= EnemyPow;
	}
};

class Bullet {
public:
	int curb;
	int maxb;
	char* effect;
	char* name;
	XMVECTOR speed;
};

class Equip {
public:
	model;
	char* name;
	int def, pow;
	enum category {
		WEAPON,
		ARMOR
	};
	category cat;
};

class Bomb {
public:
	char* name;
	int pow;
	range;
};
