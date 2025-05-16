#pragma once
#include "Engine/GameObject.h"
#include<array>
#include<list>
class GameModeScene :
    public GameObject
{
private:
	int hBack_;//�w�i
	int hBattle_;//�o�g���X�^�[�g�{�^��
	int hPractice_;//���K���[�h�{�^��
	int hHowtoPlay_;//�����т��������{�^��
	int hBackTitle_;//�^�C�g���̃{�^��
	int hFrameLine_;//�g��

	int hArrow_;//�I���̖��
	int hModeSelect_;//���[�h�Z���N�g

	int hModeSound_;

	enum Mode
	{
		Battle,
		Practice,
		HowToPlay,
		Title,
		Max
	};
	Mode SelectMode_;
	std::list<Mode> ModeList_;//�e���[�h�̃��X�g
	std::list<Mode>::iterator itr;//ModeList_�̃C���f�b�N�X���w���l
	

	Transform Trans_Arrow_;
	std::array<Transform, 5> ModeSetTrans;
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

