#pragma once
#include "Engine/GameObject.h"
class Fence :
    public GameObject
{
private:
	int hFence_;
public:
	Fence(GameObject* parent);
	~Fence();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	//�����ɓ�������
	//�����FpTarget ������������
	void OnCollision(GameObject* pTarget) override;
};

