#include "stdafx.h"
#include "Tile.h"

void Tile::Init(Tile* tile)
{
	SetCenter(tile->GetCenter());
	_index = tile->GetIndex();
	_haveObject = tile->GetHaveObject();
	_totalCost = 0;
	_costFromStart = 0;
	_costToEnd = 0;
	_isOpen = false;
	_isClose = false;
}

void Tile::Render(RECT camRc)
{
	if (!_haveObject) return;
	if (_hp <= 0) return;
	_image->SetSize({ 148, 129 });
	_image->Render(_center.x - camRc.left, _center.y - 15 - camRc.top);
}
