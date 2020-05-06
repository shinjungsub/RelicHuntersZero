#pragma once
#include "Image.h"

class HpBar
{
private:
	int _maxHp;					//최대 Hp
	Vector2 _position;			//포지션
	Vector2 _backgroundSize;	//HpBar 뒤에 이미지 사이즈
	RECT _rc;
	Image* _background;			//뒤 이미지(검정)
	Image* _foreground;			//앞 이미지(빨강)
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

