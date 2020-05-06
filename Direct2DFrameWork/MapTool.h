#pragma once
#include "Tile.h"

class MapTool : public GameNode
{
private:
	//=======================================
	// �ʿ� ���� �ٸ� Ÿ�ϵ�
	//=======================================
	vector<vector<Tile*>> _tiles;
	Vector2 _camCenter;
	int _data[TILENUMY][TILENUMX];

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;

	void CameraMove();

	//Ÿ�� save
	void save();
	//Ÿ�� load
	void load();

	MapTool() {}
	~MapTool() {}
};
