#pragma once
#include "Engine/GameObject.h"
#include"Engine/CsvReader.h"
#include<array>
#include<vector>

class Ground :
    public GameObject
{
private:
	//----------���f���n���h��----------

	//�n�ʃ��f��
	int hGrass_;
public:
	Ground(GameObject* parent);
	~Ground();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetSCV();

	int GetModelHandle() { return hGrass_; }
};