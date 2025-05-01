#pragma once
#include"Engine/Text.h"
#include "Engine/GameObject.h"
#include"Player.h"

class BattleScene :
    public GameObject
{
private:

	//�C���X�^���X
	Player* pPlayer_;

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
	int hLife_;

	//���n���h��
	int hBattleSound_;
	int hWhistle_;

	bool IsWin_;

	std::array<Transform, 2> HUD_Trans_;
	int PlayerLife_;
	int EnemyLife_;
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

	void SetPlayerHp(int _hp) { PlayerLife_ = _hp; }

};

