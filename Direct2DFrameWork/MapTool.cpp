#include "stdafx.h"
#include "MapTool.h"


void MapTool::Init()
{
	_camCenter = { WINSIZEX / 2.0f, WINSIZEY / 2.0f };
	CAMERA->SetMapSize({ TILELENGTH * TILENUMX, TILELENGTH * TILENUMY });
	CAMERA->Update(_camCenter);
	for (int i = 0; i < TILENUMY; i++)
	{
		vector<Tile*> vTile;
		for (int j = 0; j < TILENUMX; j++)
		{
			Tile* tile = new Tile({ (TILELENGTH / 2) + (TILELENGTH * j), (TILELENGTH / 2) + (TILELENGTH * i) }, { j, i }, false);
			//tile->SetCenter({ (TILELENGTH / 2) + (TILELENGTH * j), (TILELENGTH / 2) + (TILELENGTH * i) });
			tile->SetCostFromStart(0);
			tile->SetCostToEnd(0);
			tile->SetTotalCost(0);
			tile->SetHp(0);
			//tile->SetIndex({ j, i });
			tile->SetParentTile(nullptr);
			//tile->SetHaveObject(false);
			tile->SetIsOpen(false);
			tile->SetIsClose(false);
			vTile.push_back(tile);
		}
		_tiles.push_back(vTile);
	}
}

void MapTool::Release()
{
	for (int i = 0; i < TILENUMY; i++)
	{
		for (int j = 0; j < TILENUMX; j++)
		{
			SAFE_DELETE(_tiles[i][j]);
		}
	}
}

void MapTool::Update()
{
	CameraMove();
	RECT camRc = CAMERA->GetCamera();
	POINT mouseIndex = { (_ptMouse.x + camRc.left) / TILELENGTH , (_ptMouse.y + camRc.top) / TILELENGTH };
	Figure::NonOverIdx(mouseIndex, { 0, 0 }, { TILENUMX - 1, TILENUMY - 1 });
	if (KEYMANAGER->IsStayKeyDown(MK_LBUTTON))
	{
		_tiles[mouseIndex.y][mouseIndex.x]->SetHaveObject(true);
		_tiles[mouseIndex.y][mouseIndex.x]->SetHp(100);
	}
	else if (KEYMANAGER->IsStayKeyDown(MK_RBUTTON))
	{
		_tiles[mouseIndex.y][mouseIndex.x]->SetHaveObject(false);
		_tiles[mouseIndex.y][mouseIndex.x]->SetHp(0);
	}

	if (KEYMANAGER->IsStayKeyDown(VK_CONTROL))
	{
		if (KEYMANAGER->IsOnceKeyDown('S'))
		{
			save();
		}
		if (KEYMANAGER->IsOnceKeyDown('L'))
		{
			load();
		}
	}
}

void MapTool::Render()
{
	RECT camRc = CAMERA->GetCamera();
	POINT camLeftTopIndex = { camRc.left / TILELENGTH, camRc.top / TILELENGTH };
	POINT camRightBottomIndex = { camRc.right / TILELENGTH, camRc.bottom / TILELENGTH };
	Figure::NonOverIdx(camLeftTopIndex, { 0, 0 }, { TILENUMX - 1, TILENUMY - 1});
	Figure::NonOverIdx(camRightBottomIndex, { 0, 0 }, { TILENUMX - 1, TILENUMY - 1});
	for (int i = camLeftTopIndex.y; i <= camRightBottomIndex.y; i++)
	{
		for (int j = camLeftTopIndex.x; j <= camRightBottomIndex.x; j++)
		{
			if ((i + j) % 2 == 0)
			{
				IMAGEMANAGER->FindImage("tile_1")->Render(_tiles[i][j]->GetCenter().x - camRc.left, _tiles[i][j]->GetCenter().y - camRc.top);
			}
			else
			{
				IMAGEMANAGER->FindImage("tile_2")->Render(_tiles[i][j]->GetCenter().x - camRc.left, _tiles[i][j]->GetCenter().y - camRc.top);
			}
		}
	}
	for (int i = camLeftTopIndex.y; i <= camRightBottomIndex.y; i++)
	{
		for (int j = camLeftTopIndex.x; j <= camRightBottomIndex.x; j++)
		{
			if (_tiles[i][j]->GetHaveObject())
			{
				_tiles[i][j]->Render(camRc);
				/*IMAGEMANAGER->FindImage("struct1")->SetSize({ 148, 120 });*/
				//IMAGEMANAGER->FindImage("struct1")->Render(_tiles[i][j]->GetCenter().x - camRc.left, _tiles[i][j]->GetCenter().y - camRc.top);
			}
		}
	}
}

void MapTool::CameraMove()
{
	RECT camRc = CAMERA->GetCamera();
	if (_ptMouse.x < 10)
	{
		_camCenter = { _camCenter.x - 5, _camCenter.y };
	}
	else if (_ptMouse.y < 10)
	{
		_camCenter = { _camCenter.x, _camCenter.y - 5 };
	}
	else if (_ptMouse.x > WINSIZEX - 10)
	{
		_camCenter = { _camCenter.x + 5, _camCenter.y };
	}
	else if (_ptMouse.y > WINSIZEY - 10)
	{
		_camCenter = { _camCenter.x, _camCenter.y + 5 };
	}
	if (_camCenter.x < WINSIZEX / 2.0f)
		_camCenter.x = WINSIZEX / 2.0f;
	if (_camCenter.x > (TILELENGTH * TILENUMX) - (WINSIZEX / 2.0f))
		_camCenter.x = (TILELENGTH * TILENUMX) - (WINSIZEX / 2.0f);
	if (_camCenter.y < WINSIZEY / 2.0f)
		_camCenter.y = WINSIZEY / 2.0f;
	if (_camCenter.y > (TILELENGTH * TILENUMY) - (WINSIZEY / 2.0f))
		_camCenter.y = (TILELENGTH * TILENUMY) - (WINSIZEY / 2.0f);
	CAMERA->Update(_camCenter);
}


void MapTool::save()
{
	for (int i = 0; i < TILENUMY; i++)
	{
		for (int j = 0; j < TILENUMX; j++)
		{
			_data[i][j] = _tiles[i][j]->GetHaveObject();
		}
	}
	HANDLE file;
	DWORD write;

	file = CreateFile(L"./game.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(file, _data, sizeof(int) * TILENUMX * TILENUMY, &write, NULL);

	CloseHandle(file);
}

void MapTool::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile(L"./game.map", GENERIC_READ, 0, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, _data, sizeof(Tile*) * TILENUMX * TILENUMY, &read, NULL);

	CloseHandle(file);
	for (int i = 0; i < TILENUMY; i++)
	{
		for (int j = 0; j < TILENUMX; j++)
		{
			_tiles[i][j]->SetHaveObject(_data[i][j] == 0 ? false : true);
		}
	}
}

