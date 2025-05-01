#pragma once
#include "Engine/GameObject.h"
#include"Ground.h"
#include"Engine/VFX.h"

namespace {
	const XMFLOAT3 StartPosition = { 0.0f,0.5f,0.0f };
}

class Player :
    public GameObject
{
private:
	//���f���n���h��
	int hPlayer_;

	int hPlayerIdle_;//�ҋ@���[�V����
	int hPlayerMove_;//�ړ�
	int hPlayerDash_;//�_�b�V��
	int hPlayerHit_;//�U����H�����
	int hPlayerDead_;//�|���

	int hAttackArrow_;//�`���[�W���̖��

	//�T�E���h�֌W
	int hCollisionSound_;

	//VFX�֌W
	int hChargeEmit_;//�`���[�W�G�t�F�N�g
	int hHitEmit_;//�G�Ƃ̃q�b�g�G�t�F�N�g
	int hAttackEmitLocus_;//�U���G�t�F�N�g(�O��)
	int hAttackEmitAura_;//�U���G�t�F�N�g(�I�[��)

	//�C���X�^���X�֌W
	Ground* pGround_;

	//�v���C���[�X�e�[�g
	enum State
	{
		S_IDLE,//�ʏ�
		S_HIT,//�e�����
		S_CHARGE,//�`���[�W��
		S_ATTACK,//�U��
		S_WALLHIT,//�ǂɃq�b�g
		S_DEAD,//�|���
		S_MAX
	};
	State PlayerState_;

	enum CameraState
	{
		S_NORMALCAMERA,//�ʏ�J����
		S_DEBUGCAMERA,//�f�o�b�O�p�J����
		S_MAXCAMERA,
	};
	CameraState CameraState_;

	//�W�����v�֌W
	bool IsOnGround_;
	float JumpSpeed_;//+�Ȃ�W�����v���Ă����� -�Ȃ牺�~�`�n�ʂɂ�����


	//�_�b�V���֌W
	float Acceleration_;//�����x
	float AcceleValue_;//1f�ŉ����x�ɉ��Z�����
	
	//�ړ��֌W
	bool CanMove_;
	int PlayerHeight_;//�v���C���[�̍���
	XMFLOAT3 Direction_;//�v���C���[�̕���(xz�ǂ��ɐi�ނ�)
	XMVECTOR PlayerPosition_;//�ʒu�x�N�g��
	XMVECTOR NewPos_;//�v���C���[�̈ړ���
	XMFLOAT3 PlayerFront;//�v���C���[�̐��ʈʒu 
	XMVECTOR ForwardVector_;//�v���C���[�̐��ʃx�N�g��
	Transform ArrowTransform_;//��󃂃f���̈ʒu���

	//�J�����֌W
	XMFLOAT3 CameraPosition_;
	XMFLOAT3 CameraTarget_;
	Transform cameraTransform_;//�J������Transform ��]�����g��
	XMVECTOR BackCamera_;//�v���C���[�̌��ɒu���J�����̈ʒu

	//��e(HIT)�֌W
	float ColliderSize_;//�X�t�B�A�R���C�_�[�̃T�C�Y
	XMFLOAT3 KnockBack_Direction_;//�m�b�N�o�b�N�������
	XMFLOAT3 KnockBack_Velocity_;//�m�b�N�o�b�N���鑬�x

	//�_���[�W�֌W
	int CharacterLife_;//�L�����N�^�[��HP
	int deadTimer_;//�����܂ł̎���
	int InvincibilityTime_;//�_���[�W��̖��G����
	bool IsInvincibility_;//���G���Ԃ�

public:

	XMVECTOR GetPlayerDirection() { return ForwardVector_; }//�����x�N�g���擾
	XMVECTOR GetPlayerPosition() { return PlayerPosition_; }//�ʒu�x�N�g���擾

	Player(GameObject* parent);
	~Player();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;

	void UpdateIdle();
	void UpdateHit();
	void UpdateCharge();
	void UpdateAttack();
	void UpdateOut();
	void UpdateWallHit();
	void UpdateDead();
	void PlayerStop() { PlayerState_ = S_MAX; }
	
	/// <summary>
	/// �J�����̑���i�ꕔ�̃X�e�[�g�̂݁j
	/// </summary>
	void CameraControl();

	/// <summary>
	/// �G����͂�����鏈��
	/// </summary>
	/// <param name="_vector">�G����m�b�N�o�b�N�������(���K����)</param>
	/// <param name="_IsAttack">�G���U������</param>
	void EnemyReflect(XMVECTOR _vector, bool _IsAttack);

	//�����ʒu�ɖ߂�
	void SetStartPosition() { this->transform_.position_ = StartPosition; }

	bool IsAttackState() { if (PlayerState_ == S_ATTACK) return true; else return false; }

	//�`���[�W��ԃG�t�F�N�g������
	void SetChargeEffect();

	//�ˌ��G�t�F�N�g����
	void SetAttackAuraEffect();
	void SetAttackLocusEffect();

	//�Ռ��G�t�F�N�g����
	void SetHitEffect();

	//�q�b�g�G�t�F�N�g�𐔃t���[�������p�[�e�B�N���\��
	void HitEffectStop();
	
	//�O�ՃG�t�F�N�g�𐔃t���[�������p�[�e�B�N���\��
	void LocusEffectStop();

	void AuraEffectStop();

	void SetCharacterLife(int _hp) { CharacterLife_ = _hp; }
	int GetCharacterLife() { return CharacterLife_; }

	//void AttackEffectStop(int& EffectEndCount, bool& _IsEffect, int& VFXhandle);
};

