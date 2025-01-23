#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"

#include"Enemy.h"
#include"Terrain.h"
#include"Tree.h"

#include <algorithm>
#include<list>

namespace {
	const float speed = 9.0f;
	const float Player_Gravity = 0.08; //0.16333f
	const float DeltaTime = 0.016f;
	const float FullAccelerate = 50.0f;
	const XMVECTOR front = { 0,0,1 };
	XMVECTOR BackCameraPos = { 0,2,-10,0 };//BackCamera�̒l�͕ς�邪���t���[�����̒l�ɂ���i�l���ς�葱����̂�h���j

	const float TreeCollision = 4.0f;
	float PrevHeight = 0.0f;
	const XMFLOAT3 StartPosition = { 5.0f,3.0f,15.0f };
}

Player::Player(GameObject* parent)
	:GameObject(parent, "Player"), hPlayer_(-1), hLandingPoint_(-1), IsOnGround_(true), IsDash_(false),
	JumpSpeed_(0.0f), LandingPoint({ 0,0,0 }),
	Direction_({ 0,0,0 }), PlayerFrontDirection_({ 0,0,1 }), PlayerPosition_({ 0,0,0 }), Acceleration_(0.0f),
	BackCamera_(BackCameraPos), pGround_(nullptr), pTerrain_(nullptr), pTree_(nullptr), pTreeManager_(nullptr)
	, PlayerState_(S_IDLE), CanMove_(true),PlayerHeight_(0)
{
	cameraTransform_ = this->transform_;
	CameraPosition_ = { this->transform_.position_.x ,this->transform_.position_.y + 1, this->transform_.position_.z - 8 };
	CameraTarget_ = { this->transform_.position_.x,this->transform_.position_.y, this->transform_.position_.z };

	JumpTransform_.position_ = { 0.0f,1.0f,0.0f };

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

	this->transform_.position_ = StartPosition;

	pGround_ = (Ground*)FindObject("Ground");
	pTerrain_ = (Terrain*)FindObject("Terrain");
	pTree_ = (Tree*)FindObject("Tree");
	pTreeManager_ = (TreeManager*)FindObject("TreeManager");
	
	SphereCollider* col = new SphereCollider(XMFLOAT3(0,0,0),0.3f);
	this->AddCollider(col);

	
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
	case Player::S_HIDE:
		UpdateHide();
		break;
	case Player::S_JUMPBEFORE:
		UpdateJumpBefore();
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

	if (PlayerState_ == S_JUMPBEFORE)
	{
		Model::SetTransform(hLandingPoint_, JumpTransform_);
		Model::Draw(hLandingPoint_);
	}
}

void Player::Release()
{
}

void Player::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Enemy")
	{
		transform_.position_ = StartPosition;
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
	
	int nextX, nextZ;
	nextX = (int)XMVectorGetX(NewPos_) + 1.0;
	nextZ = (int)XMVectorGetZ(NewPos_) + 1.0;

	//�n��Ő��ʂ���I�u�W�F�N�g�ɂԂ��������͂��蔲���Ȃ��悤�ɂ���
	//�󒆂Ȃ��щz������
	if (pGround_->CanMoveFront(nextX, nextZ) && CanMove_)
	{
		XMStoreFloat3(&this->transform_.position_, NewPos_);
	}
	
	

	

	

	
	
	//--------------�W�����v--------------
	//�{�^���������ƃW�����v�̒��n���\��
	//WASD�Œ��n�ꏊ���ׂ����ݒ�

	//�n�ʂɂ���Ƃ��͍��W�Œ�
	LandGround();

	//�ʏ�W�����v
	if (!CanHide_ && Input::IsKeyDown(DIK_SPACE))
	{
		//PlayerState = S_JUMP;

		if (IsOnGround_)
		{
			IsOnGround_ = false;
			PrevHeight = transform_.position_.y;
			JumpSpeed_ = 1.2;//�ꎞ�I��y�����Ƀ}�C�i�X����Ă���l��傫������
		}
	}
	
	if (!IsOnGround_)
	{
		JumpSpeed_ -= Player_Gravity;//�d�͕��̒l�������A�v���C���[�͏�ɉ������ɗ͂��������Ă���
		this->transform_.position_.y += JumpSpeed_;
	}


	if (this->transform_.position_.y <= 0.0f) //�v���C���[�߂肱�ݖh�~�Ɉ��ȉ���y���W�Œl���Œ�
	{
		this->transform_.position_.y = 0.0f;
		IsOnGround_ = true;
	}

	if (JumpSpeed_ < -100) {
		JumpSpeed_ = -100;
	}
	
	//�_�����Ƃ���ɃW�����v
	if (Input::IsKeyDown(DIK_Q))
	{
		JumpTransform_ = this->transform_;
		LandingPoint = this->transform_.position_;
		PlayerState_ = S_JUMPBEFORE;
	}

	//--------------�B���-------------



	/*XMFLOAT3 TreePos = pTree_->GetPosition();
	XMVECTOR TreeVec = XMLoadFloat3(&TreePos);
	XMVECTOR d = XMVectorSubtract(PlayerPosition, TreeVec);
	float distance = XMVectorGetX(XMVector3Length(d));*/

	
	//if (CanHide && Input::IsKeyDown(DIK_SPACE))
	//{
	//	XMFLOAT3 a = pTree_->GetWorldPosition();
	//	XMVECTOR v = XMLoadFloat3(&a);
	//	XMStoreFloat3(&this->transform_.position_, v);
	//	PlayerState = S_HIDE;
	//}

	CameraControl();

	Direction_ = { 0,0,0 };//�Ō�ɐi�s�����̃��Z�b�g���t���[���s��
}

void Player::UpdateHide()
{
	if (Input::IsKeyDown(DIK_SPACE))
	{
		PlayerState_ = S_HIDE;
		CanHide_ = false;
	}

	CameraControl();
}

void Player::UpdateJumpBefore()
{
	CameraControl();
	if (Input::IsKeyDown(DIK_Z))
	{
		PlayerState_ = S_IDLE;
	}

	if (Input::IsKey(DIK_UP))
	{
		LandingPoint.z += 0.3f;
	}
	if (Input::IsKey(DIK_DOWN))
	{
		LandingPoint.z -= 0.3f;
	}
	if (Input::IsKey(DIK_LEFT))
	{
		LandingPoint.x -= 0.3f;
	}
	if (Input::IsKey(DIK_RIGHT))
	{
		LandingPoint.x += 0.3f;
	}

	JumpTransform_.position_ = LandingPoint;

	if (Input::IsKeyDown(DIK_SPACE))
	{
		//JumpSpeed_ = 1.2;
		PlayerStart_ = PlayerPosition_;
		JumpTarget_ = XMLoadFloat3(&JumpTransform_.position_);//�W�����v�̒��n�_
		JumpLength_ = JumpTarget_ - PlayerPosition_;//�W�����v���鋗��
		XMStoreFloat3(&JumpValue, JumpLength_);


		//JumpLength_ = XMVector3Normalize(JumpLength_);

		//XMStoreFloat3(&this->transform_.position_, v);//�ꎞ�I�Ȉړ�����
		PlayerState_ = S_JUMP;
	}
}

void Player::UpdateJump()
{
	XMStoreFloat3(&this->transform_.position_, JumpTarget_);
	PlayerState_ = S_IDLE;
}

void Player::UpdateHit()
{
}

void Player::PlayerRayCast(int handle)
{
	RayCastData data;
	data.start = transform_.position_;
	data.start.y =  transform_.position_.y + 1;
	data.dir = XMFLOAT3({ 0,-1,0 });
	Model::RayCast(handle, &data);
	if (data.hit == true)
	{
 		transform_.position_.y = -data.dist;
	}
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

void Player::LandGround()
{
	int x = (int)transform_.position_.x;
	int z = (int)transform_.position_.z;
	int MapPosY = pGround_->GetPositionData(x, z);

	if (transform_.position_.y <= MapPosY)
	{
		IsOnGround_ = true;
	}
	else
	{
 		IsOnGround_ = false;
	}

	if (IsOnGround_)
	{
		transform_.position_.y = MapPosY;
	}
}

bool Player::IsNearTree(const XMFLOAT3& pos)
{

	/*XMFLOAT3 TreePos = pTree_->GetPosition();
	XMVECTOR TreeVec = XMLoadFloat3(&TreePos);
	XMVECTOR d = XMVectorSubtract(PlayerPosition, TreeVec);
	float distance = XMVectorGetX(XMVector3Length(d));*/

	XMVECTOR TreeVec = XMLoadFloat3(&pos);
	XMVECTOR dist = XMVectorSubtract(PlayerPosition_, TreeVec);
	float distance = XMVectorGetX(XMVector3Length(dist));

	if (distance <= 4.0f)
		return true;
	else
		return false;
}
