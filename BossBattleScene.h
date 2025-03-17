#pragma once
#include"Engine/Text.h"
#include "Engine/GameObject.h"
class BossBattleScene :
    public GameObject
{
private:

	enum Battle
	{
		BEFORE,
		NOW,
		AFTER,
		MAX
	};
	Battle BattleState;

	//�摜�n���h��
	int hWin_;
	int hLose_;
	int hFinish_;

	//���n���h��
	int hBattleSound_;
	int hWhistle_;

	int Phase_;
	int deadCount_;
	Text* pText_;
	Text* pText2_;

	bool IsWin_;
public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	BossBattleScene(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	void UpdateBattleBefore();
	void UpdateBattle();
	void UpdateBattleAfter();

	void DeadCountPlus() { deadCount_--; }
	void PhasePlus() { Phase_++; }
};

