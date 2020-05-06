#include "stdafx.h"
#include "Stage.h"

void Stage::Init()
{
	//createFile을 사용하여 맵 정보 받아오기
	HANDLE file;
	DWORD read;
	file = CreateFile(L"./game.map", GENERIC_READ, 0, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, _data, sizeof(Tile*) * TILENUMX * TILENUMY, &read, NULL);
	CloseHandle(file);

	OBJECTMANAGER->AddObject(ObjectType::Object, new Player(Vector2(170, 150)));
	OBJECTMANAGER->AddObject(ObjectType::Object, new Duck("duck1", Vector2(1020, 150)));	//1020, 150
	OBJECTMANAGER->AddObject(ObjectType::Object, new Duck("duck2", Vector2(1320, 150)));
	OBJECTMANAGER->AddObject(ObjectType::Object, new Duck("duck3", Vector2(1220, 550)));

	_player = (Player*)OBJECTMANAGER->FindObject(ObjectType::Object, "player");
	for (int i = 0; i < 3; i++)
		_duck.push_back((Duck*)OBJECTMANAGER->FindObject(ObjectType::Object, "duck" + to_string(i + 1)));

	OBJECTMANAGER->Init();
	CAMERA->SetMapSize({ TILELENGTH * TILENUMX, TILELENGTH * TILENUMY });
	CAMERA->Update(_player->GetPosition());
	for (int i = 0; i < TILENUMY; i++)
	{
		vector<Tile*> vTile;
		for (int j = 0; j < TILENUMX; j++)
		{
			Tile* tile = new Tile({ j * TILELENGTH + TILELENGTH / 2.0f, i * TILELENGTH + TILELENGTH / 2.0f }, { j, i }, (bool)_data[i][j]);
			vTile.push_back(tile);
		}
		_tiles.push_back(vTile);
	}
	for(int i = 0; i < _duck.size(); i++)
		_duck[i]->AStarTileSetting(_tiles);
}

void Stage::Release()
{
	OBJECTMANAGER->Release();
}

void Stage::Update()
{
	Collision();
	_player->Update();
	ObjectCollision(_player);
	for (int i = 0; i < _duck.size(); i++)
		_duck[i]->Update();
	for (int i = 0; i < _duck.size(); i++)
		if(!_duck[i]->GetIsDie())
			ObjectCollision(_duck[i]);

	CAMERA->Update(_player->GetPosition());
}

void Stage::Render()
{
	RECT camRc = CAMERA->GetCamera();
	POINT camLeftTopIndex = { camRc.left / TILELENGTH, camRc.top / TILELENGTH };
	POINT camRightBottomIndex = { camRc.right / TILELENGTH, (camRc.bottom / TILELENGTH) + 1 };
	Figure::NonOverIdx(camLeftTopIndex, { 0, 0 }, { TILENUMX - 1, TILENUMY - 1 });
	Figure::NonOverIdx(camRightBottomIndex, { 0, 0 }, { TILENUMX - 1, TILENUMY - 1 });
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

	//타일과 오브젝트간에 위치에 따른 렌더 순서를 정하기 위해
	//게임오브젝트들을 모두 위치에 따라 정렬을 하고
	//타일과 오브젝트를 순서에 맞게 렌더해줌
	vector<GameObject*> vGameObject = OBJECTMANAGER->GetObjectList(ObjectType::Object);
	for (int i = 0; i < vGameObject.size(); i++)
	{
		int index = 0;
		int y = 1000;
		GameObject* temp;
		for (int j = i; j < vGameObject.size(); j++)
		{
			if (vGameObject[j]->GetMainRectPos().y < y)
			{
				y = vGameObject[j]->GetMainRectPos().y;
				index = j;
			}
		}
		if (i != index)
		{
			temp = vGameObject[index];
			vGameObject[index] = vGameObject[i];
			vGameObject[i] = temp;
		}
	}

	int size = vGameObject.size();
	for (int i = 0; i < size; i++)
	{
		POINT start;
		POINT end;
		if (i == 0)
		{
			start = camLeftTopIndex;
			end = { camRightBottomIndex.x, vGameObject[i]->GetIndex().y - 1 };
		}
		else
		{
			start = { camLeftTopIndex.x, vGameObject[i - 1]->GetIndex().y };
			end = { camRightBottomIndex.x, vGameObject[i]->GetIndex().y - 1 };
		}
		TileRender(start, end, camRc);
		if (i == size - 1)
		{
			vGameObject[i]->Render();
			start = { camLeftTopIndex.x, vGameObject[i]->GetIndex().y };
			end = camRightBottomIndex;
			TileRender(start, end, camRc);
		}
		else
		{
			while (true)
			{
				vGameObject[i]->Render();
				if (i != size - 1)
				{
					if (vGameObject[i]->GetIndex().y == vGameObject[i + 1]->GetIndex().y)
					{
						i++;
						continue;
					}
					else
					{
						break;
					}
				}
				else
				{
					start = { camLeftTopIndex.x, vGameObject[i]->GetIndex().y };
					end = camRightBottomIndex;
					TileRender(start, end, camRc);
					break;
				}
			}
		}
	}

	//마우스를 크로스헤어로 바꿔줌
	Vector2 pos = { _ptMouse.x, _ptMouse.y };
	if (KEYMANAGER->IsStayKeyDown(VK_LBUTTON))
	{
		IMAGEMANAGER->FindImage("crosshair_2")->Render(pos.x, pos.y);
	}
	else
	{
		IMAGEMANAGER->FindImage("crosshair_0")->Render(pos.x, pos.y);
	}
}

void Stage::TileRender(POINT start, POINT end, RECT camRc)
{
	for (int i = start.y; i <= end.y; i++)
	{
		for (int j = start.x; j <= end.x; j++)
		{
			if (_tiles[i][j]->GetHaveObject())
			{
				_tiles[i][j]->Render(camRc);
			}
		}
	}
}

//총알과 게임오브젝트, 오브젝트와의 충돌 처리
void Stage::Collision()
{
	for (int k = 0; k < _duck.size(); k++)
	{
		if (_duck[k]->GetActive())
		{
			vector<Gun::Bullet> bullets = _player->GetGun(0)->GetBullets();
			int bulletSize = bullets.size();
			
			for (int i = 0; i < bulletSize; i++)
			{
				if (!bullets[i].isFire) continue;
				if (Figure::IsCollision(_duck[k]->GetHitRect(), bullets[i].rc))
				{
					_player->GetGun(0)->BulletCollision(i);
					_duck[k]->HitByBullet(bullets[i].angle, bullets[i].power);
				}
			}
			bullets = _player->GetGun(1)->GetBullets();
			bulletSize = bullets.size();
			for (int i = 0; i < bulletSize; i++)
			{
				if (!bullets[i].isFire) continue;
				if (Figure::IsCollision(_duck[k]->GetHitRect(), bullets[i].rc))
				{
					_player->GetGun(1)->BulletCollision(i);
					_duck[k]->HitByBullet(bullets[i].angle, bullets[i].power);
				}
			}
			bullets = _duck[k]->GetGun()->GetBullets();
			bulletSize = bullets.size();
			for (int i = 0; i < bulletSize; i++)
			{
				if (!bullets[i].isFire) continue;
				if (Figure::IsCollision(_player->GetHitRect(), bullets[i].rc))
				{
					_duck[k]->GetGun()->BulletCollision(i);
					_player->SetHp(_player->GetHp() - bullets[i].power);
				}
			}
		}
		
	}
	for (int i = 0; i < TILENUMY; i++)
	{
		for (int j = 0; j < TILENUMX; j++)
		{
			if (!_tiles[i][j]->GetHaveObject()) continue;
			TileAndBulletCollision(i, j, _player->GetGun(0));
			TileAndBulletCollision(i, j, _player->GetGun(1));
			for (int k = 0; k < _duck.size(); k++)
			{
				TileAndBulletCollision(i, j, _duck[k]->GetGun());

				if (!_tiles[i][j]->GetHaveObject())
				{
					for (int n = 0; n < _duck.size(); n++)
						_duck[n]->ReceiveChangeTileInfo(j, i, true);
				}
			}
		}
	}
}

//타일과 총알의 충돌 처리
void Stage::TileAndBulletCollision(int tileX, int tileY, Gun* gun)
{
	vector<Gun::Bullet> bullets = gun->GetBullets();
	int bulletSize = bullets.size();
	for (int m = 0; m < bulletSize; m++)
	{
		if (!bullets[m].isFire) continue;
		if (Figure::IsCollision(_tiles[tileX][tileY]->GetRect(), bullets[m].rc))
		{
			gun->BulletCollision(m);
			_tiles[tileX][tileY]->SetHp(_tiles[tileX][tileY]->GetHp() - bullets[m].power);
		}
	}
}

// 게임오브젝트와 타일간 충돌 처리
void Stage::ObjectCollision(GameObject* gameObject)
{
	POINT objectIndex = gameObject->GetIndex();
	RECT objectRc = gameObject->GetMainRect();
	for (int i = objectIndex.y - 1; i <= objectIndex.y + 1; i++)
	{
		for (int j = objectIndex.x - 1; j <= objectIndex.x + 1; j++)
		{
			if (!_tiles[i][j]->GetHaveObject()) continue;
			if (Figure::IsCollision(objectRc, _tiles[i][j]->GetRect()))
			{
				POINT move = Figure::IsCollisionReturnPoint(objectRc, _tiles[i][j]->GetRect());
				gameObject->SetPosition({ gameObject->GetPosition().x + move.x, gameObject->GetPosition().y + move.y });
				break;
			}
		}
	}
}
