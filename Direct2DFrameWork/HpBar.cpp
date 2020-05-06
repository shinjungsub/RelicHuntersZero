#include "stdafx.h"
#include "HpBar.h"

//HpBar 렌더 함수
void HpBar::Render(Vector2 pos, int currentHp)
{
	//배경은 배경 사이즈로 설정
	//전경은 현재 Hp의 비율만큼 렌더
	_rc = Figure::RectMakeCenter(pos, _backgroundSize);
	float percent = (float)currentHp / (float)_maxHp;
	_background->SetSize({ _backgroundSize.x, _backgroundSize.y });
	_background->Render(_rc.left, _rc.top, Pivot::LEFT_TOP);
	_foreground->SetSize({ (float)_backgroundSize.x * percent, _backgroundSize.y });
	_foreground->Render(_rc.left, _rc.top, Pivot::LEFT_TOP);
}
