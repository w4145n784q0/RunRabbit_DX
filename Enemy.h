#pragma once
#include "Engine/GameObject.h"
#include"Player.h"

class Player;

class Enemy :
    public GameObject
{
private:
    int hModel_Enemy;
	Player* pPlayer_;
	enum State {
		S_IDLE = 0,//�ҋ@
		S_SUSPICIOUS,//������
		S_CHASE,//�ǂ�������
		S_ATTACK,//�U��
		S_MOVE,//�ړ�
		S_WINCE,//�Ђ��
		S_MAX
	};
	State EnemyState;

	XMVECTOR EnemyFrontDirection;//�G�̐��ʃx�N�g��
	XMVECTOR EnemyPosition;//�G�̈ʒu�x�N�g��
	float RotateY;//��]�p�x�i��{�x���̂݁j
	float FrontLength = 10.0f;//�G�̎��E�̒���
	float Fov;
	bool IsHit_;

	XMFLOAT3 pPosition;
	XMVECTOR pPositionVec;

	XMFLOAT3 ChasePoint;

	XMVECTOR sightLength;//�G�̎��E�i��^�̎��E�j�̒���

public:
	Enemy(GameObject* parent);
	~Enemy();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void UpdateIdle();
	void UpdateSuspicious();
	void UpdateChase();
	void UpdateAttack();
	void UpdateMove();
	void UpdateWince();

	void OnCollision(GameObject* pTarget) override;
	int GetModelHandle() { return hModel_Enemy; }
};

