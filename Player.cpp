#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"

#include"Enemy.h"
#include"StageObject.h"

namespace {
	const float speed = 9.0f;
	const float Player_Gravity = 0.08; //0.16333f
	const float DeltaTime = 0.016f;
	const float FullAccelerate = 50.0f;
	const XMVECTOR front = { 0,0,1 };
	XMVECTOR BackCameraPos = { 0,2,-10,0 };//BackCamera�̒l�͕ς�邪���񂱂̒l�ɂ���i�l���ς�葱����̂�h���j

	float PrevHeight = 0.0f;
}

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),hModel_Player(-1),IsOnGround_(true),IsDash_(false), 
	JumpDirection({0,0,0}), JumpSpeed_(0.0f), MovePoint({0,0,0}), LandingPoint({0,0,0}),
	Direction({0,0,0}),PlayerDirection({0,0,0}), PlayerPosition({0,0,0}), Acceleration_(0.0f),
	BackCamera(BackCameraPos), pGround(nullptr), pStageObject(nullptr), PlayerState(S_IDLE)
{
	cameraTransform = this->transform_;
	CameraPosition = { this->transform_.position_.x ,this->transform_.position_.y + 1, this->transform_.position_.z - 8 };
	CameraTarget = { this->transform_.position_.x,this->transform_.position_.y, this->transform_.position_.z };

	StartPosition.position_ = { 0,1.0f,0 };

}

Player::~Player()
{
	Release();
}

void Player::Initialize()
{
	hModel_Player = Model::Load("ensui.fbx"); 
	hModel_LandingPoint = Model::Load("LandingPoint.fbx");
	this->transform_.position_ = StartPosition.position_;

	pGround = (Ground*)FindObject("Ground");
	pStageObject = (StageObject*)FindObject("StageObject");
	
	SphereCollider* col = new SphereCollider(XMFLOAT3(0,0,0),0.3f);
	this->AddCollider(col);
}

void Player::Update()
{
	cameraTransform.position_ = this->transform_.position_;
	PlayerPosition = XMLoadFloat3(&this->transform_.position_); 

	switch (PlayerState)
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

	CameraTarget = { this->transform_.position_ };//�J�����̈ʒu�͎��@�̈ʒu�ɌŒ�
	XMMATRIX rotY = XMMatrixRotationY(XMConvertToRadians(cameraTransform.rotate_.y));//�J�����̉�]�s�������
	BackCamera = XMVector3TransformCoord(BackCamera, rotY);//�o�b�N�J�����̃x�N�g���ɂ�����
	XMStoreFloat3(&CameraPosition, NewPos + BackCamera);//�ړ��x�N�g���Ɖ��Z

	Camera::SetPosition(CameraPosition);//�J�����̈ʒu���Z�b�g 
	Camera::SetTarget(CameraTarget);//�J�����̏œ_���Z�b�g
	BackCamera = { BackCameraPos };//�o�b�N�J�����x�N�g�������Z�b�g

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
	if (pTarget->GetObjectName() == "StageObject")
	{
		IsHitWall = true;
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
		Direction.z = 1.0;
	}
	if (Input::IsKey(DIK_DOWN))
	{
		Direction.z = -1.0;
	}
	if (Input::IsKey(DIK_LEFT))
	{
		this->transform_.rotate_.y -= 1;
		cameraTransform.rotate_.y -= 1;
	}
	if (Input::IsKey(DIK_RIGHT))
	{
		this->transform_.rotate_.y += 1;
		cameraTransform.rotate_.y += 1;
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

	XMMATRIX playerRot = XMMatrixRotationY(XMConvertToRadians(this->transform_.rotate_.y));//�v���C���[��y��]�����W�A�������čs���
	XMVECTOR PrevDir = XMVectorSet(Direction.x, Direction.y, Direction.z, 0.0f);//�v���C���[�̐i�s�������x�N�g����
	PrevDir = XMVector3TransformCoord(PrevDir, playerRot);//�����x�N�g������]�s��ŕϊ�
	XMVECTOR norm = XMVector3Normalize(PrevDir);// �P�ʃx�N�g���ɐ��K��
	XMVECTOR MoveVector = XMVectorScale(norm, (speed + Acceleration_) * DeltaTime);//�ړ��x�N�g��������

	XMVECTOR PrevPos = PlayerPosition;
	NewPos = PrevPos + MoveVector;
	
	int nextX, nextZ;
	nextX = (int)XMVectorGetX(NewPos);
	nextZ = (int)XMVectorGetZ(NewPos) + 1.0f;

	//if (pGround->IsMoveFront(nextX, nextZ)){}
	if (pGround->CanMoveFront(nextX, nextZ))
	{
		XMStoreFloat3(&this->transform_.position_, NewPos);
	}

	
	
	Direction = { 0,0,0 };//�i�s�����̃��Z�b�g���t���[���s��

	/*hGetGrass = pGround->GetGrassHandle();
	hGetWall = pStageObject->GetWallHandle();

	PlayerRayCast(hGetGrass);
	PlayerRayCast(hGetWall);*/

	//--------------�W�����v--------------
	//�{�^���������ƃW�����v�̒��n���\��(������)
	//WASD�Œ��n�ꏊ���ׂ����ݒ�(������)

	/*if (Input::IsKeyDown(DIK_W))
	{
		JumpDirection.z = 1.0f;
	}
	if (Input::IsKeyDown(DIK_S))
	{
		JumpDirection.z = -1.0f;
	}
	if (Input::IsKeyDown(DIK_A))
	{
		JumpTrans.rotate_.y -= 1.0f;
		if (JumpTrans.rotate_.y <= -60.0f) {
			JumpTrans.rotate_.y = -60.0f;
		}
	}
	if (Input::IsKeyDown(DIK_D))
	{
		JumpTrans.rotate_.y += 1.0f;
		if (JumpTrans.rotate_.y >= 60.0f) {
			JumpTrans.rotate_.y = 60.0f;
		}
	}

	XMMATRIX JumpRot = XMMatrixRotationY(XMConvertToRadians(JumpTrans.rotate_.y));
	XMVECTOR jumpMove = XMVectorSet(JumpDirection.x, JumpDirection.y, JumpDirection.z, 0.0f);
	jumpMove = XMVector3TransformCoord(jumpMove, JumpRot);
	XMVECTOR normal = XMVector3Normalize(jumpMove);
	XMVECTOR JumpVector = XMVectorScale(normal, speed);

	LandingPoint = PlayerPosition + JumpVector;*/

	if (Input::IsKeyDown(DIK_SPACE))
	{
		JumpSpeed_ = 0.5;//�ꎞ�I��y�����Ƀ}�C�i�X����Ă���l��傫������
		PlayerState = S_JUMP;

		//if (IsOnGround_) {
		//	IsOnGround_ = false;
		//	XMStoreFloat3(&this->transform_.position_, LandingPoint);
		//	JumpSpeed_ = 0.8;//�ꎞ�I��y�����Ƀ}�C�i�X����Ă���l��傫������
		//	MovePoint = XMVectorZero();
		//	LandingPoint = XMVectorZero();
		//	JumpDirection = { 0,0,0 };//�i�s�����̃��Z�b�g���t���[���s��
		//}
	}
	
	JumpSpeed_ -= Player_Gravity;//�d�͕��̒l�������A�v���C���[�͏�ɉ������ɗ͂��������Ă���
	this->transform_.position_.y += JumpSpeed_;

	if (this->transform_.position_.y <= 1.0f) //�v���C���[�߂肱�ݖh�~�Ɉ��ȉ���y���W�Œl���Œ�
	{
		this->transform_.position_.y = 1.0f;
	}

	if (JumpSpeed_ < -100) {
		JumpSpeed_ = -100;
	}

	



	

	if (Input::IsKeyDown(DIK_Q))
	{
		PlayerState = S_JUMPBEFORE;
	}

	//--------------�J�����Ǐ]--------------

	if (Input::IsKey(DIK_X))
	{
		cameraTransform.rotate_.y -= 5;
	}
	if (Input::IsKey(DIK_C))
	{
		cameraTransform.rotate_.y += 5;
	}
	if (Input::IsKey(DIK_Z))//�J�����𐳖ʂɖ߂��i�����ɕω��Ȃ��j
	{
		cameraTransform.rotate_.y = 0;
		this->transform_.rotate_.y = 0;
	}
}

void Player::UpdateHide()
{
}

void Player::UpdateJumpBefore()
{
	if (Input::IsKeyDown(DIK_Q))
	{
		PlayerState = S_IDLE;
	}
}

void Player::UpdateJump()
{
	if (this->transform_.position_.y < 1.0)//�v���C���[�߂肱�ݖh�~�Ɉ��ȉ���y���W�Œl���Œ�
	{
		transform_.position_.y = 1.0f;
		PlayerState = S_IDLE;
	}

	JumpSpeed_ -= Player_Gravity;//�d�͕��̒l�������A
	this->transform_.position_.y += JumpSpeed_;//�v���C���[�͏�ɉ������ɗ͂��������Ă���
}

void Player::UpdateHit()
{
}

void Player::PlayerRayCast(int handle)
{
	RayCastData data;
	data.start = transform_.position_;
	data.start.y =  2;
	data.dir = XMFLOAT3({ 0,-1,0 });
	Model::RayCast(handle, &data);
	if (data.hit == true)
	{
		transform_.position_.y = data.dist;
		PrevHeight = data.dist;
	}
}

