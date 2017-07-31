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

	// �� �ʱ�ȭ �Լ�
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

	// ������ �Լ�

	void Damaged(int EnemyPow)
	{
		EnemyPow = EnemyPow - Def;
		if (sta == evasion)
			return;
		else if (EnemyPow < 1) // �ּҵ����� 1
			HP--;
		else
			HP -= EnemyPow;

		if (HP <= 0)
			sta = died;
	}

	void Shot()
	{
		if (cooltime < time(NULL) && Bul.curb != 0 && sta == normal) // �ѿ� ���� ����ӵ��� ������ ��ݰ����� ���� �ʿ伺�� ����. 
		{
			/* �Ѿ��� ��Ƴ��� �κ� ���� �ʿ� */
			Bul.curb--;
			cooltime = time(NULL) + Wea.firerate;
		}
	}

	void Reload()
	{
		sta = reloading;
		cooltime = time(NULL) + Wea.reloadtime;
		/* ���ε� ��� */
	}

	void Status_check() // reloading �����϶� normal���·� ���������� ��������� ���� �߰��� ���¿� ���� �� �߰��ɼ��� ����.
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
		//	position = position + 1 + Arm.movespeed; // �⺻ �̵��ӵ� 1 + Armor�� movespeed
	}

	void Evasion()
	{
		sta = evasion;
		cooltime = time(NULL) + 1;
		//	position = position + 3
	}
};