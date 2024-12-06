#pragma once
#include "Engine/GameObject.h"
class Player :
    public GameObject
{
private:
	int hModel_Player;
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
	

	XMFLOAT3 PlayerDirection;
public:
	Player(GameObject* parent);
	~Player();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void Dash();
};

