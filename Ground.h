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

	/// <summary>
	/// csv�̃f�[�^��ۊ�
	/// </summary>
	vector<vector<int>> MapData_;

	/// <summary>
	/// MapData�̍�������ۊ�
	/// </summary>
	vector<vector<int>> MapHeight_;


	int stageWidth_;
	int stageHeight_;

	CsvReader csv_;



public:
	Ground(GameObject* parent);
	~Ground();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	int GetModelHandle() { return hGrass_; }

	/// <summary>
	/// �v���C���[����ɍs���邩�Ԃ�(csv�I�[�o�[��h������)
	/// </summary>
	/// <param name="x"></param>
	/// <param name="z"></param>
	/// <param name="height"></param>
	/// <returns></returns>
	bool CanMoveFront(int x, int z);

	int GetStageWidth() { return stageWidth_; }
	int GetStageHeight() { return stageHeight_; }
};