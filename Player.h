#pragma once
#include "Engine/GameObject.h"
#include"Ground.h"

namespace {
	const XMFLOAT3 StartPosition = { 0.0f,0.5f,0.0f };
}

class Player :
    public GameObject
{
private:
	//���f���n���h��
	int hPlayer_;
	int hLandingPoint_;
	int hNextPoint_;

	//���n���h��
	int hCollisionSound_;

	//�C���X�^���X�֌W
	Ground* pGround_;

	//�X�e�[�g
	enum State
	{
		S_IDLE,
		S_JUMP,
		S_HIT,
		S_CHARGE,
		S_ATTACK,
		S_OUT,
		S_DEAD,
		S_MAX
	};
	State PlayerState_;

	//�W�����v�֌W
	bool IsOnGround_;
	float JumpSpeed_;//+�Ȃ�W�����v���Ă����� -�Ȃ牺�~�`�n�ʂɂ�����


	//�_�b�V���֌W
	bool IsDash_; //�_�b�V������
	bool IsDashStart_ = false;//�_�b�V���J�n������
	float Acceleration_;//�����x
	
	//�ړ��֌W
	bool CanMove_;//�ړ��ł��邩
	int PlayerHeight_;//�v���C���[�̍���
	XMFLOAT3 Direction_;//�v���C���[�̕���(xz�ǂ��ɐi�ނ�)
	XMVECTOR PlayerPosition_;//�ʒu�x�N�g��
	XMVECTOR NewPos_;//�v���C���[�̈ړ���
	XMFLOAT3 PlayerFront;//�v���C���[�̐��ʈʒu �����蔻��p
	XMVECTOR ForwardVector_;//�v���C���[�̐��ʃx�N�g��

	//�J�����֌W
	XMFLOAT3 CameraPosition_;
	XMFLOAT3 CameraTarget_;
	Transform cameraTransform_;//�J������Transform ��]�����g��
	XMVECTOR BackCamera_;//�v���C���[�̌��ɒu���J�����̈ʒu

	//�^�C�}�[
	int deadTimer_;//�����܂ł̎���

	//�����֌W

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
	//void UpdateJump();
	void UpdateHit();
	void UpdateCharge();
	void UpdateAttack();
	void UpdateOut();
	void UpdateDead();
	void Dash();
	void PlayerStop() { PlayerState_ = S_MAX; }
	
	/// <summary>
	/// �J�����̑���i�ꕔ�̃X�e�[�g�̂݁j
	/// </summary>
	void CameraControl();

	void EnemyReflect(XMVECTOR _vector);

	void SetStartPosition() { this->transform_.position_ = StartPosition; }
};

