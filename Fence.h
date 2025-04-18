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
	//何かに当たった
	//引数：pTarget 当たった相手
	void OnCollision(GameObject* pTarget) override;
};

