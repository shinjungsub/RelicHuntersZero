#pragma once
#include "Tile.h"

class MapTool : public GameNode
{
private:
	//=======================================
	// 맵에 따라 다른 타일들
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

	//타일 save
	void save();
	//타일 load
	void load();

	MapTool() {}
	~MapTool() {}
};
