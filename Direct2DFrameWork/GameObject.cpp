#include "stdafx.h"
#include "GameObject.h"
#include "Tile.h"

#include <functional>
#include <unordered_map>

/*************************************************************************
## GameObject ##
**************************************************************************/
GameObject::GameObject()
	:_name(""), _pivot(Pivot::CENTER), _position(0, 0), _mainSize(0, 0), _imageSize(0, 0), _isActive(true), _isLive(true)
{
	this->UpdateMainRect();
}
/*************************************************************************
## GameObject ##
@@ string name : ��ü �̸�
@@ Vector2 pos : ��ü ��ǥ
@@ Vector2 size : ��ü ������
@@ Pivot::Enum pivot : ��ü �Ǻ�
**************************************************************************/
GameObject::GameObject(string name, Vector2 pos, Vector2 mainPos, Vector2 hitPos, Vector2 mainSize, Vector2 hitSize, Vector2 imageSize, Pivot::Enum pivot)
	:_name(name), _position(pos), _mainRectPos(mainPos), _hitRectPos(hitPos), _mainSize(mainSize), _hitSize(hitSize), _imageSize(imageSize), _pivot(pivot), _isActive(true), _isLive(true)
{
	this->UpdateMainRect();
	this->UpdateHitRect();
}

GameObject::GameObject(const RECT rect)
	: _name(""), _pivot(Pivot::LEFT_TOP), _isActive(true), _isLive(true)
{
	this->_mainRect = rect;
	this->_position = Vector2(_mainRect.left, _mainRect.top);
	this->_mainSize = Vector2(_mainRect.right - _mainRect.left, _mainRect.bottom - _mainRect.top);
}

/*************************************************************************
## ~GameObject ##
**************************************************************************/
GameObject::~GameObject()
{
	this->_callbackList.clear();
	this->_reserveMessageList.clear();
}

void GameObject::Init()
{
}

/*************************************************************************
## Release ##
**************************************************************************/
void GameObject::Release()
{
	_reserveMessageList.clear();
}
/*************************************************************************
## Update ##
���� �޼����� �ִٸ� ����ð��� ��ٰ� �ð��� �ٵǾ��ٸ� �ش� �޼����� �Լ��� ȣ��
**************************************************************************/
void GameObject::Update()
{
	float deltaTime = TIMEMANAGER->getElapedTime();
	for (UINT i = 0; i < _reserveMessageList.size(); ++i)
	{
		//����� �޼����� ����ð��� deltaTime��ŭ ���ش�
		_reserveMessageList[i].delayTime -= deltaTime;
		//���� ����� �ð��� �ٵǾ��ٸ� 
		if (_reserveMessageList[i].delayTime <= 0.f)
		{
			//�ش� �޼����� �Լ��� ��������ش�. 
			this->SendCallbackMessage(_reserveMessageList[i]);
			_reserveMessageList.erase(_reserveMessageList.begin() + i--);
		}
	}
}
void GameObject::Render()
{
	if (KEYMANAGER->IsToggleKey(VK_F1))
	{
		D2DRENDERER->DrawRectangle(_mainRect, D2DRenderer::DefaultBrush::Red, true);
	}
}
/*************************************************************************
## SendCallbackMessage ##
@@ TagMessage message : �ش� ��ü�� ���� �޼���
**************************************************************************/
void GameObject::SendCallbackMessage(const TagMessage message)
{
	//���� ������ Ÿ���� ���ٸ� 
	if (message.delayTime <= 0.0f)
	{
		//�ٷ� �ش� �޼����� �̸��� ���� �Լ��� ã�Ƽ� ��������ش�. 
		CallbackHashmapIter iter = _callbackList.find(message.name);
		if (iter != _callbackList.end())
		{
			iter->second(message);
		}
	}
	//���� �޼����� ������ Ÿ���� �ִٸ�
	else
	{
		//���� �޼�������Ʈ�� �־�д�. 
		//GameObject�� ������Ʈ���� ���� �޼����� �ð��� ��ٰ� �ð��� �ٵǾ��ٸ� ȣ��
		_reserveMessageList.push_back(message);
	}
}
/*************************************************************************
## AddCallbackMessage ##
�޼��� ��ſ� ����� �Լ��� �̸� ����Ѵ�.
@@ string name : ����� �Լ��� �̸�
@@ function<void(TagMessage)> func : ����� �Լ�
**************************************************************************/
void GameObject::AddCallbackMessage(const string name, const function<void(struct TagMessage)> func)
{
	if (_callbackList.find(name) == _callbackList.end())
	{
		_callbackList.insert(make_pair(name, func));
	}
}

/*************************************************************************
## SetPivot ##
@@ Pivot::Enum pivot : �Ǻ�
**************************************************************************/
void GameObject::SetPivot(const Pivot::Enum pivot)
{
	this->_pivot = pivot;
	this->UpdateMainRect();
	this->UpdateHitRect();
}
/*************************************************************************
## SetPosition ##
@@ Vector2 pos : �ٲ��� ��ǥ
**************************************************************************/
void GameObject::SetPosition(const Vector2 position)
{
	Vector2 movePos = { position.x - _position.x , position.y - _position.y };
	this->_position = position;
	this->_mainRectPos += movePos;
	this->_hitRectPos += movePos;
	this->_index = { (long)_mainRectPos.x / TILELENGTH, (long)_mainRectPos.y / TILELENGTH };
	this->UpdateMainRect();
	this->UpdateHitRect();
}
/*************************************************************************
## SetMainSize ##
@@ Vector2 size : �ٲ��� ������
**************************************************************************/
void GameObject::SetMainSize(const Vector2 size)
{
	this->_mainSize = size;
	this->UpdateMainRect();
}
/*************************************************************************
## SetHitSize ##
@@ Vector2 size : �ٲ��� ������
**************************************************************************/
void GameObject::SetHitSize(const Vector2 size)
{
	this->_hitSize = size;
	this->UpdateHitRect();
}
/*************************************************************************
## SetImageSize ##
@@ Vector2 size : �ٲ��� ������
**************************************************************************/
void GameObject::setImageSize(const Vector2 size)
{
	this->_imageSize = size;
}
/*************************************************************************
## SetActive ##
Ȱ�����¸� �ٲ��ְ� Ȱ��,��Ȱ���ɶ��� ����Ǿ�� �� �Լ��� ȣ��
@@ bool b : Ȱ�� ����
**************************************************************************/
void GameObject::SetActive(const bool b)
{
	this->_isActive = b;
	if (_isActive)
		this->Enable();
	else
		this->Disable();
}
void GameObject::SetMainRect(const RECT rect)
{
	this->_mainRect = rect;
}
void GameObject::SetHitRect(const RECT rect)
{
	this->_hitRect = rect;
}
/*************************************************************************
## UpdateMainRect ##
�Ǻ��� ���� ��Ʈ�� ������ �������ش�.
**************************************************************************/
void GameObject::UpdateMainRect()
{
	switch (_pivot)
	{
	case Pivot::LEFT_TOP:
		this->_mainRect = Figure::RectMake(_mainRectPos, _mainSize);
		break;
	case Pivot::CENTER:
		this->_mainRect = Figure::RectMakeCenter(_mainRectPos, _mainSize);
		break;
	case Pivot::BOTTOM:
		this->_mainRect = Figure::RectMakeBottom(_mainRectPos, _mainSize);
		break;
	default:
		break;
	}
}
/*************************************************************************
## UpdateHitRect ##
�Ǻ��� ���� ��Ʈ�� ������ �������ش�.
**************************************************************************/
void GameObject::UpdateHitRect()
{
	switch (_pivot)
	{
	case Pivot::LEFT_TOP:
		this->_hitRect = Figure::RectMake(_hitRectPos, _hitSize);
		break;
	case Pivot::CENTER:
		this->_hitRect = Figure::RectMakeCenter(_hitRectPos, _hitSize);
		break;
	case Pivot::BOTTOM:
		this->_hitRect = Figure::RectMakeBottom(_hitRectPos, _hitSize);
		break;
	default:
		break;
	}
}
