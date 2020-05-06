#pragma once
#include "Enemy.h"

#define SHOOTINGDISTANCE 400
#define DISCOVERDISTANCE 600
class Duck final : public Enemy
{
private:

	class Gun* _gun;	//총

	vector<vector<Tile*>> _tiles;	//타일 정보
	POINT _playerIndex;				//플레이어 인덱스
	GameObject* _player;			//플레이어

	POINT _endTileIndex;			//마지막타일 인덱스
	POINT _endTileIndexPrev;		//마지막타일 전 인덱스

	float _shootDelayTime;			//총쏘는 것을 딜레이 시키는 변수
	bool _isShoot;					//자동으로 쏠 때 사용할 변수
	bool _isReSearch;				//길을 다시 찾아야 하는가
	bool _isAngleCheck;				//총알에 맞았을 때 총을 반대로 뒤집어 주기 위해 앵글을 체크했는가
public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;

	////////////////////
	// Set 함수
	////////////////////
	void SetHp(int hp)
	{
		_hp = hp;
		if (_hp <= 0)
		{
			SetKindOfImage(KindOfImage::Death);
		}
	}
	void SetPlayer(GameObject* player) { _player = player; }
	void SetIsReSearch(bool b) { _isReSearch = b; }

	////////////////////
	// Get 함수
	////////////////////
	int GetHp() { return _hp; }
	Gun* GetGun() { return _gun; }
	AStar* GetAStar() { return _aStar; }

	//A*를 사용하거나 플레이어와의 사이에 물체가 있는지 확인하기 위해 물체가 파괴된 경우를 확인하기 위한 함수
	void ReceiveChangeTileInfo(int idX, int idY, bool isDestroy);
	//A*를 위해 타일 정보 받아오기
	void AStarTileSetting(vector<vector<Tile*>> tiles)
	{
		_tiles = tiles;
		_aStar->SetTotalList(tiles);
		_isReSearch = true;
	}
	//움직임에 관련된 함수
	void BeHavier();
	//가만히 있는 상태
	void Stay();
	//자동으로 움직이는 상태
	void RandomMove();
	//맞고 뒤로 밀리는 상태
	void NuckBack(float angle);
	//총알에 맞았을 때 맞은 방향과 총알의 파워를 받아오기 위한 함수
	void HitByBullet(float angle, int power);
	//대쉬 상태
	void Dash();
	//플레이어 추적 상태
	void Advance();
	//공격 상태
	void Shoot();
	//상태에 따른 이미지 업데이트
	void UpdateImage();
	//플레이어의 인덱스가 변했는지 확인하기 위한 함수
	bool ReSearch();
	//자동 쏘기
	void AutoShoot();
	//플레이어와의 사이에 물체가 있는지 확인하기 위한 함수
	bool CheckStruct(float startX, float startY, float endX, float endY);
	//A*를 현재 사용하고 있는가
	bool UseAstar()
	{
		if (_state == State::RandomMove || _state == State::Advance)
			return true;
		else
			return false;
	}

	Duck() {}
	Duck(string name, Vector2 pos);
	~Duck() {}
};

