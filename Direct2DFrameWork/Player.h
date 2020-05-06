#pragma once
#include "GameObject.h"
#include "Gun.h"
#include "HpBar.h"

#define SPEED 5.0f;

class Player : public GameObject
{
private:
	//플레이어 상태 enum
	enum class KindOfImage
	{
		Idle,
		Walk,
		Run,
		Dash,
		Die,
		End
	};
	//플레이어 이동방향 enum
	enum class Direction
	{
		None,
		Left,
		LeftUp,
		Up,
		UpRight,
		Right,
		RightDown,
		Down,
		DownLeft,
	};
	KindOfImage _kindOfImage;	//플레이어 이미지 종류
	Direction _direction;		//플레이어 이동 방향
	class HpBar* _hpBar;		//HpBar 클래스

	float _speed;				//플레이어 스피드

	int _imageFrameX;			//플레이어 이미지프레임X
	int _imageFrameY;			//플레이어 이미지프레임Y
	int _imageCount;			//이미지 프레임 바뀌는 속도
	int _dashCount;				//대쉬 카운트

	bool _isLeft;				//캐릭터가 왼쪽을 바라보고 있는지?
	bool _isDash;				//대쉬 했는가
	bool _isRun;				//뛰는가
	bool _isMoveUp;				//위에 키를 눌렀는가?
	bool _isMoveDown;			//아래 키를 눌렀는가?
	bool _isMoveLeft;			//왼쪽 키를 눌렀는가?
	bool _isMoveRight;			//오른쪽 키를 눌렀는가?

	////////////////
	// 총 관련 변수
	////////////////
	class Gun* _gun[2];			//총 클래스
	int _currentGun;			//현재 총

public:


public:
	Player() {}
	Player(Vector2 pos) : GameObject("player", pos, { pos.x + 0, pos.y + 43 }, { pos.x + 0, pos.y + 10 }, { 38, 10 }, { 36, 54 }, { 114, 94 }, Pivot::CENTER)
	{
		_gun[0] = new Gun;
		_gun[1] = new Gun;
		_hp = 1000;
		_hpBar = new HpBar(_hp);
	}
	~Player() {}

	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;

	//////////////
	//Get 함수
	//////////////
	Gun* GetGun(int i) { return _gun[i]; }
	float GetSpeed() { return _speed; }
	int GetHp() { return _hp; }


	//////////////
	//Set 함수
	//////////////
	void SetHp(int hp)
	{
		_hp = hp;
		if (_hp <= 0)
		{
			_kindOfImage = KindOfImage::Die;
		}
	}
	void SetSpeed(float speed) { _speed = speed; }
	void SetKindOfImage(KindOfImage kindOfImage) { _kindOfImage = kindOfImage; }

	//키 컨트롤러
	void KeyController(void);
	//플레이어 상태에 따라 이미지 업데이트 시키는 함수
	void UpdatePlayerImage(void);
	//플레이어 이미지의 프레임을 업데이트 시키는 함수
	void UpdateImageFrame(void);
	//현재 방향으로 포지션 움직이는 함수
	void MovePosition(void);
	//누른 키를 바탕으로 방향 설정
	void PlayerDirection(void);
};

