#pragma once
#include "Engine/GameObject.h"
#include"Ground.h"
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
		S_Normal,
		S_Hide,
		S_Jump,
		S_Hit,
	};
	State PlayerState;

	//�W�����v�֌W
	bool IsOnGround_;
	float JumpSpeed_;//+�Ȃ�W�����v���Ă����� -�Ȃ牺�~�`�n�ʂɂ�����
	XMFLOAT3 JumpDirection;//�ړ������i�x�N�g���v�Z�p�j
	XMVECTOR MovePoint;
	XMVECTOR LandingPoint; //���n�_
	Transform JumpTrans;//

	//�_�b�V���֌W
	bool IsDash_; //�_�b�V������
	float Acceleration_;//�����x
	
	//�ړ��֌W
	XMFLOAT3 Direction;//�v���C���[�̕���(xz�ǂ��ɐi�ނ�)
	XMVECTOR PlayerDirection;//�����x�N�g��(���ۂɌ����Ă������)
	XMVECTOR PlayerPosition;//�ʒu�x�N�g��

	Transform cameraTransform;//�J������Transform ��]�����g��
	XMVECTOR BackCamera;//�v���C���[�̌��ɒu���J�����̈ʒu
	
	Transform StartPosition;//�J�n�ʒu
	XMVECTOR NewPos;//�v���C���[�̈ړ���

	//�J�����֌W
	XMFLOAT3 CameraPosition;
	XMFLOAT3 CameraTarget;



	//�C���X�^���X�֌W
	Ground* pGround;

public:

	XMVECTOR GetPlayerDirection() { return PlayerDirection; }//�����x�N�g���擾
	XMVECTOR GetPlayerPosition() { return PlayerPosition; }//�ʒu�x�N�g���擾

	Player(GameObject* parent);
	~Player();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;

	void UpdateNormal();
	void UpdateHide();
	void UpdateJump();
	void UpdateHit();

	void PlayerRayCast(int handle);

	void Dash();
};

