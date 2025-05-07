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
		S_ROOT,//���f�p
		S_CHASE,//�ǂ�������
		S_ATTACK,//�U��
		S_HITSTOP,//�q�b�g�X�g�b�v
		S_HIT,//�U����H�����
		S_WALLHIT,//�ǂɓ�����_���[�W
		S_AIM,//�v���C���[��_��(�U������)
		S_ONALEAT,//�l�q��

		S_MAX
	};
	State EnemyState_;

	//�ǐՊ֌W
	XMVECTOR ForwardVector_;//�G�̐��ʃx�N�g��
	//XMVECTOR EnemyFrontDirection_;
	XMVECTOR EnemyPosition_;//�G�̈ʒu�x�N�g��
	XMFLOAT3 PlayerPosition_;//�v���C���[�̈ʒu(���W)

	float RotateY_;//��]�p�x�i��{�x���̂݁j
	float FrontLength_;//�G�̎��E�̒���
	float Eye_;
	bool IsHit_;
	bool IsOnGround_;//�n�ʂɂ��邩
	float JumpSpeed_;//+�Ȃ�W�����v���Ă����� -�Ȃ牺�~�`�n�ʂɂ�����
	XMVECTOR pPositionVec_;//�v���C���[�̈ʒu��ۊ�

	//�q�b�g�֌W
	int CharacterLife_;//�L�����N�^�[��HP
	float ColliderSize_;//�X�t�B�A�R���C�_�[�̃T�C�Y
	XMFLOAT3 KnockBack_Direction_;//�m�b�N�o�b�N�������
	XMFLOAT3 KnockBack_Velocity_;//�m�b�N�o�b�N���鑬�x
	//�m�b�N�o�b�N���鑬�x= �m�b�N�o�b�N���鋭��(�萔) * �m�b�N�o�b�N�������
 
	//�ړ��֌W
	Point EnemyMovePoint_;//�G�̈ړ���
	float moveLengthX_;//�ړ�����
	float moveLengthZ_;//�ړ�����

	//�U���֌W
	int AimTimer_;//�_���Ă���U���܂ł̃^�C�}�[
	XMVECTOR AttackVector_;//�U������
	float Acceleration_;//�����x
	float AcceleValue_;//1f�ŉ����x�ɉ��Z�����

	//�_���[�W�֌W
	int HitStopTimer_;//�q�b�g�X�g�b�v����
	int deadTimer_;//�����܂ł̎���
	int InvincibilityTime_;//�_���[�W��̖��G����
	bool IsInvincibility_;//���G���Ԃ�

public:
	Enemy(GameObject* parent);
	~Enemy();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void UpdateIdle();//�ҋ@(�f�o�b�O�p)
	void UpdateRoot();
	void UpdateChase();//�ǐ�
	void UpdateAttack();//�U��
	void UpdateHitStop();//�q�b�g�X�g�b�v
	void UpdateHit();//�U����������
	void UpdateWallHit();//�ǂɐڐG
	void UpdateAim();//�v���C���[��_���Ă���
	void UpdateOnAlert();//�l�q��
	//void UpdateRandomAim();
	void Blown();

	void OnCollision(GameObject* pTarget) override;

	//�G�Ɉړ�������
	void EnemyStart() { EnemyState_ = S_ROOT; }

	//�G���~�߂�
	void EnemyStop() { EnemyState_ = S_MAX; }

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

	void LookPlayer();

	//�G�ƃv���C���[�̋�����Ԃ�
	XMFLOAT3 PlayerEnemyDistanceFloat3();
	float PlayerEnemyDistanceX();

	int GetCharacterLife() { return CharacterLife_; }
};

