#pragma once
#include "Engine/GameObject.h"
#include"Ground.h"
#include"Fence.h"

class StageManager :
    public GameObject
{
private:
	Transform GroundData_;//�n�ʃN���X�ɓn���f�[�^
	Transform WireData_;//��N���X(�S��)�ɓn���f�[�^
	Transform PillerData_;//��N���X(��)�ɓn���f�[�^

	float NorthEnd_;//�X�e�[�W�k�[
	float SouthEnd_; //�X�e�[�W��[
	float EastEnd_;//�X�e�[�W���[
	float WestEnd_;//�X�e�[�W���[

public:
	StageManager(GameObject* parent);
	~StageManager();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetSCV();

	void InitGroundData();
	void InitFenceData();
	void InitEndData();

	float GetNorthEnd() { return NorthEnd_; }
	float GetSouthEnd() { return SouthEnd_; }
	float GetEastEnd() { return EastEnd_; }
	float GetWestEnd() { return WestEnd_; }
};

