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
		Stay,			//가만히 있는 상태
		RandomMove,		//랜덤하게 움직임
		Hit,			//맞은 상태
		Dash,			//총쏘는 도중 총알 피하기용 대쉬
		Advance,		//플레이어한테 다가가기
		Attack			//공격
	};
	KindOfImage _kindOfImage;	//상태에 따른 이미지 종류
	State _state;				//상태
	State _statePrev;			//전 상태
	HpBar* _hpBar;				//HpBar 클래스
	AStar* _aStar;				//A* 클래스
	Vector2 _targetCenter;		//A*를 사용하여 이동할 때 다음으로 이동할 타일의 센터
	int _imageFrameX;			//이미지 프레임 X
	int _imageCount;			//이미지 프레임의 변화의 속도를 위한 변수
	float _hitAngle;			//맞았을 때 밀려나는 방향
	float _angle;				//이동하는 방향
	float _speed;				//이동 속도
	bool _isDiscover;			//플레이어를 발견한 상태인가
	bool _isLeft;				//왼쪽인지 오른쪽인지
public:
	Enemy();
	Enemy(string name, Vector2 pos, Vector2 mainPos, Vector2 hitPos, Vector2 mainSize, Vector2 hitSize, Vector2 imageSize);
	virtual ~Enemy();

	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;

	////////////////////
	// Get 함수
	////////////////////
	float GetAngle() { return _angle; }
	float GetSpeed() { return _speed; }
	bool GetIsDie() { return (_kindOfImage == KindOfImage::Death ? true : false); }
	bool GetIsDiscover() { return _isDiscover; }
	bool GetIsLeft() { return _isLeft; }

	////////////////////
	// Set 함수
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

	//이미지 프레임 업데이트하는 함수
	void UpdateImageFrame();
};

