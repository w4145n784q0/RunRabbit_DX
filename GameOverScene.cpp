#include "GameOverScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"

GameOverScene::GameOverScene(GameObject* parent)
	:GameObject(parent,"GameOverScene"), hImage_(-1)
{
}

void GameOverScene::Initialize()
{
	hImage_ = Image::Load("gameover.png");
}

void GameOverScene::Update()
{
	if (Input::IsKeyDown(DIK_P))//�X�y�[�X�L�[����������Q�[���X�^�[�g
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
}

void GameOverScene::Draw()
{
	Image::SetTransform(hImage_, transform_);
	Image::Draw(hImage_);
}

void GameOverScene::Release()
{
}
