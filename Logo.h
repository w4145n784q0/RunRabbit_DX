#pragma once
#include "Engine/GameObject.h"
class Logo :
    public GameObject
{
private:
	enum SetLogo
	{
		S_FINISH,
		S_MAX
	};
	SetLogo LogoState;

	int hFinish_;

	Transform trans_finish_;
public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	Logo(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	void SetFinish() { LogoState = S_FINISH; }
	void SetMax() { LogoState = S_MAX; }
};

