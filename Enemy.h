#pragma once
#include "Engine/GameObject.h"
#include"Player.h"

class Player;

class Enemy :
    public GameObject
{
private:
    int hEnemy_;
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
	State EnemyState_;

	XMVECTOR EnemyFrontDirection_;//�G�̐��ʃx�N�g��
	XMVECTOR EnemyPosition_;//�G�̈ʒu�x�N�g��
	float RotateY_;//��]�p�x�i��{�x���̂݁j
	float FrontLength_;//�G�̎��E�̒���
	float Eye_;
	bool IsHit_;

	XMFLOAT3 pPosition_;
	XMVECTOR pPositionVec_;

	XMFLOAT3 ChasePoint_;

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
};

