#pragma once
#include "Tile.h"
#include "Duck.h"
#include "Player.h"
class Stage : public GameNode
{
private:
	Player* _player;
	vector<Duck*> _duck;
	vector<vector<Tile*>> _tiles;
	int _data[TILENUMY][TILENUMX];	// 맵툴로 만든 맵에 대한 정보를 받아오기 위한 배열(물체가 존재 하는지 안하는지만 체크)
public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;

	void TileRender(POINT start, POINT end, RECT camRc);

	void Collision();
	void TileAndBulletCollision(int tileX, int tileY, Gun* gun);
	void ObjectCollision(GameObject* gameObject);

	Stage() {}
	~Stage() {}
};

