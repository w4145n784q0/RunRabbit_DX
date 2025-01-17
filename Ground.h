#pragma once
#include "Engine/GameObject.h"
#include"Engine/CsvReader.h"
#include<array>
#include<vector>

#include"Ground.h"

using std::vector;

class Ground :
    public GameObject
{
	int hModel_Ground;
	int hModel_Grass;
	int hModel_Wall;
	int hModel_Hole;

	/*struct StageData {
		int height;
		int type;
	};
	StageData stageTable[20][20];
	std::array<int, 2> blockArray;*/

	/// <summary>
	/// csv�̃f�[�^��ۊ�
	/// </summary>
	vector<vector<int>> MapData;

	/// <summary>
	/// MapData�̍�������ۊ�
	/// </summary>
	vector<vector<int>> MapHeight;
	int stageWidth_;
	int stageHeight_;
	Transform mapTrans;

	//�n�`�̈ʒu��ۊǂ���ϐ�
	Transform ObjectTrans;

	CsvReader csv;

public:
	Ground(GameObject* parent);
	~Ground();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;

	void ObjectSet();
	/// <summary>
	/// �v���C���[����ɍs���邩�Ԃ�
	/// </summary>
	/// <param name="x"></param>
	/// <param name="z"></param>
	/// <param name="height"></param>
	/// <returns></returns>
	bool CanMoveFront(int x, int z,int height);

	int GetGrassHandle() { return hModel_Ground; }
	int GetWallHandle() { return hModel_Wall; }

	/// <summary>
	/// �Q�[�����y���W���Ƃ�
	/// </summary>
	/// <param name="x">�n���ꂽx���W</param>
	/// <param name="z">�n���ꂽz���W</param>
	/// <returns>csv�̒l���Ƃ�</returns>
	int GetPositionData(int x, int z);

	int GetStageWidth() { return stageWidth_; }
	int GetStageHeight() { return stageHeight_; }

	Transform GetObjectTrans() { return ObjectTrans; }
};

