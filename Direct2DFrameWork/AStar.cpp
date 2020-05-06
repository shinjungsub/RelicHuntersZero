#include "stdafx.h"
#include "AStar.h"

void AStar::CreateStruct(int idX, int idY)
{
	if (_vTotalList.size() - 1 < idY) return;
	if (_vTotalList[idY].size() - 1 < idX) return;
	Tile* tile = _vTotalList[idY][idX];
	tile->SetParentTile(nullptr);
	tile->SetHaveObject(true);
	_isFind = false;
}

void AStar::DestroyStruct(int idX, int idY)
{
	if (_vTotalList.size() - 1 < idY) return;
	if (_vTotalList[idY].size() - 1 < idX) return;
	Tile* tile = _vTotalList[idY][idX];
	tile->SetHaveObject(false);
	tile->SetCostFromStart(0);
	tile->SetCostToEnd(0);
	tile->SetTotalCost(0);
	tile->SetIsClose(false);
	tile->SetIsOpen(false);
	tile->SetParentTile(nullptr);
	_isFind = false;
}

void AStar::SetTotalList(vector<vector<Tile*>> tiles)
{
	//맨 처음 타일 정보 받아오기
	if (_vTotalList.size() == 0)
	{
		for (int i = 0; i < tiles.size(); i++)
		{
			vector<Tile*> vAstar;
			for (int j = 0; j < tiles[i].size(); j++)
			{
				Tile* aStar = new Tile;
				aStar->Init(tiles[i][j]);
				vAstar.push_back(aStar);
			}
			_vTotalList.push_back(vAstar);
		}
	}
	//구조물이 파괴되었을 때 다시 정보 받아오기 위함
	else
	{
		for (int i = 0; i < tiles.size(); i++)
		{
			for (int j = 0; j < tiles[i].size(); j++)
			{
				_vTotalList[i][j]->Init(tiles[i][j]);
			}
		}
	}
}

void AStar::SetTiles(Vector2 start, Vector2 end, bool isIndex)
{
	_vOpenList.clear();
	_vCenterList.clear();

	if (isIndex)
	{
		_startTile = _vTotalList[start.y][start.x];
		_endTile = _vTotalList[end.y][end.x];
	}
	else
	{
		_startTile = _vTotalList[start.y / TILELENGTH][start.x / TILELENGTH];
		_endTile = _vTotalList[end.y / TILELENGTH][end.x / TILELENGTH];
	}

	for (int i = 0; i < TILENUMY; i++)
	{
		for (int j = 0; j < TILENUMX; j++)
		{
			_vTotalList[i][j]->SetCostFromStart(0);
			_vTotalList[i][j]->SetCostToEnd(0);
			_vTotalList[i][j]->SetTotalCost(0);
			_vTotalList[i][j]->SetIsClose(false);
			_vTotalList[i][j]->SetIsOpen(false);
			_vTotalList[i][j]->SetParentTile(nullptr);
		}
	}

	_isFind = false;
}

vector<Tile*> AStar::AddOpenList(Tile* currentTile)
{
	int startX = currentTile->GetIndex().x - 1;
	int startY = currentTile->GetIndex().y - 1;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (startY + i < 0 || startY + i >= TILENUMY) continue;
			if (startX + j < 0 || startX + j >= TILENUMX) continue;
			Tile* node = _vTotalList[startY + i][startX + j];
			if (node == currentTile) continue;
			if (_vTotalList[node->GetIndex().y][currentTile->GetIndex().x]->GetHaveObject() ||
				_vTotalList[currentTile->GetIndex().y][node->GetIndex().x]->GetHaveObject()) continue;
			if (node == _startTile) continue;
			if (node->GetHaveObject()) continue;
			if (node->GetIsClose()) continue;
			if (node->GetIsOpen())
			{
				if (node->GetParentTile() == nullptr) continue;
				Tile* parentTile = node->GetParentTile();
				Vector2 parentCenter = parentTile->GetCenter();
				Vector2 center = node->GetCenter();
				if (node->GetParentTile()->GetCostFromStart() > currentTile->GetCostFromStart())
				{
					node->SetParentTile(currentTile);
					node->SetCostFromStart(((Math::GetDistance(parentCenter.x, parentCenter.y, center.x, center.y) > TILELENGTH) ? 14 : 10) +
						(parentTile == nullptr ? 0 : parentTile->GetCostFromStart()));
					node->SetTotalCost(node->GetCostToEnd() + node->GetCostFromStart());
				}
				continue;
			}

			node->SetParentTile(currentTile);
			_vOpenList.push_back(node);
		}
	}
	return _vOpenList;
}

void AStar::PathFinder(POINT currentIndex)
{
	if (_isFind) return;
	Tile* currentTile = _vTotalList[currentIndex.y][currentIndex.x];
	if (currentTile == _endTile) return;
	float tempTotalCost = 100000.0f;
	Tile* tempTile = nullptr;

	int openListSize = AddOpenList(currentTile).size();
	for (int i = 0; i < openListSize; i++)
	{
		_vOpenList[i]->SetCostToEnd((abs(_endTile->GetIndex().x - _vOpenList[i]->GetIndex().x) +
			abs(_endTile->GetIndex().y - _vOpenList[i]->GetIndex().y)) * 10);

		Tile* parentTile = _vOpenList[i]->GetParentTile();
		Vector2 parentCenter = parentTile->GetCenter();
		Vector2 center = _vOpenList[i]->GetCenter();
		_vOpenList[i]->SetCostFromStart(((Math::GetDistance(parentCenter.x, parentCenter.y, center.x, center.y) > TILELENGTH) ? 14 : 10) +
			(parentTile == nullptr ? 0 : parentTile->GetCostFromStart()));

		_vOpenList[i]->SetTotalCost(_vOpenList[i]->GetCostToEnd() +
			_vOpenList[i]->GetCostFromStart());

		if (tempTotalCost > _vOpenList[i]->GetTotalCost())
		{
			tempTotalCost = _vOpenList[i]->GetTotalCost();
			tempTile = _vOpenList[i];
		}

		_vOpenList[i]->SetIsOpen(true);
	}

	if (tempTile == _endTile)
	{
		while (tempTile->GetParentTile() != nullptr)
		{
			_vCenterList.push_back(tempTile->GetCenter());
			tempTile = tempTile->GetParentTile();
		}
		_isFind = true;
		return;
	}

	tempTile->SetIsClose(true);
	tempTile->SetIsOpen(false);
	int size = _vOpenList.size();
	for (int i = 0; i < size; i++)
	{
		if (_vOpenList[i] == tempTile)
		{
			_vOpenList.erase(_vOpenList.begin() + i);
			break;
		}
	}

	PathFinder(tempTile->GetIndex());
}
