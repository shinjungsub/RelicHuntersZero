#pragma once
#include "Image.h"

#define BULLETMAX 50

class Gun
{
public:
	//총 종류 enum
	enum GunKinds
	{
		None,
		SingleGun,
		MachineGun,
		ShotGun
	};
	//총알 구조체
	struct Bullet
	{
		Vector2 position;	//총알 위치
		Vector2 size;		//총알 사이즈
		RECT rc;			//총알과 오브젝트의 충돌을 체크하기 위한 렉트
		Image* image;		//총알 이미지
		Image* hitImage;	//총알이 충돌했을 때 렌더할 이미지
		int power;			//총알 파워
		float speed;		//총알이 날아가는 스피드
		float angle;		//총알이 날아가는 각도
		bool isFire;		//쏜 총알인가?
		bool isHit;			//충돌하였는가
		int imageFrameX;	//이미지 프레임(충돌했을 시 사용)
		int imageCount;		//이미지 프레임 변화 속도를 위한 변수
	};
private:
	vector<Bullet> _bullets;	//총알 50개로 돌려 쓸 예정
	GunKinds _kinds;			//총 종류
	Vector2 _muzzlePos;			//총구 좌표
	Vector2 _position;			//총 위치
	Vector2 _size;				//총 이미지 사이즈
	Image* _image;				//총 이미지
	float _angle;				//총의 각도
	float _gunLength;			//총 길이 - 총알 나가는 위치를 알기 위함
	float _shootDelayTime;		//총쏘는 딜레이 시간
	int _totalBullet;			//총 가지고 있는 총알 개수
	int _currentBullet;			//현재 탄창에 있는 총알 개수
	int _shootCount;			//머신건일때 프레임 몇번에 한번 나가게 할지 정하는 변수
	int _power;					//총알에 파워를 넘겨주기 위한 변수
	bool _isShoot;				//쏘라고 명령을 줬는가
	bool _isRender;				//렌더하는가?
	bool _isLeft;				//총이 왼쪽을 바라보고 있는가? - 좌우 반전 된 이미지가 달라서 쓰는 변수
	bool _isShootDelay;			//총쏘는거 딜레이 시간 중인가?
public:
	////////////////////
	// Get 함수
	////////////////////
	Vector2 GetMuzzlePos() { return _muzzlePos; }
	Vector2 GetPostion() { return _position; }
	float GetAngle() { return _angle; }
	vector<Bullet> GetBullets() { return _bullets; }
	int GetBulletPower() { return _power; }
	GunKinds GetGunKind() { return _kinds; }

	////////////////////
	// Set 함수
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

	//isLeft나 총 종류에 따라 바뀌는 상태들(이미지, 총 파워 등등)
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

	//총알 충돌 함수
	void BulletCollision(int i);

	//총알 발사 함수
	void Shoot();

	//총알 움직임 함수
	void BulletUpdate();

	void Init();
	void Update();
	void Render();

	Gun() {}
	~Gun() {}
};

