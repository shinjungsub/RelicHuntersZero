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
	// �׾��� �� �� ����
	if (_kindOfImage == KindOfImage::Death) return;
	_gun->SetIsLeft(_isLeft);
	if (_isLeft)
		_gun->SetPosition({ _position.x - 10.0f, _position.y + 25.0f });
	else
		_gun->SetPosition({ _position.x + 10.0f, _position.y + 25.0f });
	// ���� ���°� �ƴ� �� ���� ������ ���� or ������ ��
	if (_state != State::Attack)
		_gun->SetAngle(_isLeft ? Math::PI : 0.0f);
	_gun->Update();
}

void Duck::Render()
{
	if (!_isActive) return;
	RECT camRc = CAMERA->GetCamera();

	//���� �̹��� ����
	_image->SetSize({ _imageSize.x, _imageSize.y });
	_image->FrameRender(_position.x - camRc.left, _position.y - camRc.top, _imageFrameX, 0, Pivot::CENTER, true);

	//�׾��� �� HpBar, ��, ����ǥ ����
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

// Ÿ�� ���� �޾ƿ��� �Լ�
void Duck::ReceiveChangeTileInfo(int idX, int idY, bool isDestroy)
{
	if (isDestroy)
		_aStar->DestroyStruct(idX, idY);
	else
		_aStar->CreateStruct(idX, idY);
	_isReSearch = true;
}

// ���� ������ �Ѱ��ϴ� �Լ�
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
	
	// ������ �ִ� ���³� �ڵ����� �����̰� ���� �� �÷��̾ �߰��ϰ� �Ǹ� �������·� �ٲ���
	if (_state == State::Stay || _state == State::RandomMove)
	{
		if((_player->GetMainRectPos().x < _mainRectPos.x && _isLeft) || (_player->GetMainRectPos().x > _mainRectPos.x && !_isLeft))
			if (!CheckStruct(_mainRectPos.x, _mainRectPos.y, _player->GetMainRectPos().x, _player->GetMainRectPos().y))
				_state = State::Advance;
	}

	// �������¿� �ڵ� �����ӿ��� A*�� ����ϱ� ���� ����
	if ((_state == State::Advance || _state == State::RandomMove) && _state == _statePrev)
	{
		//������ �ε����� �ٲ���� �� ��Ž���� ���� isReSearch�� true�� �ٲ���
		if ((_endTileIndex.x != _endTileIndexPrev.x) || (_endTileIndex.y != _endTileIndexPrev.y))
		{
			_isReSearch = true;
		}
		// ��Ž��
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
			// �̵��� Ÿ���� ���Ͱ� ������� �ʴٸ� ���� Ÿ���� ���͸� �־���
			if (_targetCenter == NULL)
			{
				_targetCenter = _aStar->GetNextTargetCenter();
			}
			// ������ ��ǥ Ÿ�ϰ��� �Ÿ��� �̵� �ӵ����� �۴ٸ� ������ ������ �����ϰ� ���� Ÿ���� ���Ͱ��� �־���
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
		//�̵��� Ÿ���� ����ִٸ� �ޱ��� ã�Ƽ� �־��ְ� �̵���Ŵ
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

// ������ �ִ� ���� - ��Ȳ�� �°� ������ �� ���¸� �ٲ���
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

// �ڵ� ������ - ���� �ֺ��� 2ĭ ������ �ִ� Ÿ���� ��ǥ�� �����Ͽ� A*�� ���� ã�� �̵�
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

// �÷��̾� �Ѿ˿� �¾��� �� �ڷ� �и��� �Լ�
void Duck::NuckBack(float angle)
{
	if (!_isAngleCheck)
	{
		float gunAngle = _gun->GetAngle();
		// ���� �ޱ۰� ���� ��� �ִ� �ޱ��� �ݴ� �����̶�� �� �ޱ��� ���� �������� ��������
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
	// ��Ʈ �̹����� ������ �ٽ� ���� ���·� �ǵ���
	if (_kindOfImage != KindOfImage::Hit)
	{
		_isAngleCheck = false;
		_state = State::Stay;
	}
}

// �뽬�ϴ� �Լ� - �浹 �˻縦 ���� �ϰ� �浹�� ���ϸ� �̵���Ŵ, �浹 �� Stay�� �ٲ���
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

//�����ϴ� �Լ� - A*�� ����Ͽ� �ִܰŸ��� ã�� �̵�
void Duck::Advance()
{
	_isDiscover = true;
	if (_statePrev != State::Advance)
	{
		_isReSearch = true;
		_statePrev = _state;
	}
	_endTileIndex = { (long)_player->GetMainRectPos().x / TILELENGTH, (long)_player->GetMainRectPos().y / TILELENGTH };

	//���ðŸ� ������ ������ ���̿� ��ü�� ���� �� ���� ���·� �ٲ� ��
	if (Math::GetDistance(_mainRectPos.x, _mainRectPos.y, _player->GetMainRectPos().x, _player->GetMainRectPos().y) < SHOOTINGDISTANCE)
	{
		if (!CheckStruct(_mainRectPos.x, _mainRectPos.y, _player->GetMainRectPos().x, _player->GetMainRectPos().y))
			_state = State::Attack;
	}
}

//���� - �����ϴٰ� �������� �����س��� �ð��� �Ǹ� �뽬�� �ٲ���(�÷��̾��� �Ѿ��� ���ϰ� ���������� ���̱� ����)
void Duck::Shoot()
{
	_isLeft = _player->GetMainRectPos().x < _mainRectPos.x;
	static float shootTime = 0.0f;
	static float randomShootTime = Math::Random(3.0f, 5.0f);
	shootTime += TIMEMANAGER->getElapedTime();
	if (shootTime < randomShootTime)
	{
		// �Ÿ��� �־����ų� ���̿� ��ü�� ������ �ٽ� �����ϴ� ���·� �ٲ���
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

//�Ѿ˰� �浹���� �� �Ѿ��� ������ �Ŀ��� �޾ƿ�
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

// ���� ���ݾ� �̵���Ű�鼭 �÷��̾�� ���̿� ��ü�� �ִ��� üũ�ϴ� �Լ�
bool Duck::CheckStruct(float startX, float startY, float endX, float endY)
{
	float angle = Math::GetAngle(startX, startY, endX, endY);
	//cos�� sin�� while�ۿ��� ���� ��� �� while�ȿ� ���� ������ ��
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
