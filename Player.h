#pragma once
#include "Engine/GameObject.h"
#include"Ground.h"
class Player :
    public GameObject
{
private:
	int hModel_Player;
	int hcamera;
	bool IsOnGround_;
	float JumpSpeed_;//+�Ȃ�W�����v���Ă����� -�Ȃ牺�~�`�n�ʂɂ�����

	bool IsDash_;//�_�b�V���֌W �_�b�V������
	float Acceleration_;//�_�b�V���֌W�@�����x

	enum MoveDirection
	{
		Front = 0, Left,  Back, Right, None,
	};
	MoveDirection moveDir;
	int MoveDirArray[5] = { 0,270,180,90,0 };
	

	XMFLOAT3 Direction;//�v���C���[�̕���(xz�ǂ��ɐi�ނ�)
	XMVECTOR PlayerDirection;//�����x�N�g��(���ۂɌ����Ă������)
	XMVECTOR PlayerPosition;//�ʒu�x�N�g��
	//float RotateY;//��]�p�x�i��{�x���̂݁j

	Transform cameraTransform;//�J������Transform ��]�����g��
	XMVECTOR BackCamera;//�v���C���[�̌��ɒu���J�����̈ʒu
	
	XMFLOAT3 CameraPosition;
	XMFLOAT3 CameraTarget;

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
	void Dash();
};

