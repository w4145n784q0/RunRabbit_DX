#pragma once
#include "Engine/GameObject.h"
class HUD :
    public GameObject
{
private:
	//----------�摜�n���h��----------

	//�^�C�g���ɖ߂�(���K���[�h�p)
	int hBackTitleLogo_;

	//���K��(���K���[�h�p)
	int hPracticeNow_;

	

	enum GameMode
	{
		Battle,
		Practice,
		Max,
	};
	GameMode GameModeHUD_;

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

	void UpdateBattle();
	void UpdatePractice();

	void DrawBattle();
	void DrawPractice();

	void SetStateBattle() { GameModeHUD_ = Battle; }
	void SetStatePractice() { GameModeHUD_ = Practice; }

};

