#pragma once
#include"Engine/Text.h"
#include "Engine/GameObject.h"
#include"Player.h"
#include"Enemy.h"

class BattleScene :
    public GameObject
{
private:

	//�C���X�^���X
	Text* pTime_;
	Text* pPlayerScore_;
	Text* pEnemyScore_;

	enum Battle
	{
		BEFORE,
		NOW,
		AFTER,
		MAX
	};
	Battle BattleState;

	//�摜�n���h��

	//"finish"�̃��S
	int hFinish_;

	//�v���C���[��HP�摜
	int hPlayerLife_;

	//�G��HP�摜
	int hEnemyLife_;

	//���n���h��
	int hBattleSound_;
	int hWhistle_;

	std::array<Transform, 2> HUD_Trans_;
	//int PlayerLife_;
	//int EnemyLife_;

	//�Q�[���J�n�J�E���g
	int StartCount_;

	//��������
	int GameTime_;

	int PlayerScore_;
	int EnemyScore_;

public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	BattleScene(GameObject* parent);

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

	void DrawBattleBefore();
	void DrawBattle();
	void DrawBattleAfter();

	//void SetPlayerHp(int _hp) { PlayerLife_ = _hp; }

	void PlusPlayerScore() { PlayerScore_++; }
	void PlusEnemyScore() { EnemyScore_++; }

};

