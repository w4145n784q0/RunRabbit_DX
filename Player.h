#pragma once
#include "Engine/GameObject.h"
#include"Ground.h"
#include"StageObject.h"

class Player :
    public GameObject
{
private:
	//���f���n���h��
	int hModel_Player;
	int hModel_LandingPoint;
	int hGetGrass;
	int hGetWall;

	//�X�e�[�g
	enum State
	{
		S_IDLE,
		S_HIDE,
		S_JUMPBEFORE,
		S_JUMP,
		S_HIT,
	};
	State PlayerState;

	//�W�����v�֌W
	bool IsOnGround_;
	float JumpSpeed_;//+�Ȃ�W�����v���Ă����� -�Ȃ牺�~�`�n�ʂɂ�����
	//XMFLOAT3 JumpDirection;//�ړ������i�x�N�g���v�Z�p�j

	Transform JumpTransform_;//���n�ʒu�iTransform�j
	XMFLOAT3 LandingPoint;//���n�ʒu�iXMFLOAT3�j

	//�_�b�V���֌W
	bool IsDash_; //�_�b�V������
	float Acceleration_;//�����x
	
	//�ړ��֌W
	XMFLOAT3 Direction;//�v���C���[�̕���(xz�ǂ��ɐi�ނ�)
	XMVECTOR PlayerFrontDirection;//���ʃx�N�g��
	XMVECTOR PlayerPosition;//�ʒu�x�N�g��

	Transform StartPosition;//�J�n�ʒu
	XMVECTOR NewPos;//�v���C���[�̈ړ���

	//�J�����֌W
	XMFLOAT3 CameraPosition;
	XMFLOAT3 CameraTarget;
	Transform cameraTransform;//�J������Transform ��]�����g��
	XMVECTOR BackCamera;//�v���C���[�̌��ɒu���J�����̈ʒu

	//�C���X�^���X�֌W
	Ground* pGround;
	StageObject* pStageObject;

	//���I�u�W�F�N�g�֌W
	bool IsHitWall;

public:

	XMVECTOR GetPlayerDirection() { return PlayerFrontDirection; }//�����x�N�g���擾
	XMVECTOR GetPlayerPosition() { return PlayerPosition; }//�ʒu�x�N�g���擾

	Player(GameObject* parent);
	~Player();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;

	void UpdateIdle();
	void UpdateHide();
	void UpdateJumpBefore();
	void UpdateJump();
	void UpdateHit();

	/// <summary>
	/// �n�ʂɌ������ă��C���΂��i�K�v�ȂƂ������j
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	void PlayerRayCast(int handle);

	void Dash();

	/// <summary>
	/// �J�����̑���i�ꕔ�̃X�e�[�g�̂݁j
	/// </summary>
	void CameraControl();

	void RideObject();
};

