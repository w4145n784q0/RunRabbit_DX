#pragma once
#include "Engine/GameObject.h"
#include"Ground.h"
#include"Engine/VFX.h"
#include"Engine/CsvReader.h"

namespace {
	const XMFLOAT3 StartPosition = { 0.0f,0.5f,0.0f };
}

class Player :
    public GameObject
{
private:
	//----------���f���n���h��----------

	//�v���C���[���f��
	int hPlayer_;

	//�`���[�W���̖��
	//int hAttackArrow_;

	//----------�T�E���h�n���h��----------

	//�ڐG��
	int hCollisionSound_;

	//----------�C���X�^���X�֌W----------
	Ground* pGround_;

	//----------�v���C���[�X�e�[�g----------
	enum State
	{
		S_IDLE,//�ʏ�
		S_HIT,//�e�����
		S_CHARGE,//�`���[�W��
		S_ATTACK,//�U��
		S_WALLHIT,//�ǂɃq�b�g
		S_MAX
	};
	State PlayerState_;

	//----------�J�����X�e�[�g----------
	enum CameraState
	{
		S_NORMALCAMERA,//�ʏ�J����
		S_DEBUGCAMERA,//�f�o�b�O�p�J����
		S_ROCKONCAMERA,//�G�𒍎�����J����
		S_MAXCAMERA,
	};
	CameraState CameraState_;

	//----------�W�����v�֌W----------
	bool IsOnGround_;//�n�ʂɂ��邩
	float JumpSpeed_;//+�Ȃ�W�����v���Ă����� -�Ȃ牺�~�`�n�ʂɂ�����


	//----------�U���֌W----------
	float Acceleration_;//�����x
	float AcceleValue_;//1f�ŉ����x�ɉ��Z�����
	
	//----------�ړ��֌W----------
	XMVECTOR PlayerPosition_;//�ʒu�x�N�g��(���t���[���ۑ�)
	XMVECTOR ForwardVector_;//�v���C���[�̐��ʃx�N�g��
	XMVECTOR NewPos_;//�v���C���[�̈ړ��� 
	XMFLOAT3 Direction_;//�v���C���[�̐i�s����(xz�ǂ��ɐi�ނ�)
	Transform ArrowTransform_;//��󃂃f���̈ʒu���
	
	//----------�J�����֌W----------
	XMFLOAT3 CameraPosition_;//�J�����ʒu
	XMFLOAT3 CameraTarget_;//�J���������_
	Transform cameraTransform_;//�J������Transform ��]�����g��
	XMVECTOR BackCamera_;//�v���C���[�̌��ɒu���J�����̈ʒu

	//----------��e(HIT)�֌W----------
	float ColliderSize_;//�X�t�B�A�R���C�_�[�i�����蔻��j�̃T�C�Y
	XMFLOAT3 KnockBack_Direction_;//�m�b�N�o�b�N�������
	XMFLOAT3 KnockBack_Velocity_;//�m�b�N�o�b�N���鑬�x

	//----------�_���[�W�֌W----------
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
	void UpdateWallHit();

	//�v���C���[�Ɉړ�������
	void PlayerStart() { PlayerState_ = S_IDLE; }

	//�v���C���[���~�߂�
	void PlayerStop() { PlayerState_ = S_MAX; }
	
	//�v���C���[�̔\���I�ړ�
	void PlayerMove();

	//�v���C���[���͂�����铮��
	void Blown();

	//�v���C���[�̏d��
	void Gravity();

	//�J�����̑���i�ꕔ�̃X�e�[�g�̂݁j
	void CameraControl();

	//�J�����̐ݒ�(���t���[���Ăяo��)
	void CameraUpdate();



	void NormalCamera();
	void DebugCamera();
	void RockOnCamra();

	/// <summary>
	/// �G����͂�����鏈��
	/// </summary>
	/// <param name="_vector">�G����m�b�N�o�b�N�������(���K����)</param>
	/// <param name="_IsAttack">�G���U������</param>
	void EnemyReflect(XMVECTOR _vector, bool _IsAttack);

	//�����ʒu�ɖ߂�
	void SetStartPosition() { 

		XMFLOAT3 tmp = { 0,0,0 };
		this->transform_.position_ = tmp;
	}

	//�U�������ǂ����擾(�ڐG����Ɏg�p)
	bool IsAttackState() { if (PlayerState_ == S_ATTACK) return true; else return false; }

	bool GetIsInvincibility() { if (IsInvincibility_) return true; else return false; }

	//�`���[�W��ԃG�t�F�N�g������
	void SetChargingEffect();

	//�ˌ��G�t�F�N�g����
	void SetAttackLocusEffect();

	//�Ռ��G�t�F�N�g����
	void SetHitEffect();

};

