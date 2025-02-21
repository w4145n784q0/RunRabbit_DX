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

	//enum Type {
	//	GRASS = 0,//�ʏ�@�ړ��ł���
	//	NOENTRY, //����Ȃ�
	//};
	//Type Type_;


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

	int GetModelHandle() { return hGrass_; }

	void TerrainSet();

	/// <summary>
	/// �A�C�e����M�~�b�N��ݒu(�n�`���Z�b�g������KO)
	/// </summary>
	void ObjectSet();

	/// <summary>
	/// �v���C���[����ɍs���邩�Ԃ�(csv�I�[�o�[��h������)
	/// </summary>
	/// <param name="x"></param>
	/// <param name="z"></param>
	/// <param name="height"></param>
	/// <returns></returns>
	bool CanMoveFront(int x, int z);

	/// <summary>
	/// �i�s�s�G���A���ǂ������f���v���C���[����ɍs���邩�Ԃ�
	/// </summary>
	/// <param name="x"></param>
	/// <param name="z"></param>
	/// <returns></returns>
	bool CanNoEntrySpace(int x, int z);

	/// <summary>
	/// �ړ����ƈړ�����r���v���C���[����ɍs���邩�Ԃ�
	/// </summary>
	/// <param name="prev">�ړ����̍���</param>
	/// <param name="nextX">�ړ����x���W</param>
	/// <param name="nextZ">�ړ����z���W</param>
	/// <returns></returns>
	bool CompareHeight(int prev, int nextX, int nextZ);

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