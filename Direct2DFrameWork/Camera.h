#pragma once
#include "SingletonBase.h"
class Camera : public SingletonBase<Camera>
{
private:
	float width;
	float height;
	RECT cameraRect;
	Vector2 position;
	Vector2 mapSize;
public:
	void SetMapSize(Vector2 size) { mapSize = size; }
	void Update(Vector2 pos);
	RECT GetCamera(void) { return cameraRect; }
	RECT ReviseRect(RECT rc);
};

