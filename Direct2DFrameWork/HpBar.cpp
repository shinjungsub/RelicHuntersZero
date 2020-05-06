#include "stdafx.h"
#include "HpBar.h"

//HpBar ���� �Լ�
void HpBar::Render(Vector2 pos, int currentHp)
{
	//����� ��� ������� ����
	//������ ���� Hp�� ������ŭ ����
	_rc = Figure::RectMakeCenter(pos, _backgroundSize);
	float percent = (float)currentHp / (float)_maxHp;
	_background->SetSize({ _backgroundSize.x, _backgroundSize.y });
	_background->Render(_rc.left, _rc.top, Pivot::LEFT_TOP);
	_foreground->SetSize({ (float)_backgroundSize.x * percent, _backgroundSize.y });
	_foreground->Render(_rc.left, _rc.top, Pivot::LEFT_TOP);
}
