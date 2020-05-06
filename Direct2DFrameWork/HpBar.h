#pragma once
#include "Image.h"

class HpBar
{
private:
	int _maxHp;					//�ִ� Hp
	Vector2 _position;			//������
	Vector2 _backgroundSize;	//HpBar �ڿ� �̹��� ������
	RECT _rc;
	Image* _background;			//�� �̹���(����)
	Image* _foreground;			//�� �̹���(����)
public:
	HpBar() {}
	HpBar(int maxHp) :_maxHp(maxHp)
	{
		_background = IMAGEMANAGER->FindImage("HpBarBackground");
		_foreground = IMAGEMANAGER->FindImage("HpBarForeground");
		_backgroundSize = { 61, 7 };
	}
	void Render(Vector2 pos, int currentHp);
};

