#pragma once
#include "GameObject.h"
#include "HpBar.h"
#include "AStar.h"

class Enemy : public GameObject
{
protected:
	enum class KindOfImage
	{
		Idle,
		Walk,
		Hit,
		Death,
		Dash
	};
	enum class State
	{
		Stay,			//������ �ִ� ����
		RandomMove,		//�����ϰ� ������
		Hit,			//���� ����
		Dash,			//�ѽ�� ���� �Ѿ� ���ϱ�� �뽬
		Advance,		//�÷��̾����� �ٰ�����
		Attack			//����
	};
	KindOfImage _kindOfImage;	//���¿� ���� �̹��� ����
	State _state;				//����
	State _statePrev;			//�� ����
	HpBar* _hpBar;				//HpBar Ŭ����
	AStar* _aStar;				//A* Ŭ����
	Vector2 _targetCenter;		//A*�� ����Ͽ� �̵��� �� �������� �̵��� Ÿ���� ����
	int _imageFrameX;			//�̹��� ������ X
	int _imageCount;			//�̹��� �������� ��ȭ�� �ӵ��� ���� ����
	float _hitAngle;			//�¾��� �� �з����� ����
	float _angle;				//�̵��ϴ� ����
	float _speed;				//�̵� �ӵ�
	bool _isDiscover;			//�÷��̾ �߰��� �����ΰ�
	bool _isLeft;				//�������� ����������
public:
	Enemy();
	Enemy(string name, Vector2 pos, Vector2 mainPos, Vector2 hitPos, Vector2 mainSize, Vector2 hitSize, Vector2 imageSize);
	virtual ~Enemy();

	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;

	////////////////////
	// Get �Լ�
	////////////////////
	float GetAngle() { return _angle; }
	float GetSpeed() { return _speed; }
	bool GetIsDie() { return (_kindOfImage == KindOfImage::Death ? true : false); }
	bool GetIsDiscover() { return _isDiscover; }
	bool GetIsLeft() { return _isLeft; }

	////////////////////
	// Set �Լ�
	////////////////////
	void SetTargetCenter(Vector2 center) { _targetCenter = center; }
	void SetAngle(float angle) { _angle = angle; }
	void SetSpeed(float speed) { _speed = speed; }
	void SetKindOfImage(KindOfImage kindOfImage)
	{
		if (_kindOfImage == KindOfImage::Hit || _kindOfImage == KindOfImage::Death || _kindOfImage == KindOfImage::Dash)
			return;
		if (_kindOfImage == kindOfImage)
			return;
		/*if (_kindOfImage == KindOfImage::Idle && (kindOfImage == KindOfImage::Hit || kindOfImage == KindOfImage::Dash))
			return;*/
		_kindOfImage = kindOfImage;
		_imageFrameX = 0;
	}
	void SetState(State state) { _state = state; }
	void SetIsDiscover(bool b) { _isDiscover = b; }
	void SetIsLeft(bool b) { _isLeft = b; }

	//�̹��� ������ ������Ʈ�ϴ� �Լ�
	void UpdateImageFrame();
};

