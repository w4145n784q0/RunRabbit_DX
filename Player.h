#pragma once
#include "Engine/GameObject.h"
#include"Ground.h"
#include"Terrain.h"
#include"Tree.h"
#include"TreeManager.h"

class Player :
    public GameObject
{
private:
	//���f���n���h��
	int hPlayer_;
	int hLandingPoint_;

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
	float PrevHeight_;//���̍�����ۊǂ���
	//XMFLOAT3 JumpDirection;//�ړ������i�x�N�g���v�Z�p�j
	XMVECTOR JumpTarget_;
	XMVECTOR JumpLength_;

	XMFLOAT3 JumpValue;

	Transform JumpTransform_;//���n�ʒu�iTransform�j
	XMFLOAT3 LandingPoint;//���n�ʒu�iXMFLOAT3�j

	//�_�b�V���֌W
	bool IsDash_; //�_�b�V������
	float Acceleration_;//�����x
	
	//�ړ��֌W
	bool CanMove_;
	XMFLOAT3 Direction_;//�v���C���[�̕���(xz�ǂ��ɐi�ނ�)
	XMVECTOR PlayerFrontDirection_;//���ʃx�N�g��
	XMVECTOR PlayerPosition_;//�ʒu�x�N�g��
	XMVECTOR PlayerStart_;//�v�Z�p�@�v���C���[�̎n�_

	XMVECTOR NewPos_;//�v���C���[�̈ړ���

	//�B���֌W
	bool CanHide_;

	//�J�����֌W
	XMFLOAT3 CameraPosition_;
	XMFLOAT3 CameraTarget_;
	Transform cameraTransform_;//�J������Transform ��]�����g��
	XMVECTOR BackCamera_;//�v���C���[�̌��ɒu���J�����̈ʒu

	//�C���X�^���X�֌W
	Ground* pGround_;
	Terrain* pTerrain_;
	Tree* pTree_;
	TreeManager* pTreeManager_;
	const vector<Tree>* TreesVector_;

	//���I�u�W�F�N�g�֌W
	bool IsHitWall;

public:

	XMVECTOR GetPlayerDirection() { return PlayerFrontDirection_; }//�����x�N�g���擾
	XMVECTOR GetPlayerPosition() { return PlayerPosition_; }//�ʒu�x�N�g���擾

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

	void LandGround();

	bool IsNearTree(const XMFLOAT3& pos);

};

