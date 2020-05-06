#include "stdafx.h"
#include "Camera.h"

void Camera::Update(Vector2 pos)
{
	//��ǥ�� �߽����� ī�޶� �������� �� �� ������ �������� Ȯ�� �� ������ �� ī�޶� ��Ʈ ����
	if (pos.x - (WINSIZEX / 2.0f) < 0)
		pos.x -= (pos.x - (WINSIZEX / 2));
	else if (pos.x + (WINSIZEX / 2.0f) > mapSize.x)
		pos.x -= (pos.x - (mapSize.x - (WINSIZEX / 2.0f)));
	if (pos.y - (WINSIZEY / 2.0f) < 0)
		pos.y -= (pos.y - (WINSIZEY / 2));
	else if (pos.y + (WINSIZEY / 2.0f) > mapSize.y)
		pos.y -= (pos.y - (mapSize.y - (WINSIZEY / 2.0f)));
	cameraRect = Figure::RectMakeCenter(pos, { WINSIZEX, WINSIZEY });
}

RECT Camera::ReviseRect(RECT rc)
{
	rc.left -= cameraRect.left;
	rc.top -= cameraRect.top;
	rc.right -= cameraRect.left;
	rc.bottom -= cameraRect.top;
	return rc;
}
