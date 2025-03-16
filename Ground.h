#pragma once
#include "Engine/GameObject.h"
#include"Engine/CsvReader.h"
#include<array>
#include<vector>

using std::vector;

class Ground :
    public GameObject
{
	int hGrass_;

	///// <summary>
	///// csv�̃f�[�^��ۊ�
	///// </summary>
	//vector<vector<int>> MapData_;
	///// <summary>
	///// MapData�̍�������ۊ�
	///// </summary>
	//vector<vector<int>> MapHeight_;
	//int stageWidth_;
	//int stageHeight_;
	//CsvReader csv_;



public:
	Ground(GameObject* parent);
	~Ground();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	Point GetRandomMovePoint();
};