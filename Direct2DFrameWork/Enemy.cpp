#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy()
{
}

Enemy::Enemy(string name, Vector2 pos, Vector2 mainPos, Vector2 hitPos, Vector2 mainSize, Vector2 hitSize, Vector2 imageSize)
	:GameObject(name, pos, mainPos, hitPos, mainSize, hitSize, imageSize, Pivot::CENTER)
{
	_aStar = new AStar;
	_hpBar = new HpBar(150);
}


Enemy::~Enemy()
{
	delete _aStar;
	delete _hpBar;
}

void Enemy::Init()
{
}

void Enemy::Release()
{
}

void Enemy::Update()
{
}

void Enemy::Render()
{
}

//�̹��� ������ ������Ʈ�ϴ� �Լ�
void Enemy::UpdateImageFrame()
{
	_imageCount++;
	if (_imageCount > 5)
	{
		if (_imageFrameX < _image->GetMaxFrameX() - 1)
		{
			_imageFrameX++;
		}
		else
		{
			//�¾��� �� ���¿� �뽬 ������ ������ �̹����� ������ �����ӱ��� ������ �� �� Stay ���·� ������
			if (this->_kindOfImage == KindOfImage::Hit || this->_kindOfImage == KindOfImage::Dash)
			{
				_kindOfImage = KindOfImage::Idle;
				_state = State::Stay;
				_speed = 2.0f;
			}
			if (this->_kindOfImage == KindOfImage::Death)
				_isActive = false;
			else
				_imageFrameX = 0;
		}
		_imageCount = 0;
	}
}
