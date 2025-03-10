#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

#include"Enemy.h"

#include <algorithm>
#include<list>

namespace {
	const float speed = 9.0f;
	const float Player_Gravity = 0.08; //0.16333f
	const float DeltaTime = 0.016f;
	const float FullAccelerate = 50.0f;
	XMVECTOR BackCameraPos = { 0,2,-10,0 };//BackCamera�̒l�͕ς�邪���t���[�����̒l�ɂ���i�l���ς�葱����̂�h���j
	
	const float TreeCollision = 4.0f;
	float PrevHeight = 0.0f;

	XMVECTOR PlayerFrontDirection = { 0,0,1 };//���ʃx�N�g�� ��������ǂꂾ����]������
	
}

Player::Player(GameObject* parent)
	:GameObject(parent, "Player"), hPlayer_(-1), hLandingPoint_(-1), IsOnGround_(true), IsDash_(false),
	JumpSpeed_(0.0f),
	Direction_({ 0,0,0 }),  PlayerPosition_({ 0,0,0 }), Acceleration_(0.0f),BackCamera_(BackCameraPos),
	PlayerState_(S_IDLE), CanMove_(true),PlayerHeight_(0)
{
	cameraTransform_ = this->transform_;
	CameraPosition_ = { this->transform_.position_.x ,this->transform_.position_.y + 1, this->transform_.position_.z - 8 };
	CameraTarget_ = { this->transform_.position_.x,this->transform_.position_.y, this->transform_.position_.z };

}

Player::~Player()
{
	Release();
}

void Player::Initialize()
{
	hPlayer_ = Model::Load("Player.fbx"); 
	assert(hPlayer_ >= 0);
	hLandingPoint_ = Model::Load("LandingPoint.fbx");
	assert(hLandingPoint_ >= 0);

	SetStartPosition();

	pGround_ = (Ground*)FindObject("Ground");
	pTerrain_ = (Terrain*)FindObject("Terrain");
	
	SphereCollider* collider = new SphereCollider(XMFLOAT3(0,0,0),0.3f);
	this->AddCollider(collider);

	
}

void Player::Update()
{
	cameraTransform_.position_ = this->transform_.position_;
	PlayerPosition_ = XMLoadFloat3(&this->transform_.position_); 

	switch (PlayerState_)
	{
	case Player::S_IDLE:
		UpdateIdle();
		break;
	case Player::S_JUMP:
		UpdateJump();
		break;
	case Player::S_HIT:
		UpdateHit();
		break;
	default:
		break;
	}

	//--------------�J�����Ǐ]--------------
	CameraTarget_ = { this->transform_.position_ };//�J�����̈ʒu�͎��@�̈ʒu�ɌŒ�
	XMMATRIX rotY = XMMatrixRotationY(XMConvertToRadians(cameraTransform_.rotate_.y));//�J�����̉�]�s�������
	BackCamera_ = XMVector3TransformCoord(BackCamera_, rotY);//�o�b�N�J�����̃x�N�g���ɂ�����
	XMStoreFloat3(&CameraPosition_, NewPos_ + BackCamera_);//�ړ��x�N�g���Ɖ��Z

	Camera::SetPosition(CameraPosition_);//�J�����̈ʒu���Z�b�g 
	Camera::SetTarget(CameraTarget_);//�J�����̏œ_���Z�b�g
	BackCamera_ = { BackCameraPos };//�o�b�N�J�����x�N�g�������Z�b�g

}

void Player::Draw()
{
	Model::SetTransform(hPlayer_, transform_);
	Model::Draw(hPlayer_);

	Transform t;
	t.position_ = PlayerFront;

	//�f�o�b�O�p ���ʂɉ~�̕`��
	Model::SetTransform(hLandingPoint_, t);
	Model::Draw(hLandingPoint_);

	ImGui::Text("PositionX:%.3f", this->transform_.position_.x);
	ImGui::Text("PositionY:%.3f", this->transform_.position_.y);
	ImGui::Text("PositionZ:%.3f", this->transform_.position_.z);

	ImGui::Text("IsOnGround:%.1f", IsOnGround_);

}

void Player::Release()
{
}

void Player::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Enemy")
	{
		Enemy* pEnemy = (Enemy*)FindObject("Enemy");
		XMFLOAT3 normal = pEnemy->GetPosition() - this->transform_.position_;
		XMVECTOR normalVec = XMVector3Normalize(XMLoadFloat3(&normal));
		pEnemy->PlayerReflect(normalVec);
	}
}

void Player::Dash()
{
	if (IsDash_)
	{
		Acceleration_ += 2.0;
		if (Acceleration_ > FullAccelerate) {
			Acceleration_ = FullAccelerate;
		}
	}
	else
	{
		Acceleration_ = 0;
	}
}

void Player::UpdateIdle()
{
	if (Input::IsKey(DIK_UP))
	{
		Direction_.z = 1.0;
	}
	if (Input::IsKey(DIK_DOWN))
	{
		Direction_.z = -1.0;
	}
	if (Input::IsKey(DIK_LEFT))
	{
		this->transform_.rotate_.y -= 1;
		cameraTransform_.rotate_.y -= 1;
	}
	if (Input::IsKey(DIK_RIGHT))
	{
		this->transform_.rotate_.y += 1;
		cameraTransform_.rotate_.y += 1;
	}

	//�v���C���[�̐��ʃx�N�g�����X�V
	//�����̑O���x�N�g��(��]���������܂�)
	ForwardVector_ = RotateVecFront(this->transform_.rotate_.y, PlayerFrontDirection);

	XMFLOAT3 rot = { 0,0,0 };
	XMStoreFloat3(&rot, ForwardVector_);
	PlayerFront = { transform_.position_ + rot };

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

	//�v���C���[��y��]�����W�A�������čs���
	XMMATRIX playerRot = XMMatrixRotationY(XMConvertToRadians(this->transform_.rotate_.y));

	//�v���C���[�̐i�s�������x�N�g����
	XMVECTOR PrevDir = XMVectorSet(Direction_.x, Direction_.y, Direction_.z, 0.0f);

	//�����x�N�g������]�s��ŕϊ�
	PrevDir = XMVector3TransformCoord(PrevDir, playerRot);

	//�P�ʃx�N�g��������
	XMVECTOR norm = XMVector3Normalize(PrevDir);

	//�ړ��x�N�g��������
	XMVECTOR MoveVector = XMVectorScale(norm, (speed + Acceleration_) * DeltaTime);

	//���݈ʒu�ƈړ��x�N�g�������Z
	XMVECTOR PrevPos = PlayerPosition_;
	NewPos_ = PrevPos + MoveVector;

	//int nextX, nextZ;
	//nextX = (int)XMVectorGetX(NewPos_) + 1.0;
	//nextY = (int)XMVectorGetY(NewPos_);
	//nextZ = (int)XMVectorGetZ(NewPos_) + 1.0;

	XMStoreFloat3(&this->transform_.position_, NewPos_);

	//�W�����v
	if (Input::IsKeyDown(DIK_SPACE))
	{
		if (IsOnGround_)
		{
			IsOnGround_ = false;
			PrevHeight = transform_.position_.y;
			JumpSpeed_ = 1.2;//�ꎞ�I��y�����Ƀ}�C�i�X����Ă���l��傫������
		}
	}

	if (transform_.position_.x > 15.0f || transform_.position_.x < -15.0f ||
		transform_.position_.z > 15.0f || transform_.position_.z < -15.0f)
	{
		IsOnGround_ = false;
	}
	else
	{
		IsOnGround_ = true;
	}
	

	JumpSpeed_ -= Player_Gravity;//�d�͕��̒l�������A�v���C���[�͏�ɉ������ɗ͂��������Ă���
	this->transform_.position_.y += JumpSpeed_;//�t�B�[���h�ɏ���Ă��邩�͊֌W�Ȃ��d�͂͂����葱����

	if (this->transform_.position_.y <= 0.5f && IsOnGround_)//�v���C���[�߂肱�ݖh�~�Ɉ��ȉ���y���W�Œl���Œ�
	{
		this->transform_.position_.y = 0.5f;
	}
	if (this->transform_.position_.y < -200) 
	{
		this->transform_.position_.y = -200;//�����̍Œ�l
	}

	if (Input::IsKeyDown(DIK_ESCAPE))
	{
		SetStartPosition();
	}


	CameraControl();

	Direction_ = { 0,0,0 };//�Ō�ɐi�s�����̃��Z�b�g���t���[���s��
}

void Player::UpdateJumpBefore()
{

}

void Player::UpdateJump()
{

}

void Player::UpdateHit()
{
}

void Player::CameraControl()
{
	if (Input::IsKey(DIK_X))
	{
		cameraTransform_.rotate_.y -= 5;
	}
	if (Input::IsKey(DIK_C))
	{
		cameraTransform_.rotate_.y += 5;
	}
	if (Input::IsKey(DIK_Z))//�J�����𐳖ʂɖ߂��i�����ɕω��Ȃ��j
	{
		cameraTransform_.rotate_.y = 0;
		this->transform_.rotate_.y = 0;
	}
}

void Player::EnemyReflect(XMVECTOR _vector)
{
	XMFLOAT3 f;
	XMStoreFloat3(&f, _vector);
	f.x *= 10.0;
	f.y *= 10.0;
	f.z *= 10.0;
	this->transform_.position_ = this->transform_.position_ + f;
}
