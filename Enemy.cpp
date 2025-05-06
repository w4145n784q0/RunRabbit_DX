#include "Enemy.h"

#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"
#include"BattleScene.h"
#include"Engine/SceneManager.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace
{
	float speed = 6.0f;
	const int EyeAngle = 60;
	const float EyeLength = 10.0f;
	const float DeltaTime = 0.016f;
	const float ChaseLength = 50.0f;

	const float FullAccelerate = 50.0f;//�ő�����x
	const float MoveRotateX = 10.0f;//�ړ�����1f�̉�]��
	const float FastRotateX = 30.0f;//(�`���[�W���Ȃ�)������]����1f�̉�]��

	const float Enemy_Gravity = 0.08; //0.16333f
	const float KnockBackPower = 2.0f; //�m�b�N�o�b�N���鋭��
	const int EnemyAttackTime = 180;//�G���U������܂ł̎���

	const int deadTimerValue = 60;//�����܂ł̎���
	const int Invincibility = 120;//���G���Ԃ̒萔

	const XMVECTOR EnemyFrontDirection = { 0,0,1 };//�G�̐��ʂ̏����l ��������ǂꂾ����]������
}

Enemy::Enemy(GameObject* parent)
	:GameObject(parent, "Enemy"), hEnemy_(-1), pPlayer_(nullptr), IsHit_(false), FrontLength_(EyeLength)
    ,ForwardVector_({0,0,0}),
	Eye_(XMConvertToRadians(EyeAngle)), IsOnGround_(true),Acceleration_(0.0f), AcceleValue_(1.0f),
	HitStopTimer_(0), deadTimer_(deadTimerValue),IsInvincibility_(false),InvincibilityTime_(Invincibility),ColliderSize_(1.5f), CharacterLife_(3)
{
	transform_.position_ = { 0,0,0 };
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{

	hEnemy_ = Model::Load("chara2.fbx");
	assert(hEnemy_ >= 0);

	transform_.position_ = { 0.0,0.5 ,5.0 };
	//transform_.scale_ = { 1.5,1.5,1.5 };
	//transform_.rotate_.y = 180.0;

	//��x�N�g��������@0,0,1
	//EnemyFrontDirection = XMVector3TransformCoord(EnemyFrontDirection, GetWorldMatrix());  //getworldmatrix�ŕϊ�


	pPlayer_ = (Player*)FindObject("Player");
	pGround_ = (Ground*)FindObject("Ground");
	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), ColliderSize_);
	this->AddCollider(collision);

	EnemyState_ = S_IDLE;

}

void Enemy::Update()
{
	PlayerPosition_ = pPlayer_->GetWorldPosition();//�v���C���[�̈ʒu�i���[���h���W�j
	pPositionVec_ = XMLoadFloat3(&PlayerPosition_);//�v���C���[�̈ʒu���x�N�g��������葱����
	EnemyPosition_ = XMLoadFloat3(&this->transform_.position_);//�G�̈ʒu���x�N�g��������葱����
	ForwardVector_ = RotateVecFront(this->transform_.rotate_.y, EnemyFrontDirection);//�����̑O���x�N�g��(��]���������܂�)���X�V

	switch (EnemyState_)
	{
	case Enemy::S_IDLE:
		UpdateIdle();
		break;
	case Enemy::S_ROOT:
		UpdateRoot();
		break;
	case Enemy::S_CHASE:
		UpdateChase();
		break;
	case Enemy::S_ATTACK:
		UpdateAttack();
		break;
	case Enemy::S_HITSTOP:
		UpdateHitStop();
		break;
	case Enemy::S_HIT:
		UpdateHit();
		break;
	case Enemy::S_WALLHIT:
		UpdateWallHit();
		break;
	case Enemy::S_AIM:
		UpdateAim();
		break;
	case Enemy::S_ONALEAT:
		UpdateOnAlert();
		break;
	case Enemy::S_MAX:
		break;
	default:
		break;
	}

	if (!IsInvincibility_ && !(EnemyState_ == S_WALLHIT))//�ǃ_���[�W����
	{
		if (transform_.position_.x > 60.0f || transform_.position_.x < -60.0f ||
			transform_.position_.z > 60.0f || transform_.position_.z < -60.0f)
		{
			EnemyState_ = S_WALLHIT;
		}
	}

	//���G���Ԃ̌v�Z
	if (IsInvincibility_)
	{
		if (--InvincibilityTime_ < 0)
		{
			InvincibilityTime_ = Invincibility;
			IsInvincibility_ = false;
		}
	}

	JumpSpeed_ -= Enemy_Gravity;//�d�͕��̒l�������A�v���C���[�͏�ɉ������ɗ͂��������Ă���
	this->transform_.position_.y += JumpSpeed_;//�t�B�[���h�ɏ���Ă��邩�͊֌W�Ȃ��d�͂͂����葱����

	if (this->transform_.position_.y <= 0.5f && IsOnGround_)//�߂肱�ݖh�~�Ɉ��ȉ���y���W�Œl���Œ�
	{
		this->transform_.position_.y = 0.5f;
	}

}

void Enemy::Draw()
{
	Model::SetTransform(hEnemy_, transform_);
	Model::Draw(hEnemy_);

#ifdef _DEBUG
	if (ImGui::Button("EnemystateChange"))
	{
		if (!EnemyState_ == S_IDLE)
			EnemyState_ = S_IDLE;
		else
			EnemyState_ = S_AIM;

	}

	/*XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, ForwardVector_);

	ImGui::Text("front.x:%3f", (float)tmp.x);
	ImGui::Text("front.y:%3f", (float)tmp.y);
	ImGui::Text("front.z:%3f", (float)tmp.z);*/

	ImGui::Text("EnemyLife:%.3f", (float)CharacterLife_);
#endif
}

void Enemy::Release()
{
}

void Enemy::UpdateIdle()
{	
	////����(enemy)�Ƒ���(player)�̋������͂���(�x�N�g��)
	//XMVECTOR DistVec = XMVectorSubtract(EnemyPosition_, pPositionVec_);
	//float Pointdist = XMVectorGetX(XMVector3Length(DistVec));

	//XMVECTOR forward = RotateVecFront(this->transform_.rotate_.y,EnemyFrontDirection_);//�����̑O���x�N�g��(��]���������܂�)
	//DistVec = XMVector3Normalize(DistVec);//�����Ƒ���̋����x�N�g�� ���ς̌v�Z�p
	//XMVECTOR dot = XMVector3Dot(DistVec, forward);//����Ƃ̃x�N�g���Ǝ����̑O���x�N�g���̓��ς��Ƃ�
	//float cosine = XMVectorGetX(dot);

	//if (cosine > cosf(Eye_) && Pointdist < FrontLength_) //������60�x�ȓ�������Ƃ̋�����FrontLength��菬����
	//{
	//	IsHit_ = true;
	//}
	//else {
	//	IsHit_ = false;
	//}
	//
	//if (IsHit_)
	//{
	//	EnemyState_ = S_CHASE;
	//}
}

void Enemy::UpdateRoot()
{
	float dist = PlayerEnemyDistanceX();

	if (dist > ChaseLength)//��苗���ȏ㗣��Ă���Ȃ�ǐ�
	{
		EnemyState_ = S_CHASE;
	}
	else//�ڋ߂��Ă���Ȃ�U������
	{
		EnemyState_ = S_AIM;
	}
}

void Enemy::UpdateChase()
{
	LookPlayer();
	XMVECTOR MoveVector = XMVectorScale(AttackVector_, speed * DeltaTime);//�ړ��x�N�g��������
	XMVECTOR PrevPos = EnemyPosition_;
	XMVECTOR NewPos = PrevPos + MoveVector;
	
	XMStoreFloat3(&this->transform_.position_, NewPos);
	//this->transform_.position_.y = 0.5f;

	float dist = PlayerEnemyDistanceX();
	if (dist < ChaseLength)
	{
		EnemyState_ = S_AIM;
	}
}

void Enemy::UpdateHitStop()
{
	//�q�b�g�X�g�b�v���p���̂ݎg�p
	if (++HitStopTimer_ > 2)
	{
		HitStopTimer_ = 0;
		EnemyState_ = S_HIT;
	}
}

void Enemy::UpdateHit()
{
	Blown();
	if (KnockBack_Velocity_.x <= 0.5f || KnockBack_Velocity_.z <= 0.5f)
	{
		transform_.rotate_.x = 0.0f;
		EnemyState_ = S_ROOT;
	}
}

void Enemy::UpdateWallHit()
{
	Blown();
	if (KnockBack_Velocity_.x <= 0.1f || KnockBack_Velocity_.z <= 0.1f)
	{
		transform_.rotate_.x = 0.0f;
		CharacterLife_--;
		deadTimer_ = deadTimerValue;
		EnemyState_ = S_ROOT;
		IsInvincibility_ = true;

		SceneManager* pSM = (SceneManager*)FindObject("SceneManager");
		if (pSM->IsBattleScene())
		{
			BattleScene* pBattleScene = (BattleScene*)FindObject("BattleScene");
			//pBattleScene->SetPlayerHp(CharacterLife_);
			pBattleScene->PlusPlayerScore();
		}
	}

	/*if (--deadTimer_ < 0)
	{
		CharacterLife_--;
		deadTimer_ = deadTimerValue;
		EnemyState_ = S_ROOT;
		IsInvincibility_ = true;
		this->transform_.position_ = { 0,0,0 };
	}*/
}

void Enemy::UpdateAim()
{
	LookPlayer();
	//this->transform_.rotate_.x -= ChargeRotateX;

	if (++AimTimer_ > EnemyAttackTime)
	{
		AimTimer_ = 0;
		Acceleration_ = FullAccelerate;
		EnemyState_ = S_ATTACK;
	}

}

void Enemy::UpdateOnAlert()
{
	LookPlayer();
}

void Enemy::Blown()
{
	this->transform_.rotate_.x -= FastRotateX;

	//���x�������Ă���
	KnockBack_Velocity_.x *= 0.9;
	KnockBack_Velocity_.z *= 0.9;

	//���t���[��position�ɕ��������Z
	//�ʒu = �ʒu + ���� * ���x
	transform_.position_.x += KnockBack_Direction_.x * KnockBack_Velocity_.x;
	transform_.position_.z += KnockBack_Direction_.z * KnockBack_Velocity_.z;
}

void Enemy::UpdateAttack()
{
	//�ړ��x�N�g�����v�Z(���� * ���x(���� + ����))
	XMVECTOR MoveVector = XMVectorScale(AttackVector_,(speed + Acceleration_) * DeltaTime);

	//�G�̈ʒu�Ɉړ��x�N�g�������Z
	XMVECTOR PrevPos = EnemyPosition_;
	XMVECTOR NewPos = PrevPos + MoveVector;

	XMStoreFloat3(&this->transform_.position_, NewPos);

	//���x�𖈃t���[������
	Acceleration_ -= AcceleValue_;

	//�m�b�N�o�b�N����]
	this->transform_.rotate_.x -= FastRotateX;

	if (Acceleration_ <= 0.0f)
	{
		transform_.rotate_.x = 0.0f;
		EnemyState_ = S_ROOT;
	}
}

void Enemy::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Fence")
	{
		if (!IsInvincibility_ && !(EnemyState_ == S_WALLHIT))
		{
		//	KnockBack_Velocity_ = { 0,0,0 };
			Acceleration_ = 0.0f;
			XMFLOAT3 inverse;
			XMStoreFloat3(&inverse, ForwardVector_);
			KnockBack_Direction_ = { inverse.x * -1, inverse.y * -1, inverse.z * -1 };
			KnockBack_Velocity_.x = KnockBackPower;
			KnockBack_Velocity_.z = KnockBackPower;

			EnemyState_ = S_WALLHIT;
		}
	}
}

void Enemy::PlayerReflect(XMVECTOR _vector,bool _isDush)
{
	XMFLOAT3 f;
	XMStoreFloat3(&f, _vector);
	KnockBack_Direction_ = f;

	if (_isDush)
	{
		KnockBack_Velocity_.x = KnockBackPower * 1.5;
		KnockBack_Velocity_.z = KnockBackPower * 1.5;
	}
	else
	{
		KnockBack_Velocity_.x = KnockBackPower;
		KnockBack_Velocity_.z = KnockBackPower;
	}

	EnemyState_ = S_HIT;
}

void Enemy::LookPlayer()
{
	//����(enemy)�Ƒ���(player)�̋������͂���(�x�N�g��)
	//XMVECTOR DistVec = XMVectorSubtract(EnemyPosition_, pPositionVec_);
	//float Pointdist = XMVectorGetX(XMVector3Length(DistVec));

	//�G�Ǝ��@�̋����i���W�j
	XMFLOAT3 LookPoint = PlayerEnemyDistanceFloat3();//�v���C���[�̈ʒu-�G�̈ʒu�ŋ������Ƃ�

	//�G�Ǝ��@�̉�]����
	XMVECTOR front = EnemyFrontDirection;//�v�Z�p�̑O�����x�N�g���i�����l������j
	XMMATRIX mvec = transform_.matRotate_;//���݂̉�]���Ă�������i�����̉�]�s��j
	front = XMVector3Transform(front, mvec);//���ʂ���ǂꂾ����]���Ă��邩

	XMVECTOR PlayerDist = XMLoadFloat3(&LookPoint);//�x�N�g���ɂ���
	XMVECTOR normDist = XMVector3Normalize(PlayerDist);//�v���C���[�Ƃ̋����𐳋K��
	XMVECTOR angle = XMVector3AngleBetweenVectors(normDist, front);//��̃x�N�g���Ԃ̃��W�A���p�����߂�
	XMVECTOR cross = XMVector3Cross(front, normDist);

	AttackVector_ = normDist;//�U��������ۑ�

	float crossY = XMVectorGetY(cross);//�O�ς�Y���i+��-�ō��E�ǂ���ɂ��邩���f�j

	//float angleX = XMVectorGetX(angle);
	//float dig = XMConvertToDegrees(angleX);

	float Dig = XMConvertToDegrees(XMVectorGetX(angle));
	if (Dig > 3)
	{
		if (crossY > 0.0)
		{
			transform_.rotate_.y -= 1.5f;
		}
		else if (crossY < 0.0)
		{
			transform_.rotate_.y += 1.5f;
		}
	}

	transform_.Calclation();
}

XMFLOAT3 Enemy::PlayerEnemyDistanceFloat3()
{
	XMFLOAT3 dist = PlayerPosition_ - this->transform_.position_;
	return dist;
}

float Enemy::PlayerEnemyDistanceX()
{
	XMVECTOR DistVec = XMVectorSubtract(EnemyPosition_, pPositionVec_);
	float tmp = XMVectorGetX(XMVector3Length(DistVec));
	return tmp;
}
