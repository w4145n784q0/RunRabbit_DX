#pragma once
#include "Engine/GameObject.h"
class GameModeScene :
    public GameObject
{
private:
	int hBack_;//�w�i
	int hBossMode_;//�{�X���[�h�̃{�^��
	int hRushMode_;//���b�V�����[�h�̃{�^��
	int hBackTitle_;//�^�C�g���̃{�^��
	int hArrow_;//�I���̖��
	int hModeSelect_;//���[�h�Z���N�g

	int hModeSound_;

	enum Mode
	{
		Boss,
		Rush,
		Title,
		Max
	};
	Mode SelectMode_;

	Transform Trans_BossMode_;
	Transform Trans_RushMode_;
	Transform Trans_Title_;
	Transform Trans_Arrow_;
	Transform Trans_Mode_;
public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	GameModeScene(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;
};

