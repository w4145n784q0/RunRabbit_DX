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
		S_HIT,//�U����H�����
		S_WALLHIT,//�ǂɓ�����_���[�W
		S_AIM,//�v���C���[��_��

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
	XMFLOAT3 KnockBack_Direction_;//�m�b�N�o�b�N�������
	XMFLOAT3 KnockBack_Velocity_;//�m�b�N�o�b�N���鑬�x
	//�m�b�N�o�b�N���鑬�x= �m�b�N�o�b�N���鋭��(�萔) * �m�b�N�o�b�N�������
 
	//�ړ��֌W
	bool isStop_;
	Point EnemyMovePoint_;//�G�̈ړ���
	float moveLengthX_;//�ړ�����
	float moveLengthZ_;//�ړ�����
	float distance;

	//�U���֌W
	XMVECTOR AttackVector_;//�U������
	float Acceleration_;//�����x

	//�_���[�W�֌W
	int deadTimer_;//�����܂ł̎���
	int InvincibilityTime_;//�_���[�W��̖��G����
	bool IsInvincibility_;//���G���Ԃ�

	//�^�C�}�[
	int MoveTimer_;
	int AimTimer_;

public:
	Enemy(GameObject* parent);
	~Enemy();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void UpdateIdle();//�ҋ@(�f�o�b�O�p)
	void UpdateChase();
	void UpdateHit();//�U����������
	void UpdateWallHit();//�ǂɐڐG
	void UpdateAim();//�v���C���[��_���Ă���
	//void UpdateRandomAim();
	void UpdateAttack();//�U��

	void OnCollision(GameObject* pTarget) override;

	/// <summary>
	/// �v���C���[����͂�����鏈��
	/// </summary>
	/// <param name="_vector">�v���C���[����n���ꂽ�m�b�N�o�b�N�������(���K����)</param>
	/// <param name="_isDush">�v���C���[���_�b�V������</param>
	void PlayerReflect(XMVECTOR _vector, bool _isDush);
	
	void SetState(State s) { EnemyState_ = s; }
	State GetState() { return EnemyState_; }
	bool GetStateAttack() { if (EnemyState_ == S_ATTACK) return true; else return false; }

	void SetStateStop() { EnemyState_ = S_MAX; }
};

