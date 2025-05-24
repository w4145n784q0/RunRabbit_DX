#pragma once
#include "Engine/GameObject.h"
class Fence :
    public GameObject
{
private:
	//----------���f���n���h��----------

	//�����f��
	int hPiller_;

	//�򃂃f��
	int hFence_;

	//----------��̈ʒu----------

	float NorthEnd_;
	float SouthEnd_;
	float EastEnd_;
	float WestEnd_;

	XMFLOAT3 piller_UpperLeft_;
	XMFLOAT3 piller_UpperRight_;
	XMFLOAT3 piller_LowerLeft_;
	XMFLOAT3 piller_LowerRight_;
	

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

	void SetSCV();
};

