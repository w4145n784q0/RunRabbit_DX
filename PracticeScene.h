#pragma once
#include "Engine/GameObject.h"
class PracticeScene :
    public GameObject
{
private:
	int Press_;
public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	PracticeScene(GameObject* parent);

	~PracticeScene();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;
};

