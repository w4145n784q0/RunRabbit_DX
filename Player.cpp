#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"

namespace {
	const float speed = 9.0f;
	const float Player_Gravity = 0.08; //0.16333f
	const float DeltaTime = 0.016f;
}

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),hModel_Player(-1),IsOnGround_(true),JumpSpeed_(0.0f),
	PlayerDirection({0,0,0}),Acceleration_(0.0f)
{
}

Player::~Player()
{
	Release();
}

void Player::Initialize()
{
	hModel_Player = Model::Load("ensui.fbx"); 
	transform_.position_ = { -20,0,-20};
	
	SphereCollider* col = new SphereCollider(XMFLOAT3(0,0,0),0.1f);
	this->AddCollider(col);
}

void Player::Update()
{
	if (Input::IsKey(DIK_UP))
	{
		PlayerDirection.z = 1.0;
		moveDir = Front;
	}
	if (Input::IsKey(DIK_DOWN))
	{
		PlayerDirection.z = -1.0;
		moveDir = Back;
	}
	if (Input::IsKey(DIK_LEFT))
	{
		PlayerDirection.x = -1.0;
		moveDir = Left;
		
	}
	if (Input::IsKey(DIK_RIGHT))
	{
		PlayerDirection.x = 1.0;
		moveDir = Right;
	}
	transform_.rotate_.y = MoveDirArray[moveDir];//�L�����̉�]�@�S�����̂�

	
	//--------------�_�b�V���֌W--------------
	if (Input::IsKey(DIK_LSHIFT) || Input::IsKey(DIK_RSHIFT)) 
	{
		IsDash_ = true;
	}
	else
	{
		IsDash_ = false;
	}
	Dash();
	

	XMVECTOR DirectionVec = XMVectorSet(PlayerDirection.x,PlayerDirection.y, PlayerDirection.z, 0.0f);
	DirectionVec = XMVector3Normalize(DirectionVec);// �P�ʃx�N�g���ɐ��K��
	XMVECTOR MoveVector = XMVectorScale(DirectionVec,(speed + Acceleration_) * DeltaTime);

	XMVECTOR PrevPos = XMVectorSet(transform_.position_.x,transform_.position_.y, transform_.position_.z, 0.0f);

	XMVECTOR NewPos = PrevPos + MoveVector;
	XMStoreFloat3(&transform_.position_, NewPos);

	PlayerDirection = { 0,0,0 };

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
	if (JumpSpeed_ < -100) {
		JumpSpeed_ = -100;
	}

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

void Player::OnCollision(GameObject* pTarget)
{

}

void Player::Dash()
{
	if (IsDash_)
	{
		Acceleration_ += 2.0;
		if (Acceleration_ > 50.0) {
			Acceleration_ = 50.0;
		}
	}
	else
	{
		Acceleration_ = 0;
	}
}
