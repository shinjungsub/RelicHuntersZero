#pragma once
#include "Image.h"
#define TILENUMX 25
#define TILENUMY 15
#define TILELENGTH 64

class Tile
{
private:
	Tile* _parentTile;
	Image* _image;

	RECT _rc;
	Vector2 _center;

	POINT _index;
	int _imageFrameX;
	int _imageFrameY;

	int _totalCost;
	int _costFromStart;
	int _costToEnd;

	int _hp;		//오브젝트를 가지고 있는 애들만 사용

	bool _isOpen;
	bool _isClose;
	bool _haveObject;
public:
	void Init(Tile*);

	Tile* GetParentTile() { return _parentTile; }
	RECT GetRect() { return _rc; }
	Vector2 GetCenter() { return _center; }
	POINT GetIndex() { return _index; }
	int GetTotalCost() { return _totalCost; }
	int GetCostFromStart() { return _costFromStart; }
	int GetCostToEnd() { return _costToEnd; }
	int GetHp() { return _hp; }
	bool GetIsOpen() { return _isOpen; }
	bool GetIsClose() { return _isClose; }
	bool GetHaveObject() { return _haveObject; }

	void SetParentTile(Tile* parentTile) { _parentTile = parentTile; }
	void SetCenter(Vector2 center)
	{
		_center = center;
		_rc = Figure::RectMakeCenter(_center, { TILELENGTH, TILELENGTH });
	}
	void SetIndex(POINT index) { _index = index; }
	void SetTotalCost(int totalCost) { _totalCost = totalCost; }
	void SetCostFromStart(int costFromStart) { _costFromStart = costFromStart; }
	void SetCostToEnd(int costToEnd) { _costToEnd = costToEnd; }
	void SetHp(int hp)
	{
		_hp = hp;
		if (_hp <= 0)
			_haveObject = false;
		else if (_hp < 33)
			_image = IMAGEMANAGER->FindImage("struct3");
		else if (_hp < 66)
			_image = IMAGEMANAGER->FindImage("struct2");
		else
			_image = IMAGEMANAGER->FindImage("struct1");
	}
	void SetIsOpen(bool isOpen) { _isOpen = isOpen; }
	void SetIsClose(bool isClose) { _isClose = isClose; }
	void SetHaveObject(bool haveObject) { _haveObject = haveObject; }

	void Render(RECT camRc);

	Tile() : _center({ 0, 0 }), _index({ 0, 0 }), _totalCost(0), _costFromStart(0), _costToEnd(0), _isOpen(false) {}
	Tile(Vector2 center, POINT index, bool haveObject) : _index(index), _totalCost(0), _costFromStart(0), _costToEnd(0), _isOpen(false), _isClose(false), _haveObject(haveObject)
	{
		SetCenter(center);
		if (haveObject) _hp = 100;
		_image = IMAGEMANAGER->FindImage("struct1");
	}
	~Tile() {}
};