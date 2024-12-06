#pragma once
#include "Engine/GameObject.h"
class Player :
    public GameObject
{
private:
	int hModel_Player;
	bool IsOnGround_;
	float JumpSpeed_;//+�Ȃ�W�����v���Ă����� -�Ȃ牺�~�`�n�ʂɂ�����
public:
	Player(GameObject* parent);
	~Player();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

