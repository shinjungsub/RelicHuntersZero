#pragma once
#include "Image.h"

#define BULLETMAX 50

class Gun
{
public:
	//�� ���� enum
	enum GunKinds
	{
		None,
		SingleGun,
		MachineGun,
		ShotGun
	};
	//�Ѿ� ����ü
	struct Bullet
	{
		Vector2 position;	//�Ѿ� ��ġ
		Vector2 size;		//�Ѿ� ������
		RECT rc;			//�Ѿ˰� ������Ʈ�� �浹�� üũ�ϱ� ���� ��Ʈ
		Image* image;		//�Ѿ� �̹���
		Image* hitImage;	//�Ѿ��� �浹���� �� ������ �̹���
		int power;			//�Ѿ� �Ŀ�
		float speed;		//�Ѿ��� ���ư��� ���ǵ�
		float angle;		//�Ѿ��� ���ư��� ����
		bool isFire;		//�� �Ѿ��ΰ�?
		bool isHit;			//�浹�Ͽ��°�
		int imageFrameX;	//�̹��� ������(�浹���� �� ���)
		int imageCount;		//�̹��� ������ ��ȭ �ӵ��� ���� ����
	};
private:
	vector<Bullet> _bullets;	//�Ѿ� 50���� ���� �� ����
	GunKinds _kinds;			//�� ����
	Vector2 _muzzlePos;			//�ѱ� ��ǥ
	Vector2 _position;			//�� ��ġ
	Vector2 _size;				//�� �̹��� ������
	Image* _image;				//�� �̹���
	float _angle;				//���� ����
	float _gunLength;			//�� ���� - �Ѿ� ������ ��ġ�� �˱� ����
	float _shootDelayTime;		//�ѽ�� ������ �ð�
	int _totalBullet;			//�� ������ �ִ� �Ѿ� ����
	int _currentBullet;			//���� źâ�� �ִ� �Ѿ� ����
	int _shootCount;			//�ӽŰ��϶� ������ ����� �ѹ� ������ ���� ���ϴ� ����
	int _power;					//�Ѿ˿� �Ŀ��� �Ѱ��ֱ� ���� ����
	bool _isShoot;				//���� ����� ��°�
	bool _isRender;				//�����ϴ°�?
	bool _isLeft;				//���� ������ �ٶ󺸰� �ִ°�? - �¿� ���� �� �̹����� �޶� ���� ����
	bool _isShootDelay;			//�ѽ�°� ������ �ð� ���ΰ�?
public:
	////////////////////
	// Get �Լ�
	////////////////////
	Vector2 GetMuzzlePos() { return _muzzlePos; }
	Vector2 GetPostion() { return _position; }
	float GetAngle() { return _angle; }
	vector<Bullet> GetBullets() { return _bullets; }
	int GetBulletPower() { return _power; }
	GunKinds GetGunKind() { return _kinds; }

	////////////////////
	// Set �Լ�
	////////////////////
	void SetPosition(Vector2 pos) { _position = pos; }
	void SetIsShoot(bool b) { _isShoot = b; }
	void SetAngle(float angle) { _angle = angle; }
	void SetTotalBullet(int bullet) { _totalBullet = bullet; }
	void SetCurrentBullet(int bullet) { _currentBullet = bullet; }
	void SetIsRender(bool b) { _isRender = b; }
	void SetImage(Image* image) { _image = image; }
	void SetIsLeft(bool b)
	{
		_isLeft = b;
		UpdateGunKinds();
	}
	void SetGunKinds(GunKinds kinds)
	{
		_kinds = kinds;
		UpdateGunKinds();
	}

	//isLeft�� �� ������ ���� �ٲ�� ���µ�(�̹���, �� �Ŀ� ���)
	void UpdateGunKinds()
	{
		string gunName = "";

		switch (_kinds)
		{
		case Gun::GunKinds::None:
			break;
		case Gun::GunKinds::SingleGun:
			gunName = "pistol_0";
			_size = { 62.0f, 26.0f };
			_gunLength = 45.0f;
			_power = 10;
			break;
		case Gun::GunKinds::MachineGun:
			gunName = "machinegun_0";
			_size = { 72.0f, 28.0f };
			_gunLength = 50.0f;
			_power = 8;
			break;
		case Gun::GunKinds::ShotGun:
			gunName = "shotgun_0";
			_size = { 76.0f, 20.0f };
			_gunLength = 52.0f;
			_power = 12;
			break;
		default:
			break;
		}

		if (_isLeft) gunName += "_left";

		_image = IMAGEMANAGER->FindImage(gunName);
	}

	//�Ѿ� �浹 �Լ�
	void BulletCollision(int i);

	//�Ѿ� �߻� �Լ�
	void Shoot();

	//�Ѿ� ������ �Լ�
	void BulletUpdate();

	void Init();
	void Update();
	void Render();

	Gun() {}
	~Gun() {}
};

