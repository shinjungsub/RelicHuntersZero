#include "stdafx.h"
#include "Gun.h"

//총알 충돌 함수
void Gun::BulletCollision(int i)
{
	if (!_bullets[i].isFire) return;
	int power = 0;
	_bullets[i].isFire = false;
	_bullets[i].isHit = true;
	power = _bullets[i].power;
}

//총알 발사 함수
void Gun::Shoot()
{
	if (!_isShoot) return;
	if (_isShootDelay) return;

	//총구 위치 잡기
	_muzzlePos = { _position.x + cosf(_angle) * _gunLength, _position.y - sinf(_angle) * _gunLength };
	float angle = 0.0f;
	if (_isLeft) angle = _angle - (Math::PI / 2.0f);
	else angle = _angle + (Math::PI / 2.0f);
	if (angle > 2.0f * Math::PI) angle -= 2.0f * Math::PI;
	if (angle < 0.0f) angle = 2.0f * Math::PI + angle;
	_muzzlePos = { _muzzlePos.x + cosf(angle) * ((_size.y / 2.0f) - 5.0f), _muzzlePos.y - sinf(angle) * ((_size.y / 2.0f) - 5.0f) };

	//총의 종류에 따라 발사 안된 총알에 정보 넘겨주기
	switch (_kinds)
	{
	case Gun::GunKinds::None:
		break;
	case Gun::GunKinds::SingleGun:
		for (int i = 0; i < BULLETMAX; i++)
		{
			if (_bullets[i].isFire) continue;
			if (_bullets[i].isHit) continue;
			_bullets[i].image = IMAGEMANAGER->FindImage("spr_bullet_1");
			_bullets[i].hitImage = IMAGEMANAGER->FindImage("light_hit_effect");
			_bullets[i].position = _muzzlePos;
			_bullets[i].angle = _angle;
			_bullets[i].speed = 10.0f;
			_bullets[i].power = _power;
			_bullets[i].isFire = true;
			_bullets[i].isHit = false;
			_bullets[i].imageFrameX = 0;
			_bullets[i].imageCount = 0;
			_isShoot = false;
			break;
		}
		break;
	case Gun::GunKinds::MachineGun:
		for (int i = 0; i < BULLETMAX; i++)
		{
			if (_bullets[i].isFire) continue;
			if (_bullets[i].isHit) continue;
			_bullets[i].image = IMAGEMANAGER->FindImage("spr_bullet_blue_1");
			_bullets[i].hitImage = IMAGEMANAGER->FindImage("heavy_hit_effect");
			_bullets[i].position = _muzzlePos;
			_bullets[i].angle = _angle;
			_bullets[i].speed = 10.0f;
			_bullets[i].power = _power;
			_bullets[i].isFire = true;
			_bullets[i].isHit = false;
			_bullets[i].imageFrameX = 0;
			_bullets[i].imageCount = 0;
			break;
		}
		break;
	case Gun::GunKinds::ShotGun:
		_shootCount = 0;
		for (int i = 0; i < BULLETMAX; i++)
		{
			if (_bullets[i].isFire) continue;
			if (_bullets[i].isHit) continue;
			_shootCount++;
			_bullets[i].image = IMAGEMANAGER->FindImage("spr_bullet_critical_1");
			_bullets[i].hitImage = IMAGEMANAGER->FindImage("medium_hit_effect");
			_bullets[i].position = _muzzlePos;
			_bullets[i].angle = _angle - ((float)(3 - _shootCount) * 0.2f);
			_bullets[i].speed = 10.0f;
			_bullets[i].power = _power;
			_bullets[i].isFire = true;
			_bullets[i].isHit = false;
			_bullets[i].imageFrameX = 0;
			_bullets[i].imageCount = 0;
			if (_shootCount == 5)
				break;
		}
		_shootCount = 0;
		_isShoot = false;
		break;
	default:
		break;
	}
	_isShootDelay = true;
}

void Gun::BulletUpdate()
{
	for (int i = 0; i < BULLETMAX; i++)
	{
		//충돌한 총알 이미지 프레임 업데이트
		if (!_bullets[i].isFire)
		{
			if (_bullets[i].isHit)
			{
				_bullets[i].imageCount++;
				if (_bullets[i].imageCount > 5)
				{
					_bullets[i].imageFrameX++;
					if (_bullets[i].imageFrameX == 3)
					{
						_bullets[i].isHit = false;
						_bullets[i].imageFrameX = 0;
					}
					_bullets[i].imageCount = 0;
				}
			}
			else
			{
				continue;
			}
		}
		else
		{
			//현재 이동중인 총알의 포지션 및 밖으로 나가는 총알 fire false로 바꿔주기
			_bullets[i].position.x += cosf(_bullets[i].angle) * _bullets[i].speed;
			_bullets[i].position.y -= sinf(_bullets[i].angle) * _bullets[i].speed;
			_bullets[i].rc = Figure::RectMakeCenter(_bullets[i].position, _bullets[i].size);
			if (_bullets[i].position.x > 64 * 40 || _bullets[i].position.x < 0)
				_bullets[i].isFire = false;
			if(_bullets[i].position.y > 64 * 30 || _bullets[i].position.y < 0)
				_bullets[i].isFire = false;
		}
	}
}

void Gun::Init()
{
	_kinds = GunKinds::None;
	_muzzlePos = { 0.0f, 0.0f };
	_position = { 0.0f, 0.0f };
	_isRender = false;
	_isShoot = false;
	_isLeft = false;
	_isShootDelay = false;
	_shootDelayTime = 0.0f;
	_angle = 0.0f;
	_image = IMAGEMANAGER->FindImage("pistol_0");
	_shootCount = 0;
	for (int i = 0; i < BULLETMAX; i++)
	{
		Bullet bullet;
		bullet.position = { 0.0f, 0.0f };
		bullet.size = { 46.0f, 30.0f };
		bullet.angle = 0.0f;
		bullet.speed = 10.0f;
		bullet.isFire = false;
		bullet.image = IMAGEMANAGER->FindImage("spr_bullet_1");
		bullet.isHit = false;
		bullet.imageFrameX = 0;
		_bullets.push_back(bullet);
	}
}

void Gun::Update()
{
	BulletUpdate();

	//총 쏘는 것에 딜레이 걸어주기
	if (!_isShootDelay) return;
	float delay = _kinds == GunKinds::ShotGun ? 0.5f : 0.3f;
	_shootDelayTime += TIMEMANAGER->getElapedTime();
	if (_shootDelayTime > delay)
	{
		_isShootDelay = false;
		_shootDelayTime = 0.0f;
	}
}

void Gun::Render()
{
	RECT camRc = CAMERA->GetCamera();
	for (int i = 0; i < BULLETMAX; i++)
	{
		//총알이 충돌했을 때 이미지 렌더
		if (!_bullets[i].isFire)
		{
			if (_bullets[i].isHit)
			{
				_bullets[i].hitImage->SetSize({ 62, 34 });
				_bullets[i].hitImage->FrameRender(_bullets[i].position.x - camRc.left, _bullets[i].position.y - camRc.top, _bullets[i].imageFrameX, 0);
			}
			else
			{
				continue;
			}
		}
		else
		{
			//총알 이미지 렌더
			_bullets[i].image->SetSize({ _bullets[i].size.x, _bullets[i].size.y });
			_bullets[i].image->rotationRender(_bullets[i].position.x - camRc.left, _bullets[i].position.y - camRc.top, _bullets[i].angle * 180.0f / Math::PI, Pivot::CENTER);
		}
	}
	if (!_isRender)
		return;
	//총 이미지 렌더
	_image->SetSize({ _size.x, _size.y });

	if (_isLeft)
		_image->rotationRender(_position.x - camRc.left, _position.y - camRc.top, _angle * 180.0f / Math::PI - 180.0f, Pivot::CENTER);
	else
		_image->rotationRender(_position.x - camRc.left, _position.y - camRc.top, _angle * 180.0f / Math::PI, Pivot::CENTER);

}
