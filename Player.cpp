#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Audio.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"
#include"Engine/VFX.h"
#include"Engine/SceneManager.h"
#include"BattleScene.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

#include"Enemy.h"

#include <algorithm>
#include<list>

namespace {
	const float speed = 9.0f;
	const float Player_Gravity = 0.08f; //0.16333f
	const float DeltaTime = 0.016f;

	const float MoveRotateX = 10.0f;//�ړ�����1f�̉�]��
	const float FastRotateX = 30.0f;////(�`���[�W���Ȃ�)������]����1f�̉�]��
	const float FullAccelerate = 120.0f;//�`���[�W�ő�l
	XMVECTOR BackCameraPos = { 0,3,-10,0 };//BackCamera�̒l�͕ς�邪���t���[�����̒l�ɂ���i�l���ς�葱����̂�h���j

	const float KnockBackPower = 2.0f; //�m�b�N�o�b�N���鋭��

	XMVECTOR PlayerFrontDirection = { 0,0,1 };//���ʂ̏����l ��������ǂꂾ����]������
	const int deadTimerValue = 60;//�����܂ł̎���
	const int Invincibility = 120;//���G���Ԃ̒萔

}

Player::Player(GameObject* parent)
	:GameObject(parent, "Player"), hPlayer_(-1), /*hAttackArrow_(-1),*/ hCollisionSound_(-1),
	pGround_(nullptr),PlayerState_(S_MAX),CameraState_(S_NORMALCAMERA),
	IsOnGround_(true),JumpSpeed_(0.0f),

	Direction_({ 0,0,0 }),  PlayerPosition_({ 0,0,0 }), Acceleration_(0.0f),BackCamera_(BackCameraPos),
	PlayerHeight_(0),AcceleValue_(2.0f),
	deadTimer_(deadTimerValue),InvincibilityTime_(Invincibility),IsInvincibility_(false),ColliderSize_(0.3f),CharacterLife_(3)
{
	cameraTransform_ = this->transform_;
	CameraPosition_ = { this->transform_.position_.x ,this->transform_.position_.y + 1, this->transform_.position_.z - 8 };
	CameraTarget_ = { this->transform_.position_.x,this->transform_.position_.y, this->transform_.position_.z };

	srand((unsigned)time(NULL));
}

Player::~Player()
{
	Release();
}

void Player::Initialize()
{
	hPlayer_ = Model::Load("Model\\chara2.fbx");
	assert(hPlayer_ >= 0);

	//hAttackArrow_ = Model::Load("Model\\AttackArrow.fbx");
	//assert(hAttackArrow_ >= 0);

	hCollisionSound_ = Audio::Load("Sound\\maou_se_battle15.wav");
	assert(hCollisionSound_ >= 0);

	SetStartPosition();
	transform_.rotate_.y = 180.0f;
	//ArrowTransform_.rotate_.y = 180.0f;

	pGround_ = (Ground*)FindObject("Ground");
	
	SphereCollider* collider = new SphereCollider(XMFLOAT3(0,0,0),ColliderSize_);
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
	case Player::S_WALLHIT:
		UpdateWallHit();
		break;
	default:
		break;
	}


	//AttackEffectStop(LocusEffectCount, IsLocusEffect, hAttackEmitLocus_);
	//AttackEffectStop(AuraEffectCount, IsAuraEffect, hAttackEmitAura_);

	//--------------�J�����Ǐ]--------------
	CameraTarget_ = { this->transform_.position_ };//�J�����̏œ_�͎��@�̈ʒu�ɌŒ�
	XMMATRIX rotY = XMMatrixRotationY(XMConvertToRadians(cameraTransform_.rotate_.y));//�J�����̉�]�s��쐬(Y��)
	XMMATRIX rotX = XMMatrixRotationX(XMConvertToRadians(cameraTransform_.rotate_.x));//�J�����̉�]�s��쐬(X��)
	XMMATRIX rotCamera = XMMatrixMultiply(rotX, rotY);
	BackCamera_ = XMVector3TransformCoord(BackCamera_, rotCamera);//�o�b�N�J�����̃x�N�g���ɂ�����
	XMStoreFloat3(&CameraPosition_, NewPos_ + BackCamera_);//�ړ��x�N�g���Ɖ��Z

	//--------------�J�����U��--------------
	//CameraPosition_.y += Camera::CameraShake();
	CameraPosition_.x += Camera::CameraShakeFloat3().x;
	CameraPosition_.y += Camera::CameraShakeFloat3().y;
	CameraPosition_.z += Camera::CameraShakeFloat3().z;

	Camera::SetPosition(CameraPosition_);//�J�����̈ʒu���Z�b�g 
	Camera::SetTarget(CameraTarget_);//�J�����̏œ_���Z�b�g
	BackCamera_ = { BackCameraPos };//�o�b�N�J�����x�N�g�������Z�b�g

	if (Input::IsKeyDown(DIK_ESCAPE))
	{
		SetStartPosition();
	}

}

void Player::Draw()
{
	Model::SetTransform(hPlayer_, transform_);
	Model::Draw(hPlayer_);

	/*if (PlayerState_ == S_CHARGE)
	{
		Model::SetTransform(hAttackArrow_, ArrowTransform_);
		Model::Draw(hAttackArrow_);
	}*/

#ifdef _DEBUG
	ImGui::Text("PositionX:%.3f", this->transform_.position_.x);
	ImGui::Text("PositionY:%.3f", this->transform_.position_.y);
	ImGui::Text("PositionZ:%.3f", this->transform_.position_.z);

	//ImGui::Text("camera y :%.3f", CameraPosition_.y);
	//ImGui::Text("camera x :%.3f", CameraPosition_.x);

	//ImGui::Text("dash:%.3f", Acceleration_);
	
	XMFLOAT3 tmp;
    XMStoreFloat3(&tmp, ForwardVector_);

	//ImGui::Text("front.x:%3f", (float)tmp.x);
	//ImGui::Text("front.y:%3f", (float)tmp.y);
	//ImGui::Text("front.z:%3f", (float)tmp.z);

	ImGui::Text("PlayerLife:%.3f", (float)CharacterLife_);
#endif

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

		//�v���C���[�̏Փˎ�����
		//�v���C���[:�ʏ� �G:�ʏ� �o�����͂������(�y�x)
		//�v���C���[:�ʏ� �G:�U�� �v���C���[��傫���͂���
		//�v���C���[:�_�b�V�� �G:�U�� �G���͂��� �v���C���[�͕����x�N�g��(�G�̈ʒu-���@�̈ʒu)�ɑ΂����������Ɉړ��i���ʂ���Ԃ�������v���C���[�͒�~
		//�v���C���[:�_�b�V�� �G:�ʏ� �G��傫���͂���
		//�v���C���[�͕����x�N�g��(�G�̈ʒu-���@�̈ʒu)�ɑ΂����������Ɉړ��i���ʂ���Ԃ�������v���C���[�͒�~

		bool IsEnemyAttack = pEnemy->GetStateAttack();

		if (IsEnemyAttack)//�G:�U��
		{
			if (IsAttackState())//�v���C���[:�U��
			{
				//�G�̃m�b�N�o�b�N����
				pEnemy->PlayerReflect(EnemynormalDirection, true);
				EnemyReflect(PlayernormalDirection, IsEnemyAttack);
			}
			else//�v���C���[:�ʏ�
			{
				EnemyReflect(PlayernormalDirection, IsEnemyAttack);
			}
			//�q�b�g�G�t�F�N�g
			SetHitEffect();
		}
		else//�G:�ʏ�
		{
			if (IsAttackState())//�v���C���[:�U��
			{
				//�G�̃m�b�N�o�b�N����
				pEnemy->PlayerReflect(EnemynormalDirection,true);
				//�q�b�g�G�t�F�N�g
				SetHitEffect();
			}
			else//�v���C���[:�ʏ�
			{
				pEnemy->PlayerReflect(EnemynormalDirection, false);
				EnemyReflect(PlayernormalDirection, IsEnemyAttack);
			}
		}

		//�J�����U��
		Camera::CameraShakeStart(0.3f);

		//�Ռ���
		Audio::Play(hCollisionSound_);

		Acceleration_ = 0;
	}

	if (pTarget->GetObjectName() == "Fence")
	{
		if(!IsInvincibility_ && !(PlayerState_ == S_WALLHIT))
		{
			Acceleration_ = 0.0f;
			XMFLOAT3 tmp;
			XMStoreFloat3(&tmp, ForwardVector_);
			KnockBack_Direction_ = tmp;
			KnockBack_Velocity_.x = KnockBackPower;
			KnockBack_Velocity_.z = KnockBackPower;
			PlayerState_ = S_WALLHIT;
		}
	}
}

void Player::UpdateIdle()
{
	//���G���Ԃ̌v�Z
	if (IsInvincibility_)
	{
		if (--InvincibilityTime_ < 0)
		{
			InvincibilityTime_ = Invincibility;
			IsInvincibility_ = false;
		}
	}

	//------------------�L�[�{�[�h���͂̈ړ�------------------//
	if (Input::IsKey(DIK_UP))
	{
		Direction_.z = -1.0;
		this->transform_.rotate_.x -= MoveRotateX;
	}
	if (Input::IsKey(DIK_DOWN))
	{
		Direction_.z = 1.0;
		this->transform_.rotate_.x += MoveRotateX;
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
		Direction_.z = -1.0;
		this->transform_.rotate_.x -= MoveRotateX;
	}

	//��������Ɉړ�
	if (Input::GetPadStickL().y <= -0.5 /*&& Input::GetPadStickL().x >= 0.5 && Input::GetPadStickL().x <= -0.5*/)
	{
		Direction_.z = 1.0;
		this->transform_.rotate_.x += MoveRotateX;
	}

	//�O�i&����]
	if (Input::GetPadStickL().y >= 0.5 && Input::GetPadStickL().x <= -0.25)
	{
		Direction_.z = -1.0;
		this->transform_.rotate_.y -= 1;
		this->transform_.rotate_.x -= MoveRotateX;
		cameraTransform_.rotate_.y -= 1;
	}

	//�O�i&�E��]
	if (Input::GetPadStickL().y >= 0.5 && Input::GetPadStickL().x >= 0.25)
	{
		Direction_.z = -1.0;
		this->transform_.rotate_.y += 1;
		this->transform_.rotate_.x += MoveRotateX;
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

	//--------------�_�b�V���֌W--------------

	if (Input::IsKeyDown(DIK_LSHIFT) || Input::IsKeyDown(DIK_RSHIFT) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B))
	{
		if (IsOnGround_)
		{
			//SetChargeEffect();
			PlayerState_ = S_CHARGE;
		}
	}

	PlayerMove();

	//�����̑O���x�N�g��(��]���������܂�)���X�V
	ForwardVector_ = RotateVecFront(this->transform_.rotate_.y, PlayerFrontDirection);

	//�v���C���[�̐��ʃx�N�g�����X�V
	//�����̑O���x�N�g��(��]���������܂�)
	/*ForwardVector_ = RotateVecFront(this->transform_.rotate_.y, PlayerFrontDirection);
	XMFLOAT3 rot = { 0,0,0 };
	XMStoreFloat3(&rot, ForwardVector_);
	PlayerFront = { transform_.position_ + rot };
	PlayerFront.z += 1.0;
	ArrowTransform_.position_ = PlayerFront;*/


	//�W�����v
	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	{
		if (IsOnGround_)
		{
			IsOnGround_ = false;
			JumpSpeed_ = 1.8f;//�ꎞ�I��y�����Ƀ}�C�i�X����Ă���l��傫������
		}
	}

	Gravity();

	/*if (Input::IsKeyDown(DIK_ESCAPE))
	{
		SetStartPosition();
	}*/


	CameraControl();


	Direction_ = { 0,0,0 };//�Ō�ɐi�s�����̃��Z�b�g���t���[���s��
}

void Player::UpdateHit()
{
	//���x�������Ă���
	//KnockBack_Velocity_.x *= 0.9;
	//KnockBack_Velocity_.z *= 0.9;

	//���t���[��position�ɕ��������Z
	//�ʒu = �ʒu + ���� * ���x
	//transform_.position_.x += KnockBack_Direction_.x * KnockBack_Velocity_.x;
	//transform_.position_.z += KnockBack_Direction_.z * KnockBack_Velocity_.z;
	//cameraTransform_.position_ = transform_.position_;

	Blown();
	Gravity();

	if (KnockBack_Velocity_.x <= 0.5f || KnockBack_Velocity_.z <= 0.5f)
	{
		PlayerState_ = S_IDLE;
	}
}

void Player::UpdateCharge()
{
	SetChargingEffect();

	if (Input::IsKey(DIK_LEFT))
	{
		this->transform_.rotate_.y -= 1;
		cameraTransform_.rotate_.y -= 1;
		//ArrowTransform_.rotate_.y -= 1;
	}
	if (Input::IsKey(DIK_RIGHT))
	{
		this->transform_.rotate_.y += 1;
		cameraTransform_.rotate_.y += 1;
		//ArrowTransform_.rotate_.y += 1;
	}

	//����]����
	if (Input::GetPadStickL().x <= -0.8 && Input::GetPadStickL().y <= 0.8)
	{
		this->transform_.rotate_.y -= 1;
		cameraTransform_.rotate_.y -= 1;
		//ArrowTransform_.rotate_.y -= 1;
	}
	//�E��]����
	if (Input::GetPadStickL().x >= 0.8 && Input::GetPadStickL().y <= 0.8)
	{
		this->transform_.rotate_.y += 1;
		cameraTransform_.rotate_.y += 1;
		//ArrowTransform_.rotate_.y += 1;
	}

	if (Input::IsKeyUp(DIK_LSHIFT) || Input::IsKeyUp(DIK_RSHIFT) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B))
	{
		//VFX::End(hChargeEmit_);
		//SetAttackLocusEffect();
		//SetAttackAuraEffect();
		PlayerState_ = S_ATTACK;
	}

	//�`���[�W�����̉����ʂ����Z��������
	if (Acceleration_ < FullAccelerate)
	{
		Acceleration_ += AcceleValue_;
	}
	else
	{
		Acceleration_ = FullAccelerate;
	}

	this->transform_.rotate_.x -= FastRotateX;
	CameraControl();
}

void Player::UpdateAttack()
{
	SetAttackLocusEffect();

	Acceleration_ -= AcceleValue_;
	Direction_.z = -1.0;
	this->transform_.rotate_.x -= FastRotateX;
	if (Acceleration_ <= 0)
	{
		Acceleration_ = 0.0f;
		PlayerState_ = S_IDLE;
	}

	PlayerMove();
}

void Player::UpdateWallHit()
{	
	Blown();
	Gravity();

	if (KnockBack_Velocity_.x <= 0.1f || KnockBack_Velocity_.z <= 0.1f)
	{
		CharacterLife_--;
		deadTimer_ = deadTimerValue;
		PlayerState_ = S_IDLE;
		IsInvincibility_ = true;

		SceneManager* pSM = (SceneManager*)FindObject("SceneManager");
		if (pSM->IsBattleScene())
		{
			BattleScene* pBattleScene = (BattleScene*)FindObject("BattleScene");
			//pBattleScene->SetPlayerHp(CharacterLife_);
			pBattleScene->PlusEnemyScore();
		}
	}

	//if (--deadTimer_ < 0)
	//{
	//	//BossBattleScene* pBossBattleScene = (BossBattleScene*)FindObject("BossBattleScene");
	//	//pBossBattleScene->DeadCountPlus();

	//	CharacterLife_--;
	//	deadTimer_ = deadTimerValue;
	//	PlayerState_ = S_IDLE;
	//	IsInvincibility_ = true;

	//	SceneManager* pSM = (SceneManager*)FindObject("SceneManager");
	//	if (pSM->IsBattleScene())
	//	{
	//		BattleScene* pBattleScene = (BattleScene*)FindObject("BattleScene");
	//		pBattleScene->SetPlayerHp(CharacterLife_);
	//	}
	//	else
	//	{

	//	}

	//	//SetStartPosition();
	//}
}

void Player::PlayerMove()
{
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

	//��O�łȂ���Έʒu�X�V 
	XMFLOAT3 f;
	XMStoreFloat3(&f, NewPos_);
	if (!(f.x > 60.0f || f.x < -60.0f || f.z > 60.0f || f.z < -60.0f))
	{
		XMStoreFloat3(&this->transform_.position_, NewPos_);
	}
}

void Player::Blown()
{
	this->transform_.rotate_.x += MoveRotateX;

	//���t���[�����x������
	KnockBack_Velocity_.x *= 0.9;
	KnockBack_Velocity_.z *= 0.9;

	//�ʒu = �ʒu + ���� * ���x
	XMFLOAT3 TmpPos = this->transform_.position_;
	TmpPos.x += KnockBack_Direction_.x * KnockBack_Velocity_.x;
	TmpPos.z += KnockBack_Direction_.z * KnockBack_Velocity_.z;

	NewPos_ = XMLoadFloat3(&TmpPos);

	//��O�łȂ���Έʒu�X�V 
	XMFLOAT3 f;
	XMStoreFloat3(&f, NewPos_);
	if (!(f.x > 60.0f || f.x < -60.0f || f.z > 60.0f || f.z < -60.0f))
	{
		XMStoreFloat3(&this->transform_.position_, NewPos_);
	}
}

void Player::Gravity()
{
	JumpSpeed_ -= Player_Gravity;//�d�͕��̒l�������A�v���C���[�͏�ɉ������ɗ͂��������Ă���
	this->transform_.position_.y += JumpSpeed_;//�t�B�[���h�ɏ���Ă��邩�͊֌W�Ȃ��d�͂͂����葱����

	if (this->transform_.position_.y <= 0.5f)//�v���C���[�߂肱�ݖh�~�Ɉ��ȉ���y���W�Œl���Œ�
	{
		this->transform_.position_.y = 0.5f;
		IsOnGround_ = true;
	}

}

void Player::CameraControl()
{
	if (Input::IsKeyDown(DIK_Q))
	{
		if (CameraState_ == S_NORMALCAMERA)
		{
			CameraState_ = S_DEBUGCAMERA;
		}
		else if (CameraState_ == S_DEBUGCAMERA)
		{
			CameraState_ = S_NORMALCAMERA;
			cameraTransform_.rotate_.x = 0.0f;
		}
	}

	if(CameraState_ == S_NORMALCAMERA)
	{

		if (Input::IsKey(DIK_A) || Input::GetPadStickR().x <= -0.7)	//�J�������E�ړ�
		{
			cameraTransform_.rotate_.y -= 2.5;
		}
		if (Input::IsKey(DIK_D) || Input::GetPadStickR().x >= 0.7)
		{
			cameraTransform_.rotate_.y += 2.5;
		}

		if (Input::IsKey(DIK_W) || Input::GetPadStickR().y <= -0.7)	//�J�����㉺�ړ�
		{
			if (cameraTransform_.rotate_.x >= 60.0f)
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
			this->transform_.rotate_.y = 180;
		}
	}
	else if (CameraState_ == S_DEBUGCAMERA)
	{
		cameraTransform_.rotate_.x = 90.0f;
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

void Player::SetChargingEffect()
{
	EmitterData charge;
	charge.textureFileName = "PaticleAssets\\circle_R.png";
	charge.delay = 0;
	charge.lifeTime = 20;
	charge.position = this->transform_.position_;
	charge.positionRnd = XMFLOAT3(1, 1, 1);
	charge.direction = { 0,1,0 };
	//charge.directionRnd = XMFLOAT3(90, 90, 90);
	charge.speed = 0.15;
	charge.number = (DWORD)1;
	VFX::Start(charge);

}

void Player::SetAttackLocusEffect()
{
	//�v���C���[�̔w��Ɍ��̗��q
	EmitterData locus;
	locus.textureFileName = "PaticleAssets\\flashB_Y.png";
	locus.delay = 0;
	locus.number = (DWORD)3;
	locus.position = this->transform_.position_;
	locus.position.z = this->transform_.position_.z - 0.5f;
	locus.positionRnd = { 1,1,1 };
	locus.direction = { 0,0,1 };
	locus.sizeRnd = { 0.5,0.5 };
	locus.lifeTime = (DWORD)10;
	VFX::Start(locus);

	/*Attackdata_locus.textureFileName = "PaticleAssets\\flashB_Y.png";
	Attackdata_locus.number = (DWORD)3;
	Attackdata_locus.position = this->transform_.position_;
	Attackdata_locus.position.z = this->transform_.position_.z - 0.5f;
	Attackdata_locus.positionRnd = { 1,1,1 };
	Attackdata_locus.direction = { 0,0,1 };
	Attackdata_locus.sizeRnd = { 0.5,0.5 };
	Attackdata_locus.lifeTime = (DWORD)10;
	hAttackEmitLocus_ = VFX::Start(Attackdata_locus);
	LocusEffectCount = 5;
	IsLocusEffect = true;*/
}

void Player::SetHitEffect()
{
	EmitterData hit;
	hit.textureFileName = "PaticleAssets\\flashB_W.png";
	hit.position = this->transform_.position_;
	hit.position.y = this->transform_.position_.y + 1.0f;
	hit.delay = 0;
	hit.direction = { 1,1,0 };
	hit.directionRnd = XMFLOAT3(90, 90, 90);
	hit.speed = 0.5;
	hit.speedRnd = 1.0;
	hit.accel = 1.0;
	hit.lifeTime = 10.0;
	hit.number = (DWORD)10;
	//hit.size = XMFLOAT2(0.1, 0.1);
	hit.sizeRnd = XMFLOAT2(0.4, 0.4);
	//hit.scale = XMFLOAT2(0.99, 0.99);
	//hit.color = XMFLOAT4(1, 1, 0.1, 1);
	//hit.deltaColor = XMFLOAT4(0, 0, 0, 0);
	//hit.gravity = 0.0f;
	VFX::Start(hit);


	/*Hitdata.textureFileName = "PaticleAssets\\flashB_W.png";
	Hitdata.position = this->transform_.position_;
	Hitdata.position.y = this->transform_.position_.y + 1.0f;
	Hitdata.direction = { 1,1,0 };
	Hitdata.directionRnd = { 360,360,0 };
	Hitdata.number = (DWORD)10;
	Hitdata.delay = 5;
	Hitdata.lifeTime = 10;
	hHitEmit_ = VFX::Start(Hitdata);
	HitEffectCount = 5;
	IsHitEffect = true;*/
}

