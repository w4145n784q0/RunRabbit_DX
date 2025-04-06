#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Audio.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

#include"Enemy.h"
#include"BossBattleScene.h"

#include <algorithm>
#include<list>

namespace {
	const float speed = 9.0f;
	const float Player_Gravity = 0.08f; //0.16333f
	const float DeltaTime = 0.016f;
	const float FullAccelerate = 60.0f;
	XMVECTOR BackCameraPos = { 0,2,-10,0 };//BackCamera�̒l�͕ς�邪���t���[�����̒l�ɂ���i�l���ς�葱����̂�h���j
	
	const float TreeCollision = 4.0f;
	float PrevHeight = 0.0f;

	const float KnockBackPower = 2.5f; //�m�b�N�o�b�N���鋭��

	XMVECTOR PlayerFrontDirection = { 0,0,1 };//���ʃx�N�g�� ��������ǂꂾ����]������
	int deadTimerValue = 60;//��������
	
}

Player::Player(GameObject* parent)
	:GameObject(parent, "Player"), hPlayer_(-1), hLandingPoint_(-1), IsOnGround_(true), IsDash_(false),
	JumpSpeed_(0.0f),
	Direction_({ 0,0,0 }),  PlayerPosition_({ 0,0,0 }), Acceleration_(0.0f),BackCamera_(BackCameraPos),
	PlayerState_(S_IDLE), CanMove_(true),PlayerHeight_(0),
	deadTimer_(deadTimerValue)
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
	hCollisionSound_ = Audio::Load("maou_se_battle15.wav");
	assert(hCollisionSound_ >= 0);

	SetStartPosition();

	pGround_ = (Ground*)FindObject("Ground");
	
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
	case Player::S_HIT:
		UpdateHit();
		break;
	case Player::S_CHARGE:
		UpdateCharge();
		break;
	case Player::S_ATTACK:
		UpdateAttack();
		break;
	case Player::S_OUT:
		UpdateOut();
		break;
	case Player::S_WALLHIT:
		UpdateWallHit();
		break;
	default:
		break;
	}

	//--------------�J�����Ǐ]--------------
	CameraTarget_ = { this->transform_.position_ };//�J�����̏œ_�͎��@�̈ʒu�ɌŒ�
	XMMATRIX rotY = XMMatrixRotationY(XMConvertToRadians(cameraTransform_.rotate_.y));//�J�����̉�]�s��쐬(Y��)
	XMMATRIX rotX = XMMatrixRotationX(XMConvertToRadians(cameraTransform_.rotate_.x));//�J�����̉�]�s��쐬(X��)
	XMMATRIX rotCamera = XMMatrixMultiply(rotX, rotY);
	BackCamera_ = XMVector3TransformCoord(BackCamera_, rotCamera);//�o�b�N�J�����̃x�N�g���ɂ�����
	XMStoreFloat3(&CameraPosition_, NewPos_ + BackCamera_);//�ړ��x�N�g���Ɖ��Z

	//CameraPosition_.y += Camera::CameraShake();
	CameraPosition_.x += Camera::CameraShakeFloat3().x;
	CameraPosition_.y += Camera::CameraShakeFloat3().y;
	CameraPosition_.z += Camera::CameraShakeFloat3().z;

	Camera::SetPosition(CameraPosition_);//�J�����̈ʒu���Z�b�g 
	Camera::SetTarget(CameraTarget_);//�J�����̏œ_���Z�b�g
	BackCamera_ = { BackCameraPos };//�o�b�N�J�����x�N�g�������Z�b�g

}

void Player::Draw()
{
	Model::SetTransform(hPlayer_, transform_);
	Model::Draw(hPlayer_);

	ImGui::Text("PositionX:%.3f", this->transform_.position_.x);
	ImGui::Text("PositionY:%.3f", this->transform_.position_.y);
	ImGui::Text("PositionZ:%.3f", this->transform_.position_.z);

	//ImGui::Text("camera y :%.3f", CameraPosition_.y);
	//ImGui::Text("camera x :%.3f", CameraPosition_.x);

	/*ImGui::Text("IsOnGround:%.1f", IsOnGround_);*/

}

void Player::Release()
{
}

void Player::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Enemy")
	{
		//�G�̃m�b�N�o�b�N
		Enemy* pEnemy = (Enemy*)FindObject("Enemy");

		//�G�̈ʒu-���@�̈ʒu���v�Z�i�G�̔��ˁj
		//�P�ʃx�N�g���ɂ���
		XMFLOAT3 Enemydirection = pEnemy->GetPosition() - this->transform_.position_;
		XMVECTOR EnemynormalDirection = XMVector3Normalize(XMLoadFloat3(&Enemydirection));

		//���@�̈ʒu-�G�̈ʒu���v�Z�i���@�̔��ˁj
		//�P�ʃx�N�g���ɂ���
		XMFLOAT3 Playerdirection = this->transform_.position_ - pEnemy->GetPosition();
		XMVECTOR PlayernormalDirection = XMVector3Normalize(XMLoadFloat3(&Playerdirection));

		////�G�̃m�b�N�o�b�N����
		//pEnemy->PlayerReflect(normalDirection, IsDash_);

		//�v���C���[�̏Փˎ�����
		//�v���C���[:�ʏ� �G:�ʏ� �ω��Ȃ�
		//�v���C���[:�ʏ� �G:�U�� �v���C���[���͂���
		//�v���C���[:�_�b�V�� �G:�U�� �G���͂��� �v���C���[�͕����x�N�g��(�G�̈ʒu-���@�̈ʒu)�ɑ΂����������Ɉړ��i���ʂ���Ԃ�������v���C���[�͒�~
		//�v���C���[:�_�b�V�� �G:�ʏ� �G��傫���͂���
		//�v���C���[�͕����x�N�g��(�G�̈ʒu-���@�̈ʒu)�ɑ΂����������Ɉړ��i���ʂ���Ԃ�������v���C���[�͒�~

		//XMFLOAT3 normal;
		//XMStoreFloat3(&normal, normalDirection);

		bool IsEnemyAttack = pEnemy->GetStateAttack();

		if (IsEnemyAttack)//�G:�U��
		{
			if (IsDash_)//�v���C���[:�U��
			{
				//�G�̃m�b�N�o�b�N����
				pEnemy->PlayerReflect(EnemynormalDirection, IsDash_);
				EnemyReflect(PlayernormalDirection, IsEnemyAttack);
				//PlayerState_ = S_HIT;
			}
			else//�v���C���[:�ʏ�
			{
				//PlayerState_ = S_HIT;
				EnemyReflect(PlayernormalDirection, IsEnemyAttack);
			}
		}
		else//�G:�ʏ�
		{
			if (IsDash_)//�v���C���[:�U��
			{
				//�G�̃m�b�N�o�b�N����
				pEnemy->PlayerReflect(EnemynormalDirection, IsDash_);
			}
			else//�v���C���[:�ʏ�
			{
				//�ω��Ȃ�
			}
		}

		//�J�����U��
		Camera::CameraShakeStart(0.15f);

		//�Ռ���
		Audio::Play(hCollisionSound_);

		Acceleration_ = 0;
		IsDash_ = false;
		IsDashStart_ = false;
	}
}

void Player::Dash()
{
	/*if (IsDash_)
	{
		Acceleration_ += 2.0;
		if (Acceleration_ > FullAccelerate) {
			Acceleration_ = FullAccelerate;
		}
	}
	else
	{
		Acceleration_ = 0;
	}*/
	if(IsDash_)
	{
		if (!IsDashStart_)
		{
			Acceleration_ += FullAccelerate;
			IsDashStart_ = true;
		}
		else
		{
			Acceleration_ -= 2;
			Direction_.z = 1.0;
			if (Acceleration_ <= 0)
			{
				IsDash_ = false;
				IsDashStart_ = false;
			}
		}
	}
}

void Player::UpdateIdle()
{
	//------------------�L�[�{�[�h���͂̈ړ�------------------//
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

	//------------------�Q�[���p�b�h�X�e�B�b�N�̈ړ�------------------//

	//�O�������Ɉړ�
	if (Input::GetPadStickL().y >= 0.5 /*&& Input::GetPadStickL().x <= 0.5 && Input::GetPadStickL().x >= -0.5*/)
	{
		Direction_.z = 1.0;
	}

	//��������Ɉړ�
	if (Input::GetPadStickL().y <= -0.5 /*&& Input::GetPadStickL().x >= 0.5 && Input::GetPadStickL().x <= -0.5*/)
	{
		Direction_.z = -1.0;
	}

	//�O�i&����]
	if (Input::GetPadStickL().y >= 0.5 && Input::GetPadStickL().x <= -0.25)
	{
		Direction_.z = 1.0;
		this->transform_.rotate_.y -= 1;
		cameraTransform_.rotate_.y -= 1;
	}

	//�O�i&�E��]
	if (Input::GetPadStickL().y >= 0.5 && Input::GetPadStickL().x >= 0.25)
	{
		Direction_.z = 1.0;
		this->transform_.rotate_.y += 1;
		cameraTransform_.rotate_.y += 1;
	}

	//����]����
	if (Input::GetPadStickL().x <= -0.8 && Input::GetPadStickL().y <= 0.8)
	{
		this->transform_.rotate_.y -= 1;
		cameraTransform_.rotate_.y -= 1;
	}
	//�E��]����
	if (Input::GetPadStickL().x >= 0.8 && Input::GetPadStickL().y <= 0.8)
	{
		this->transform_.rotate_.y += 1;
		cameraTransform_.rotate_.y += 1;
	}

	//�v���C���[�̐��ʃx�N�g�����X�V
	//�����̑O���x�N�g��(��]���������܂�)
	ForwardVector_ = RotateVecFront(this->transform_.rotate_.y, PlayerFrontDirection);

	//XMFLOAT3 rot = { 0,0,0 };
	//XMStoreFloat3(&rot, ForwardVector_);
	//PlayerFront = { transform_.position_ + rot };

	//--------------�_�b�V���֌W--------------
	/*if (Input::IsKey(DIK_LSHIFT) || Input::IsKey(DIK_RSHIFT) 
		|| Input::IsPadButton(XINPUT_GAMEPAD_LEFT_SHOULDER) || Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		IsDash_ = true;
	}
	else
	{
		IsDash_ = false;
	}*/
	if (Input::IsKey(DIK_LSHIFT) || Input::IsKey(DIK_RSHIFT) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B))
	{
		if(IsOnGround_)
		{
			IsDash_ = true;
		}
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

	XMStoreFloat3(&this->transform_.position_, NewPos_);

	//�W�����v
	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	{
		if (IsOnGround_)
		{
			IsOnGround_ = false;
			PrevHeight = transform_.position_.y;
			JumpSpeed_ = 2.2f;//�ꎞ�I��y�����Ƀ}�C�i�X����Ă���l��傫������
		}
	}

	if (transform_.position_.x > 60.0f || transform_.position_.x < -60.0f ||
		transform_.position_.z > 60.0f || transform_.position_.z < -60.0f)
	{
		PlayerState_ = S_WALLHIT;
	}
	

	JumpSpeed_ -= Player_Gravity;//�d�͕��̒l�������A�v���C���[�͏�ɉ������ɗ͂��������Ă���
	this->transform_.position_.y += JumpSpeed_;//�t�B�[���h�ɏ���Ă��邩�͊֌W�Ȃ��d�͂͂����葱����

	if (this->transform_.position_.y <= 0.5f && IsOnGround_)//�v���C���[�߂肱�ݖh�~�Ɉ��ȉ���y���W�Œl���Œ�
	{
		this->transform_.position_.y = 0.5f;
	}
	if (this->transform_.position_.y < -500) 
	{
		this->transform_.position_.y = -500;//�����̍Œ�l
		PlayerState_ = S_OUT;
	}

	if (Input::IsKeyDown(DIK_ESCAPE))
	{
		SetStartPosition();
	}


	CameraControl();


	Direction_ = { 0,0,0 };//�Ō�ɐi�s�����̃��Z�b�g���t���[���s��
}

void Player::UpdateHit()
{
	//���x�������Ă���
	KnockBack_Velocity_.x *= 0.9;
	KnockBack_Velocity_.z *= 0.9;

	//���t���[��position�ɕ��������Z
	//�ʒu = �ʒu + ���� * ���x
	transform_.position_.x += KnockBack_Direction_.x * KnockBack_Velocity_.x;
	transform_.position_.z += KnockBack_Direction_.z * KnockBack_Velocity_.z;

	if (KnockBack_Velocity_.x <= 0.5f || KnockBack_Velocity_.z <= 0.5f)
	{
		PlayerState_ = S_IDLE;
	}
}

void Player::UpdateCharge()
{

}

void Player::UpdateAttack()
{

}

void Player::UpdateOut()
{
	if (--deadTimer_ < 0)
	{
		//BossBattleScene* pBossBattleScene = (BossBattleScene*)FindObject("BossBattleScene");
		//pBossBattleScene->DeadCountPlus();

		deadTimer_ = deadTimerValue;
		PlayerState_ = S_IDLE;
		SetStartPosition();
	}
}

void Player::UpdateWallHit()
{
	if (--deadTimer_ < 0)
	{
		//BossBattleScene* pBossBattleScene = (BossBattleScene*)FindObject("BossBattleScene");
		//pBossBattleScene->DeadCountPlus();

		deadTimer_ = deadTimerValue;
		PlayerState_ = S_IDLE;
		SetStartPosition();
	}
}

void Player::UpdateDead()
{

}

void Player::CameraControl()
{
	if (Input::IsKey(DIK_A) || Input::GetPadStickR().x <= -0.7)
	{
		cameraTransform_.rotate_.y -= 2.5;
	}
	if (Input::IsKey(DIK_D) || Input::GetPadStickR().x >= 0.7)
	{
		cameraTransform_.rotate_.y += 2.5;
	}

	if (Input::IsKey(DIK_W) || Input::GetPadStickR().y <= -0.7)
	{
		if(cameraTransform_.rotate_.x >= 60.0f)
		{
			cameraTransform_.rotate_.x = 60.0f;
		}
		else
		{
			cameraTransform_.rotate_.x += 2.5;
		}
	}
	if (Input::IsKey(DIK_S) || Input::GetPadStickR().y >= 0.7)
	{
		if (cameraTransform_.rotate_.x <= -10.0f)
		{
			cameraTransform_.rotate_.x = -10.0f;
		}
		else
		{
			cameraTransform_.rotate_.x -= 2.5;
		}
	}

	if (Input::IsKey(DIK_Z) || Input::IsPadButton(XINPUT_GAMEPAD_Y))//�J�����𐳖ʂɖ߂��i�����ɕω��Ȃ��j
	{
		cameraTransform_.rotate_.y = 0;
		cameraTransform_.rotate_.x = 0;
		this->transform_.rotate_.y = 0;
	}
}

void Player::EnemyReflect(XMVECTOR _vector, bool _IsAttack)
{
	XMFLOAT3 f;
	XMStoreFloat3(&f, _vector);
	KnockBack_Direction_ = f;

	if (_IsAttack)
	{
		KnockBack_Velocity_.x = KnockBackPower * 1.5;
		KnockBack_Velocity_.z = KnockBackPower * 1.5;
	}
	else
	{
		KnockBack_Velocity_.x = KnockBackPower;
		KnockBack_Velocity_.z = KnockBackPower;
	}

	PlayerState_ = S_HIT;
}

