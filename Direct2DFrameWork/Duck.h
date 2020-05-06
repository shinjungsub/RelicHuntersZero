#pragma once
#include "Enemy.h"

#define SHOOTINGDISTANCE 400
#define DISCOVERDISTANCE 600
class Duck final : public Enemy
{
private:

	class Gun* _gun;	//��

	vector<vector<Tile*>> _tiles;	//Ÿ�� ����
	POINT _playerIndex;				//�÷��̾� �ε���
	GameObject* _player;			//�÷��̾�

	POINT _endTileIndex;			//������Ÿ�� �ε���
	POINT _endTileIndexPrev;		//������Ÿ�� �� �ε���

	float _shootDelayTime;			//�ѽ�� ���� ������ ��Ű�� ����
	bool _isShoot;					//�ڵ����� �� �� ����� ����
	bool _isReSearch;				//���� �ٽ� ã�ƾ� �ϴ°�
	bool _isAngleCheck;				//�Ѿ˿� �¾��� �� ���� �ݴ�� ������ �ֱ� ���� �ޱ��� üũ�ߴ°�
public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;

	////////////////////
	// Set �Լ�
	////////////////////
	void SetHp(int hp)
	{
		_hp = hp;
		if (_hp <= 0)
		{
			SetKindOfImage(KindOfImage::Death);
		}
	}
	void SetPlayer(GameObject* player) { _player = player; }
	void SetIsReSearch(bool b) { _isReSearch = b; }

	////////////////////
	// Get �Լ�
	////////////////////
	int GetHp() { return _hp; }
	Gun* GetGun() { return _gun; }
	AStar* GetAStar() { return _aStar; }

	//A*�� ����ϰų� �÷��̾���� ���̿� ��ü�� �ִ��� Ȯ���ϱ� ���� ��ü�� �ı��� ��츦 Ȯ���ϱ� ���� �Լ�
	void ReceiveChangeTileInfo(int idX, int idY, bool isDestroy);
	//A*�� ���� Ÿ�� ���� �޾ƿ���
	void AStarTileSetting(vector<vector<Tile*>> tiles)
	{
		_tiles = tiles;
		_aStar->SetTotalList(tiles);
		_isReSearch = true;
	}
	//�����ӿ� ���õ� �Լ�
	void BeHavier();
	//������ �ִ� ����
	void Stay();
	//�ڵ����� �����̴� ����
	void RandomMove();
	//�°� �ڷ� �и��� ����
	void NuckBack(float angle);
	//�Ѿ˿� �¾��� �� ���� ����� �Ѿ��� �Ŀ��� �޾ƿ��� ���� �Լ�
	void HitByBullet(float angle, int power);
	//�뽬 ����
	void Dash();
	//�÷��̾� ���� ����
	void Advance();
	//���� ����
	void Shoot();
	//���¿� ���� �̹��� ������Ʈ
	void UpdateImage();
	//�÷��̾��� �ε����� ���ߴ��� Ȯ���ϱ� ���� �Լ�
	bool ReSearch();
	//�ڵ� ���
	void AutoShoot();
	//�÷��̾���� ���̿� ��ü�� �ִ��� Ȯ���ϱ� ���� �Լ�
	bool CheckStruct(float startX, float startY, float endX, float endY);
	//A*�� ���� ����ϰ� �ִ°�
	bool UseAstar()
	{
		if (_state == State::RandomMove || _state == State::Advance)
			return true;
		else
			return false;
	}

	Duck() {}
	Duck(string name, Vector2 pos);
	~Duck() {}
};

