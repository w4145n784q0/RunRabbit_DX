#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"

namespace {
	const float speed = 5.0f;
	const float Player_Gravity = 0.08; //0.16333f
	const float DeltaTime = 0.016f;
}

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),hModel_Player(-1),IsOnGround_(true),JumpSpeed_(0.0f)
{
}

Player::~Player()
{
	Release();
}

void Player::Initialize()
{
	hModel_Player = Model::Load("box.fbx"); transform_.position_ = { 0,0,0 };
	SphereCollider* col = new SphereCollider(XMFLOAT3(0,0,0),0.1f);
	this->AddCollider(col);
}

void Player::Update()
{
	if (Input::IsKey(DIK_LEFT))
	{
		transform_.position_.x -= 0.1;
	}

	if (Input::IsKey(DIK_RIGHT))
	{
		transform_.position_.x += 0.1;
	}

	if (Input::IsKey(DIK_UP))
	{
		transform_.position_.z += 0.1;
	}

	if (Input::IsKey(DIK_DOWN))
	{
		transform_.position_.z -= 0.1;
	}

	//--------------�W�����v(�����̂�)--------------
	if (Input::IsKeyDown(DIK_SPACE))
	{
		if (IsOnGround_) {
			IsOnGround_ = false;
			JumpSpeed_ = 1.5;//�ꎞ�I��y�����Ƀ}�C�i�X����Ă���l��傫������
		}
	}

	JumpSpeed_ -= Player_Gravity;//�d�͕��̒l�������A�v���C���[�͏�ɉ������ɗ͂��������Ă���
	transform_.position_.y += JumpSpeed_;

	if (transform_.position_.y <= 0.0f) {//�v���C���[�߂肱�ݖh�~�Ɉ��ȉ���y���W�Œl���Œ�
		transform_.position_.y = 0.0f;
		IsOnGround_ = true;
	}

	//--------------�J�����Ǐ]--------------
	XMFLOAT3 CameraPositionVec = { transform_.position_.x ,transform_.position_.y + 1, transform_.position_.z - 8 };
	XMFLOAT3 CameraTargetVec = { transform_.position_.x,transform_.position_.y, transform_.position_.z };

	Camera::SetPosition(CameraPositionVec);//�J�����̈ʒu���Z�b�g�i���͒Ǐ]���邾���j 
	Camera::SetTarget(CameraTargetVec);//�J�����̏œ_���Z�b�g�i���͒Ǐ]���邾���j

}

void Player::Draw()
{
	Model::SetTransform(hModel_Player, transform_);
	Model::Draw(hModel_Player);
}

void Player::Release()
{
}
