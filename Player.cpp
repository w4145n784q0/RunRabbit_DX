#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"

#include"Enemy.h"

namespace {
	const float speed = 9.0f;
	const float Player_Gravity = 0.08; //0.16333f
	const float DeltaTime = 0.016f;
	const XMVECTOR front = { 0,0,1 };
	XMVECTOR BackCameraPos = { 0,2,-10,0 };//BackCamera�̒l�͕ς�邪���񂱂̒l�ɂ���i�l���ς�葱����̂�h���j
}

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),hModel_Player(-1),IsOnGround_(true),JumpSpeed_(0.0f),
	 PlayerDirection({ 0,0,0 }),PlayerPosition({0,0,0}), Acceleration_(0.0f),BackCamera(BackCameraPos)
{
	cameraTransform = transform_;
	CameraPosition = { transform_.position_.x ,transform_.position_.y + 1, transform_.position_.z - 8 };
	CameraTarget = { transform_.position_.x,transform_.position_.y, transform_.position_.z };

	hcamera = - 1;
}

Player::~Player()
{
	Release();
}

void Player::Initialize()
{
	hModel_Player = Model::Load("ensui.fbx"); 
	hcamera = Model::Load("box.fbx");
	this->transform_.position_ = { -20,0,-20};

	
	SphereCollider* col = new SphereCollider(XMFLOAT3(0,0,0),0.1f);
	this->AddCollider(col);
}

void Player::Update()
{
	cameraTransform.position_ = transform_.position_;
	

	PlayerPosition = XMLoadFloat3(&this->transform_.position_); 
	XMFLOAT3 x;
	XMStoreFloat3(&x, PlayerPosition);

	if (Input::IsKey(DIK_UP))
	{
		Direction.z = 1.0;
		moveDir = Front;
	}
	if (Input::IsKey(DIK_DOWN))
	{
		Direction.z = -1.0;
		moveDir = Back;
	}
	if (Input::IsKey(DIK_LEFT))
	{
		Direction.x = -1.0;
		moveDir = Left;
		
	}
	if (Input::IsKey(DIK_RIGHT))
	{
		Direction.x = 1.0;
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
	

	XMVECTOR PrevDir = XMVectorSet(Direction.x,Direction.y, Direction.z, 0.0f);
	XMVECTOR norm = XMVector3Normalize(PrevDir);// �P�ʃx�N�g���ɐ��K��
	XMVECTOR MoveVector = XMVectorScale(norm,(speed + Acceleration_) * DeltaTime);//�ړ��x�N�g��������

	XMVECTOR PrevPos = PlayerPosition;

	XMVECTOR NewPos = PrevPos + MoveVector;
	XMStoreFloat3(&transform_.position_, NewPos);

	Direction = { 0,0,0 };

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
	
	if (Input::IsKey(DIK_A))
	{
		cameraTransform.rotate_.y -= 5;
	}
	if (Input::IsKey(DIK_D))
	{
		cameraTransform.rotate_.y += 5;
	}
	if (Input::IsKey(DIK_Z))//�J�����𐳖ʂɖ߂��i�����ɕω��Ȃ��j
	{
		cameraTransform.rotate_.y = 0;
	}

	CameraTarget = { transform_.position_ };//�J�����̈ʒu�͎��@�̈ʒu�ɌŒ�
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

	/*Model::SetTransform(hcamera, cameraTransform);
	Model::Draw(hcamera);*/
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
