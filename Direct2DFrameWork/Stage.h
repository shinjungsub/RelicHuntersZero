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
	int _data[TILENUMY][TILENUMX];	// ������ ���� �ʿ� ���� ������ �޾ƿ��� ���� �迭(��ü�� ���� �ϴ��� ���ϴ����� üũ)
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

