#pragma once
#include "GameObject.h"
#include "Gun.h"
#include "HpBar.h"

#define SPEED 5.0f;

class Player : public GameObject
{
private:
	//�÷��̾� ���� enum
	enum class KindOfImage
	{
		Idle,
		Walk,
		Run,
		Dash,
		Die,
		End
	};
	//�÷��̾� �̵����� enum
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
	KindOfImage _kindOfImage;	//�÷��̾� �̹��� ����
	Direction _direction;		//�÷��̾� �̵� ����
	class HpBar* _hpBar;		//HpBar Ŭ����

	float _speed;				//�÷��̾� ���ǵ�

	int _imageFrameX;			//�÷��̾� �̹���������X
	int _imageFrameY;			//�÷��̾� �̹���������Y
	int _imageCount;			//�̹��� ������ �ٲ�� �ӵ�
	int _dashCount;				//�뽬 ī��Ʈ

	bool _isLeft;				//ĳ���Ͱ� ������ �ٶ󺸰� �ִ���?
	bool _isDash;				//�뽬 �ߴ°�
	bool _isRun;				//�ٴ°�
	bool _isMoveUp;				//���� Ű�� �����°�?
	bool _isMoveDown;			//�Ʒ� Ű�� �����°�?
	bool _isMoveLeft;			//���� Ű�� �����°�?
	bool _isMoveRight;			//������ Ű�� �����°�?

	////////////////
	// �� ���� ����
	////////////////
	class Gun* _gun[2];			//�� Ŭ����
	int _currentGun;			//���� ��

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
	//Get �Լ�
	//////////////
	Gun* GetGun(int i) { return _gun[i]; }
	float GetSpeed() { return _speed; }
	int GetHp() { return _hp; }


	//////////////
	//Set �Լ�
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

	//Ű ��Ʈ�ѷ�
	void KeyController(void);
	//�÷��̾� ���¿� ���� �̹��� ������Ʈ ��Ű�� �Լ�
	void UpdatePlayerImage(void);
	//�÷��̾� �̹����� �������� ������Ʈ ��Ű�� �Լ�
	void UpdateImageFrame(void);
	//���� �������� ������ �����̴� �Լ�
	void MovePosition(void);
	//���� Ű�� �������� ���� ����
	void PlayerDirection(void);
};

