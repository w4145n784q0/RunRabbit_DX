#pragma once
#include "Engine/GameObject.h"
#include"Player.h"
#include"Ground.h"
class Player;

class Enemy :
    public GameObject
{
private:
	//���f���n���h��
    int hEnemy_;

	//�C���X�^���X�֌W
	Player* pPlayer_;
	Ground* pGround_;

	//�X�e�[�g
	enum State {
		S_IDLE = 0,//�ҋ@
		S_CHASE,//�ǂ�������
		S_ATTACK,//�U��
		S_MOVE,//�ړ�
		S_HIT,//�U����H�����
		S_MAX
	};
	State EnemyState_;

	//�ǐՊ֌W
	XMVECTOR EnemyFrontDirection_;//�G�̐��ʃx�N�g��
	XMVECTOR EnemyPosition_;//�G�̈ʒu�x�N�g��
	float RotateY_;//��]�p�x�i��{�x���̂݁j
	float FrontLength_;//�G�̎��E�̒���
	float Eye_;
	bool IsHit_;
	bool IsOnGround_;//�n�ʂɂ��邩
	float JumpSpeed_;//+�Ȃ�W�����v���Ă����� -�Ȃ牺�~�`�n�ʂɂ�����
	XMVECTOR pPositionVec_;//�v���C���[�̈ʒu��ۊ�
	XMFLOAT3 ChasePoint_;

	//�q�b�g�֌W
	XMFLOAT3 ReflectMove;

	//�ړ��֌W
	bool isStop_;
	Point EnemyMovePoint_;//�G�̈ړ���
	float moveLengthX_;//�ړ�����
	float moveLengthZ_;//�ړ�����
	float distance;

	//�^�C�}�[
	int MoveTimer_;

public:
	Enemy(GameObject* parent);
	~Enemy();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void UpdateIdle();
	void UpdateChase();
	void UpdateHit();
	void UpdateMove();

	void OnCollision(GameObject* pTarget) override;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="_vector">�v���C���[����n���ꂽ�m�b�N�o�b�N�������(���K����)</param>
	void PlayerReflect(XMVECTOR _vector);

	void SetState(State s) { EnemyState_ = s; }
	State GetState() { return EnemyState_; }
};

