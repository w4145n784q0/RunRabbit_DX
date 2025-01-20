#pragma once
#include "Engine/GameObject.h"
#include"Engine/CsvReader.h"
#include<array>
#include<vector>

#include"Tree.h"

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
	Transform mapTrans_;


	CsvReader csv_;

public:
	Ground(GameObject* parent);
	~Ground();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;

	void TerrainSet();
	void ObjectSet();

	/// <summary>
	/// �v���C���[����ɍs���邩�Ԃ�
	/// </summary>
	/// <param name="x"></param>
	/// <param name="z"></param>
	/// <param name="height"></param>
	/// <returns></returns>
	bool CanMoveFront(int x, int z,int height);

	/// <summary>
	/// �Q�[�����y���W���Ƃ�
	/// </summary>
	/// <param name="x">�n���ꂽx���W</param>
	/// <param name="z">�n���ꂽz���W</param>
	/// <returns>csv�̒l���Ƃ�</returns>
	int GetPositionData(int x, int z);

	int GetStageWidth() { return stageWidth_; }
	int GetStageHeight() { return stageHeight_; }
};