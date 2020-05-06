#include "stdafx.h"
#include "Duck.h"
#include "Gun.h"

Duck::Duck(string name, Vector2 pos)
	: Enemy(name, pos, { pos.x, pos.y + 43 }, { pos.x, pos.y + 10 }, { 38, 10 }, { 36, 54 }, { 114, 94 })
{
	_gun = new Gun;
	_hp = 150;
}

void Duck::Init()
{
	_player = OBJECTMANAGER->FindObject(ObjectType::Object, "player");
	_playerIndex = { _player->GetIndex().x, _player->GetIndex().y };
	_image = IMAGEMANAGER->FindImage("duck_idle");
	_isDiscover = false;
	_isLeft = true;
	_isShoot = false;
	_isReSearch = false;
	_isAngleCheck = false;
	_imageFrameX = 0;
	_angle = 0.0f;
	_hitAngle = 0.0f;
	_speed = 2.0f;
	_index = { (long)_mainRectPos.x / TILELENGTH, (long)_mainRectPos.y / TILELENGTH };
	_endTileIndex = _index;
	_endTileIndexPrev = _endTileIndex;
	_imageCount = 0;
	_hp = 150;
	_targetCenter = NULL;
	_state = State::Stay;
	_statePrev = _state;
	SetKindOfImage(KindOfImage::Idle);
	_gun->Init();
	_gun->SetGunKinds((Gun::GunKinds)Math::Random(1, 3));
	_gun->SetIsRender(true);
}

void Duck::Release()
{
	delete _gun;
}

void Duck::Update()
{
	BeHavier();
	UpdateImage();
	UpdateImageFrame();
	// 죽었을 때 총 끄기
	if (_kindOfImage == KindOfImage::Death) return;
	_gun->SetIsLeft(_isLeft);
	if (_isLeft)
		_gun->SetPosition({ _position.x - 10.0f, _position.y + 25.0f });
	else
		_gun->SetPosition({ _position.x + 10.0f, _position.y + 25.0f });
	// 공격 상태가 아닐 때 총의 각도는 왼쪽 or 오른쪽 뿐
	if (_state != State::Attack)
		_gun->SetAngle(_isLeft ? Math::PI : 0.0f);
	_gun->Update();
}

void Duck::Render()
{
	if (!_isActive) return;
	RECT camRc = CAMERA->GetCamera();

	//오리 이미지 렌더
	_image->SetSize({ _imageSize.x, _imageSize.y });
	_image->FrameRender(_position.x - camRc.left, _position.y - camRc.top, _imageFrameX, 0, Pivot::CENTER, true);

	//죽었을 때 HpBar, 총, 느낌표 끄기
	if (_kindOfImage == KindOfImage::Death) return;

	_gun->Render();

	_hpBar->Render({ _position.x - camRc.left, _position.y - 50 - camRc.top }, _hp);
	if (_isDiscover)
	{
		IMAGEMANAGER->FindImage("ExclamationMark")->SetSize({ 30, 20 });
		IMAGEMANAGER->FindImage("ExclamationMark")->Render(_position.x + 40 - camRc.left, _position.y - camRc.top);
	}

	if (KEYMANAGER->IsToggleKey(VK_F1))
	{
		D2DRENDERER->DrawRectangle(CAMERA->ReviseRect(_mainRect), D2DRenderer::DefaultBrush::Red);
	}
	if (KEYMANAGER->IsToggleKey(VK_F2))
	{
		D2DRENDERER->DrawRectangle(CAMERA->ReviseRect(_hitRect), D2DRenderer::DefaultBrush::Blue);
	}
}

// 타일 정보 받아오는 함수
void Duck::ReceiveChangeTileInfo(int idX, int idY, bool isDestroy)
{
	if (isDestroy)
		_aStar->DestroyStruct(idX, idY);
	else
		_aStar->CreateStruct(idX, idY);
	_isReSearch = true;
}

// 오리 움직임 총괄하는 함수
void Duck::BeHavier()
{
	if (_kindOfImage == KindOfImage::Death) return;
	if (_state != State::Dash) _speed = 2.0f;
	if (_state != State::Hit) _isAngleCheck = false;
	
	switch (_state)
	{
	case Enemy::State::Stay:
		SetKindOfImage(KindOfImage::Idle);
		Stay();
		break;
	case Enemy::State::RandomMove:
		SetKindOfImage(KindOfImage::Walk);
		RandomMove();
		break;
	case Enemy::State::Hit:
		SetKindOfImage(KindOfImage::Hit);
		NuckBack(_hitAngle);
		break;
	case Enemy::State::Dash:
		SetKindOfImage(KindOfImage::Dash);
		Dash();
		break;
	case Enemy::State::Advance:
		SetKindOfImage(KindOfImage::Walk);
		Advance();
		break;
	case Enemy::State::Attack:
		SetKindOfImage(KindOfImage::Idle);
		Shoot();
		break;
	default:
		break;
	}
	
	// 가만히 있는 상태나 자동으로 움직이고 있을 때 플레이어를 발견하게 되면 추적상태로 바꿔줌
	if (_state == State::Stay || _state == State::RandomMove)
	{
		if((_player->GetMainRectPos().x < _mainRectPos.x && _isLeft) || (_player->GetMainRectPos().x > _mainRectPos.x && !_isLeft))
			if (!CheckStruct(_mainRectPos.x, _mainRectPos.y, _player->GetMainRectPos().x, _player->GetMainRectPos().y))
				_state = State::Advance;
	}

	// 추적상태와 자동 움직임에서 A*를 사용하기 위한 설정
	if ((_state == State::Advance || _state == State::RandomMove) && _state == _statePrev)
	{
		//마지막 인덱스가 바뀌었을 때 재탐색을 위해 isReSearch를 true로 바꿔줌
		if ((_endTileIndex.x != _endTileIndexPrev.x) || (_endTileIndex.y != _endTileIndexPrev.y))
		{
			_isReSearch = true;
		}
		// 재탐색
		if (_isReSearch)
		{
			// aStar setTile
			_aStar->SetTiles({ _index.x, _index.y }, _endTileIndex, true);
			_aStar->PathFinder(_index);
			_endTileIndexPrev = _endTileIndex;
			_isReSearch = false;
		}
		// astar - find
		if (_aStar->GetIsFind())
		{
			// 이동할 타일의 센터가 담겨있지 않다면 다음 타일의 센터를 넣어줌
			if (_targetCenter == NULL)
			{
				_targetCenter = _aStar->GetNextTargetCenter();
			}
			// 오리와 목표 타일과의 거리가 이동 속도보다 작다면 도착한 것으로 간주하고 다음 타일의 센터값을 넣어줌
			else if (Math::GetSqrMagnitude(_mainRectPos.x, _mainRectPos.y, _targetCenter.x, _targetCenter.y) < _speed * _speed)
			{
				_targetCenter = _aStar->GetNextTargetCenter();
				if (_targetCenter == NULL)
					_state = State::Stay;
			}
		}
		// astar - not find
		else
		{
			_isReSearch = true;
		}
		//이동할 타일이 들어있다면 앵글을 찾아서 넣어주고 이동시킴
		if (_targetCenter != NULL)
		{
			_angle = Math::GetAngle(_mainRectPos.x, _mainRectPos.y, _targetCenter.x, _targetCenter.y);
			if (_angle > 2.0f * Math::PI) _angle -= (2.0f * Math::PI);
			_isLeft = Math::ValueInRange(_angle, Math::PI / 2.0f, Math::PI * 3.0f / 2.0f);
			if (_isDiscover)
			{
				if (_player->GetMainRectPos().x < _mainRectPos.x)
					_isLeft = true;
				else
					_isLeft = false;
			}
			SetPosition({ _position.x + cosf(_angle) * _speed , _position.y - sinf(_angle) * _speed });
		}
	}
	else
	{
		_isReSearch = true;
		_targetCenter = NULL;
	}
}

// 가만히 있는 상태 - 상황에 맞게 딜레이 후 상태를 바꿔줌
void Duck::Stay()
{
	_statePrev = _state;
	static float stayTime = 0.0f;
	stayTime += TIMEMANAGER->getElapedTime();
	float distance = Math::GetDistance(_mainRectPos.x, _mainRectPos.y, _player->GetMainRectPos().x, _player->GetMainRectPos().y);
	if (distance < SHOOTINGDISTANCE)
	{
		if (stayTime > 0.5f)
		{
			_state = State::Attack;
			stayTime = 0.0f;
		}
	}
	else if (distance < DISCOVERDISTANCE)
	{
		if (stayTime > 1.0f)
		{
			_state = State::Advance;
			stayTime = 0.0f;
		}
	}
	else
	{
		if (_isDiscover)
		{
			_state = State::Advance;
			stayTime = 0.0f;
		}
		else
		{
			if (stayTime > 2.0f)
			{
				_state = State::RandomMove;
				stayTime = 0.0f;
			}
		}
	}
}

// 자동 움직임 - 오리 주변에 2칸 떨어져 있는 타일을 목표로 설정하여 A*로 길을 찾아 이동
void Duck::RandomMove()
{
	if (_statePrev != State::RandomMove)
	{
		int x = 0;
		int y = 0;
		while (true)
		{
			x = Math::Random(_index.x - 2, _index.x + 2);
			y = Math::Random(_index.y - 2, _index.y + 2);
			if (x < 0 || y < 0) continue;
			if (x > TILENUMX || y > TILENUMY) continue;
			if (_tiles[y][x]->GetHaveObject()) continue;
			if (Math::Abs(_index.x - x) == 2 || Math::Abs(_index.y - y) == 2)
				break;
		}
		_endTileIndex = { x, y };
		_statePrev = _state;
	}
}

// 플레이어 총알에 맞았을 시 뒤로 밀리는 함수
void Duck::NuckBack(float angle)
{
	if (!_isAngleCheck)
	{
		float gunAngle = _gun->GetAngle();
		// 맞은 앵글과 총을 들고 있는 앵글이 반대 방향이라면 총 앵글을 수직 법선으로 뒤집어줌
		if (angle > Math::PI / 2.0f && angle <= 3.0f * Math::PI / 2.0f)
		{
			_isLeft = false;
			if (gunAngle <= Math::PI / 2.0f || gunAngle > 3.0f * Math::PI / 2.0f)
			{
				gunAngle = Math::PI - gunAngle;
			}
		}
		else
		{
			_isLeft = true;
			if (gunAngle > Math::PI / 2.0f && gunAngle <= 3.0f * Math::PI / 2.0f)
			{
				gunAngle = Math::PI - gunAngle;
			}
		}
		if (gunAngle > 2.0f * Math::PI) gunAngle -= 2.0f * Math::PI;
		if (gunAngle < 0.0f) gunAngle = 2.0f * Math::PI + gunAngle;
		_gun->SetAngle(gunAngle);
		_isAngleCheck = true;
	}
	SetPosition({ _position.x + cosf(angle) * 2.0f, _position.y - sinf(angle) * 2.0f });
	// 히트 이미지가 끝나면 다시 원래 상태로 되돌림
	if (_kindOfImage != KindOfImage::Hit)
	{
		_isAngleCheck = false;
		_state = State::Stay;
	}
}

// 대쉬하는 함수 - 충돌 검사를 먼저 하고 충돌을 안하면 이동시킴, 충돌 시 Stay로 바꿔줌
void Duck::Dash()
{
	if (_statePrev != _state)
	{
		_speed = 10.0f;
		_angle = Math::Random(0.0f, Math::PI * 2.0f);
		_statePrev = _state;
	}
	Vector2 pos = this->_mainRectPos;
	pos.x += (cosf(_angle) * _speed);
	pos.y -= (sinf(_angle) * _speed);
	RECT rc = Figure::RectMakeCenter(pos, _mainSize);
	POINT index = { (long)pos.x / TILELENGTH, (long)pos.y / TILELENGTH };
	Figure::NonOverIdx(index, { 1, 1 }, { TILENUMX - 2, TILENUMY - 2 });
	for (int i = index.y - 1; i <= index.y + 1; i++)
	{
		for (int j = index.x - 1; j <= index.x + 1; j++)
		{
			if (!_tiles[i][j]->GetHaveObject()) continue;
			if(Figure::IsCollision(rc, _tiles[i][j]->GetRect()))
			{
				_state = State::Stay;
				_kindOfImage = KindOfImage::Idle;
				_imageFrameX = 0;
				return;
			}
		}
	}
	if (_kindOfImage != KindOfImage::Dash)
	{
		_state = State::Stay;
		_kindOfImage = KindOfImage::Idle;
		_imageFrameX = 0;
		return;
	}
	SetPosition({ _position.x + cosf(_angle) * _speed, _position.y - sinf(_angle) * _speed });
}

//추적하는 함수 - A*를 사용하여 최단거리를 찾아 이동
void Duck::Advance()
{
	_isDiscover = true;
	if (_statePrev != State::Advance)
	{
		_isReSearch = true;
		_statePrev = _state;
	}
	_endTileIndex = { (long)_player->GetMainRectPos().x / TILELENGTH, (long)_player->GetMainRectPos().y / TILELENGTH };

	//슈팅거리 안으로 들어오고 사이에 물체가 없을 때 공격 상태로 바꿔 줌
	if (Math::GetDistance(_mainRectPos.x, _mainRectPos.y, _player->GetMainRectPos().x, _player->GetMainRectPos().y) < SHOOTINGDISTANCE)
	{
		if (!CheckStruct(_mainRectPos.x, _mainRectPos.y, _player->GetMainRectPos().x, _player->GetMainRectPos().y))
			_state = State::Attack;
	}
}

//공격 - 공격하다가 랜덤으로 설정해놓은 시간이 되면 대쉬로 바꿔줌(플레이어의 총알을 피하고 역동적으로 보이기 위해)
void Duck::Shoot()
{
	_isLeft = _player->GetMainRectPos().x < _mainRectPos.x;
	static float shootTime = 0.0f;
	static float randomShootTime = Math::Random(3.0f, 5.0f);
	shootTime += TIMEMANAGER->getElapedTime();
	if (shootTime < randomShootTime)
	{
		// 거리가 멀어지거나 사이에 물체가 있으면 다시 추적하는 상태로 바꿔줌
		if (Math::GetDistance(_mainRectPos.x, _mainRectPos.y, _player->GetMainRectPos().x, _player->GetMainRectPos().y) > DISCOVERDISTANCE
			|| CheckStruct(_mainRectPos.x, _mainRectPos.y, _player->GetMainRectPos().x, _player->GetMainRectPos().y))
		{
			_gun->SetIsShoot(false);
			shootTime = 0.0f;
			randomShootTime = Math::Random(3.0f, 5.0f);
			_state = State::Advance;
			return;
		}
		float angle = Math::GetAngle(_position.x, _position.y, _player->GetPosition().x, _player->GetPosition().y);
		angle = Math::Random(angle - 0.1f, angle + 0.1f);
		_gun->SetAngle(angle);
		_gun->SetIsShoot(true);
		_gun->Shoot();
	}
	else
	{
		_gun->SetIsShoot(false);
		shootTime = 0.0f;
		randomShootTime = Math::Random(3.0f, 5.0f);
		_state = State::Dash;
	}
}

//총알과 충돌했을 때 총알의 각도와 파워를 받아옴
void Duck::HitByBullet(float angle, int power)
{
	_state = State::Hit;
	_hitAngle = angle;
	SetHp(_hp - power);
}

void Duck::UpdateImage()
{
	string imageName = "";
	switch (_kindOfImage)
	{
	case Enemy::KindOfImage::Idle:
		imageName = "duck_idle";
		break;
	case Enemy::KindOfImage::Walk:
		imageName = "duck_walk";
		break;
	case Enemy::KindOfImage::Hit:
		imageName = "duck_hit";
		break;
	case Enemy::KindOfImage::Death:
		imageName = "duck_death";
		break;
	case Enemy::KindOfImage::Dash:
		imageName = "duck_dash";
		break;
	default:
		break;
	}
	if (_isLeft)
		imageName += "_left";
	_image = IMAGEMANAGER->FindImage(imageName);
}

bool Duck::ReSearch()
{
	if (_playerIndex.x != _player->GetIndex().x || _playerIndex.y != _player->GetIndex().y)
	{
		_playerIndex.x = _player->GetIndex().x;
		_playerIndex.y = _player->GetIndex().y;
		return true;
	}
	return false;
}

void Duck::AutoShoot()
{
	if (_isShoot)
	{
		_shootDelayTime += TIMEMANAGER->getElapedTime();

		if (_shootDelayTime)
			_gun->Shoot();
	}
}

// 점을 조금씩 이동시키면서 플레이어와 사이에 물체가 있는지 체크하는 함수
bool Duck::CheckStruct(float startX, float startY, float endX, float endY)
{
	float angle = Math::GetAngle(startX, startY, endX, endY);
	//cos과 sin을 while밖에서 먼저 계산 후 while안에 값을 가지고 들어감
	float moveX = cosf(angle) * 20.0f;
	float moveY = -sinf(angle) * 20.0f;
	Vector2 pos = { startX, startY };
	while (true)
	{
		pos = { pos.x + moveX, pos.y + moveY };
		if (startX < endX)
		{
			if (pos.x >= endX)
			{
				break;
			}
		}
		else if (startX > endX)
		{
			if (pos.x <= endX)
			{
				break;
			}
		}
		if (startY < endY)
		{
			if (pos.y >= endY)
			{
				break;
			}
		}
		else if (startY > endY)
		{
			if (pos.y <= endY)
			{
				break;
			}
		}
		if (_tiles[(int)pos.y / TILELENGTH][(int)pos.x / TILELENGTH]->GetHaveObject()) return true;
	}
	return false;
}
