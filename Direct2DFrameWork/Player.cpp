#include "stdafx.h"
#include "Tile.h"
#include "Player.h"

void Player::Init()
{
	_kindOfImage = KindOfImage::Idle;

	_speed = SPEED;

	_imageFrameX = 0;
	_imageFrameY = 0;
	_imageCount = 0;
	_image = IMAGEMANAGER->FindImage("pinky_idle");
	_isLeft = false;
	_isDash = false;
	_isRun = false;
	_isMoveLeft = false;
	_isMoveUp = false;
	_isMoveRight = false;
	_isMoveDown = false;

	_currentGun = 0;

	for (int i = 0; i < 2; i++)
		_gun[i]->Init();

	_gun[0]->SetGunKinds(Gun::GunKinds::ShotGun);
	_gun[1]->SetGunKinds(Gun::GunKinds::MachineGun);
	_gun[_currentGun]->SetIsRender(true);
	_index = { (long)_mainRectPos.x / TILELENGTH, (long)_mainRectPos.y / TILELENGTH };
}

void Player::Release()
{
	SAFE_DELETE(_gun[0]);
	SAFE_DELETE(_gun[1]);
	SAFE_DELETE(_hpBar);
}

void Player::Update()
{
	KeyController();

	//마우스가 플레이어의 중앙보다 왼쪽에 있다면 isLeft는 true
	_isLeft = (_ptMouse.x + CAMERA->GetCamera().left < _position.x) ? true : false;

	//isLeft에 따라 총의 방향 및 포지션, 앵글값 바꿔주기
	if (_isLeft)
		_gun[_currentGun]->SetPosition({ _position.x - 10.0f, _position.y + 25.0f });
	else
		_gun[_currentGun]->SetPosition({ _position.x + 10.0f, _position.y + 25.0f });
	_gun[_currentGun]->SetAngle(Math::GetAngle(_position.x, _position.y, _ptMouse.x + CAMERA->GetCamera().left, _ptMouse.y + CAMERA->GetCamera().top));

	_gun[_currentGun]->SetIsLeft(_isLeft);

	for (int i = 0; i < 2; i++)
		_gun[i]->Update();

	UpdateImageFrame();

	UpdatePlayerImage();	//방향과 캐릭터 상태에 따라 이미지를 변환하므로 방향과 캐릭터 상태 변경하는 코드 밑에 있어야함
}

void Player::Render()
{
	RECT camRc = CAMERA->GetCamera();
	_image->SetSize({ _imageSize.x, _imageSize.y });
	_image->FrameRender(_position.x - camRc.left, _position.y - camRc.top, _imageFrameX, _imageFrameY, Pivot::CENTER);
	for (int i = 0; i < 2; i++)
		_gun[i]->Render();

	_hpBar->Render({ _position.x - camRc.left, _position.y - 50 - camRc.top }, _hp);

	if (KEYMANAGER->IsToggleKey(VK_F1))
	{
		D2DRENDERER->DrawRectangle(CAMERA->ReviseRect(_mainRect), D2DRenderer::DefaultBrush::Red);
	}
	else if (KEYMANAGER->IsToggleKey(VK_F2))
	{
		D2DRENDERER->DrawRectangle(CAMERA->ReviseRect(_hitRect), D2DRenderer::DefaultBrush::Blue);
	}
}

//키 컨트롤러
void Player::KeyController(void)
{
	if (!KEYMANAGER->IsStayKeyDown('A') && !KEYMANAGER->IsStayKeyDown('D') && !KEYMANAGER->IsStayKeyDown('S') && !KEYMANAGER->IsStayKeyDown('W'))
		SetKindOfImage(KindOfImage::Idle);
	if (KEYMANAGER->IsOnceKeyDown('A'))
	{
		_isMoveLeft = true;
	}
	if (KEYMANAGER->IsOnceKeyDown('D'))
	{
		_isMoveRight = true;
	}
	if (KEYMANAGER->IsOnceKeyDown('W'))
	{
		_isMoveUp = true;
	}
	if (KEYMANAGER->IsOnceKeyDown('S'))
	{
		_isMoveDown = true;
	}
	if (KEYMANAGER->IsOnceKeyUp('A'))
	{
		_isMoveLeft = false;
	}
	if (KEYMANAGER->IsOnceKeyUp('D'))
	{
		_isMoveRight = false;
	}
	if (KEYMANAGER->IsOnceKeyUp('W'))
	{
		_isMoveUp = false;
	}
	if (KEYMANAGER->IsOnceKeyUp('S'))
	{
		_isMoveDown = false;
	}

	if (KEYMANAGER->IsOnceKeyDown(VK_TAB))
	{
		_gun[_currentGun]->SetIsRender(false);
		_currentGun = _currentGun == 0 ? 1 : 0;
		_gun[_currentGun]->SetIsRender(true);
	}
	if (KEYMANAGER->IsOnceKeyDown(MK_LBUTTON))
	{
		_gun[_currentGun]->SetIsShoot(true);
		_gun[_currentGun]->Shoot();
	}
	if (KEYMANAGER->IsStayKeyDown(MK_LBUTTON))
	{
		_gun[_currentGun]->SetIsShoot(true);
		_gun[_currentGun]->Shoot();
	}
	if (KEYMANAGER->IsOnceKeyUp(MK_LBUTTON))
	{
		_gun[_currentGun]->SetIsShoot(false);
	}

	PlayerDirection();
}

//플레이어 상태에 따라 이미지 업데이트
void Player::UpdatePlayerImage()
{
	string imageName = "";
	switch (_kindOfImage)
	{
	case Player::KindOfImage::Idle:
		imageName = "pinky_idle";
		break;
	case Player::KindOfImage::Walk:
		imageName = "pinky_walk";
		break;
	case Player::KindOfImage::Run:
		imageName = "pinky_sprint";
		break;
	case Player::KindOfImage::Dash:
		imageName = "pinky_dash";
		break;
	case Player::KindOfImage::Die:
		imageName = "pinky_death";
		break;
	case Player::KindOfImage::End:
		break;
	default:
		break;
	}
	if (_isLeft)
		imageName += "_left";
	_image = IMAGEMANAGER->FindImage(imageName);
}

//플레이어 이미지 프레임 업데이트
void Player::UpdateImageFrame(void)
{
	_imageCount++;
	if (_imageCount > 5)
	{
		_imageFrameX++;
		_imageCount = 0;
	}
	if (_imageFrameX > _image->GetMaxFrameX())
	{
		_imageFrameX = 0;
	}
}

//방향에 따라 포지션 이동시키는 함수
void Player::MovePosition(void)
{
	if (_direction != Player::Direction::None)
	{
		if (_isDash)
			SetKindOfImage(KindOfImage::Dash);
		else if (_isRun)
			SetKindOfImage(KindOfImage::Run);
		else
			SetKindOfImage(KindOfImage::Walk);
	}
	switch (_direction)
	{
	case Player::Direction::None:
		break;
	case Player::Direction::Left:
		SetPosition({ _position.x - _speed, _position.y });
		break;
	case Player::Direction::LeftUp:
		SetPosition({ _position.x - _speed, _position.y - _speed });
		break;
	case Player::Direction::Up:
		SetPosition({ _position.x , _position.y - _speed });
		break;
	case Player::Direction::UpRight:
		SetPosition({ _position.x + _speed, _position.y - _speed });
		break;
	case Player::Direction::Right:
		SetPosition({ _position.x + _speed, _position.y });
		break;
	case Player::Direction::RightDown:
		SetPosition({ _position.x + _speed, _position.y + _speed });
		break;
	case Player::Direction::Down:
		SetPosition({ _position.x , _position.y + _speed });
		break;
	case Player::Direction::DownLeft:
		SetPosition({ _position.x - _speed, _position.y + _speed });
		break;
	default:
		break;
	}
}

//플레이어 방향 설정
void Player::PlayerDirection(void)
{
	if (!_isMoveLeft && !_isMoveUp && !_isMoveRight && !_isMoveDown)		//움직이지 않음
		_direction = Direction::None;
	if (_isMoveLeft && !_isMoveUp && !_isMoveRight && !_isMoveDown)	//좌
		_direction = Direction::Left;
	if (!_isMoveLeft && _isMoveUp && !_isMoveRight && !_isMoveDown)	//상
		_direction = Direction::Up;
	if (!_isMoveLeft && !_isMoveUp && _isMoveRight && !_isMoveDown)	//우
		_direction = Direction::Right;
	if (!_isMoveLeft && !_isMoveUp && !_isMoveRight && _isMoveDown)	//하
		_direction = Direction::Down;
	if (_isMoveLeft && _isMoveUp && !_isMoveRight && !_isMoveDown)		//좌상
		_direction = Direction::LeftUp;
	if (!_isMoveLeft && _isMoveUp && _isMoveRight && !_isMoveDown)		//우상
		_direction = Direction::UpRight;
	if (!_isMoveLeft && !_isMoveUp && _isMoveRight && _isMoveDown)		//우하
		_direction = Direction::RightDown;
	if (_isMoveLeft && !_isMoveUp && !_isMoveRight && _isMoveDown)		//좌하
		_direction = Direction::DownLeft;
	MovePosition();
}

