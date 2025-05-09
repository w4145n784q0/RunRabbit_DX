#include "ClearScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/Audio.h"
#include"Engine/SceneManager.h"

ClearScene::ClearScene(GameObject* parent)
	:GameObject(parent,"ClearScene"), hImage_(-1),hClearSound_(-1)
{
}

void ClearScene::Initialize()
{
	hImage_ = Image::Load("Image\\ClearScreen.png");
	assert(hImage_ >= 0);
	hClearSound_ = Audio::Load("Sound\\maou_game_jingle05.wav");
	assert(hClearSound_ >= 0);
}

void ClearScene::Update()
{
	Audio::Play(hClearSound_);
	if (Input::IsKeyDown(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))//スペースキーを押したらゲームスタート
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
}

void ClearScene::Draw()
{
	Image::SetTransform(hImage_, transform_);
	Image::Draw(hImage_);
}

void ClearScene::Release()
{
}
