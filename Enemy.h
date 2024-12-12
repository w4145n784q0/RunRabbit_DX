#pragma once
#include "Engine/GameObject.h"
#include"Player.h"

class Enemy :
    public GameObject
{
private:
    int hModel_Enemy;
	Player pPlayer;
	enum EnemyState {
		S_IDLE = 0,//�ҋ@
		S_SUSPICIOUS,//������
		S_CHASE,//�ǂ�������
		S_ATTACK,//�U��
		S_MOVE,//�ړ�
		S_WINCE,//�Ђ��
		S_MAX
	};
	EnemyState enemy_state;

	XMVECTOR FrontVector = {0,0,1};//�G�̊�{�x�N�g��
	XMVECTOR EnemyDirection;//�G�̕����x�N�g��
	XMVECTOR EnemyPosition;//�G�̈ʒu�x�N�g��
	float RotateY;//��]�p�x�i��{�x���̂݁j
	bool IsTargetted;//�G�̎��E�ɓ�������
	float FrontLength = 5.0f;//�G�̎��E�̒���


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
	bool ComparePosition(XMFLOAT3 pos);
};

