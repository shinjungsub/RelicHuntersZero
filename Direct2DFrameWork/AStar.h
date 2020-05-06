#pragma once
#include "Tile.h"
class AStar
{
private:
	vector<vector<Tile*>> _vTotalList;
	vector<Tile*> _vOpenList;
	vector<Vector2> _vCenterList;

	Tile* _startTile;
	Tile* _endTile;

	bool _isFind;
public:
	AStar() {}
	~AStar()
	{
		_vOpenList.clear();
		for (int i = 0; i < _vTotalList.size(); i++)
			for (int j = 0; j < _vTotalList[i].size(); j++)
				SAFE_DELETE(_vTotalList[i][j]);
		_vTotalList.clear();
	}

	void CreateStruct(int idX, int idY);
	void DestroyStruct(int idX, int idY);
	bool GetIsFind() { return _isFind; }
	void SetTotalList(vector<vector<Tile*>> tiles);
	void SetTiles(Vector2 start, Vector2 end, bool isIndex);
	vector<Tile*> AddOpenList(Tile* currentTile);
	void PathFinder(POINT currentIndex);
	vector<Vector2> GetCenterList() { return _vCenterList; }
	Vector2 GetNextTargetCenter()
	{
		if (_vCenterList.size() == 0)
		{
			_isFind = false;
			return NULL;
		}
		Vector2 center = _vCenterList[_vCenterList.size() - 1];
		_vCenterList.pop_back();
		return center;
	}
};

