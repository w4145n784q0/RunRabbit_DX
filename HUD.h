#pragma once
#include "Engine/GameObject.h"
#include"array"

class HUD :
    public GameObject
{
private:
	//----------�摜�n���h��----------

	//�^�C�g���ɖ߂�(���K���[�h�p)
	int hBackTitleLogo_;

	//���K��(���K���[�h�p)
	int hPracticeNow_;

	//�v���C���[�̃X�R�A�X�v���C�g(�o�g�����[�h�p)
	int hPlayerScore_;

	//�G�̃X�R�A�X�v���C�g(�o�g�����[�h�p)
	int hEnemyScore_;

	//��������
	int hTime_;

	//�J�E���g�_�E��
	int hCountDown3_;
	int hCountDown2_;
	int hCountDown1_;

	//�J�E���g�_�E���n���h���̔z��
	std::array<int, 3> Array_Number_;

	//�t�B�j�b�V���̃��S
	int hFinish_;

	//std::array<int, 3> CountDownArray_;

	enum GameMode
	{
		BattlePreStart,
		BattleInProgress,
		BattleEnd,
		Practice,
		Max,
	};
	GameMode GameModeHUD_;

	int countDownNumber_;

public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	HUD(GameObject* parent);

	~HUD();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;


	void UpdateBattlePreStart();
	void UpdateBattleInProgress();
	void UpdateBattleEnd();
	void UpdatePractice();

	void DrawBattlePreStart();
	void DrawBattleInProgress();
	void DrawBattleEnd();
	void DrawPractice();

	void SetStateBattle() { GameModeHUD_ = BattlePreStart; }
	void SetStateBattleInProgress() { GameModeHUD_ = BattleInProgress; }
	void SetStateBattleEnd(){ GameModeHUD_ = BattleEnd; }
	void SetStatePractice() { GameModeHUD_ = Practice; }

	void SetNumber(int _num) { countDownNumber_ = _num; }

};

