#include "Enemy.h"

#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"

namespace
{
	float speed_ = 6.0f;
	const int EyeAngle = 60;
	const float EyeLength = 10.0f;
	const float DeltaTime = 0.016f;
}

Enemy::Enemy(GameObject* parent)
	:GameObject(parent,"Enemy"),hEnemy_(-1),pPlayer_(nullptr),IsHit_(false), FrontLength_(EyeLength),
	Eye_(XMConvertToRadians(EyeAngle)),EnemyFrontDirection_({0,0,1})
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{

	hEnemy_ = Model::Load("enemy.fbx");
	assert(hEnemy_ >= 0);
	transform_.position_ = { 10,0,10 };
	//transform_.rotate_.y = 180;

	//��x�N�g��������@0,0,1
	//EnemyFrontDirection = XMVector3TransformCoord(EnemyFrontDirection, GetWorldMatrix());  //getworldmatrix�ŕϊ�


	pPlayer_ = (Player*)FindObject("Player");
	SphereCollider* col = new SphereCollider(XMFLOAT3(0, 0, 0), 0.1f);
	this->AddCollider(col);

}

void Enemy::Update()
{
	pPosition_ = pPlayer_->GetPosition();
	XMFLOAT3 tmp = pPlayer_->GetWorldPosition();
	pPositionVec_ = XMLoadFloat3(&tmp);
	EnemyPosition_ = XMLoadFloat3(&this->transform_.position_);

	switch (EnemyState_)
	{
	case Enemy::S_IDLE:
		UpdateIdle();
		break;
	case Enemy::S_SUSPICIOUS:
		break;
	case Enemy::S_CHASE:
		UpdateChase();
		break;
	case Enemy::S_ATTACK:
		break;
	case Enemy::S_MOVE:
		break;
	case Enemy::S_WINCE:
		break;
	case Enemy::S_MAX:
		break;
	default:
		break;
	}
}

void Enemy::Draw()
{
	Model::SetTransform(hEnemy_, transform_);
	Model::Draw(hEnemy_);
}

void Enemy::Release()
{
}

void Enemy::UpdateIdle()
{	
	//����(enemy)�Ƒ���(player)�̋������͂���(�x�N�g��)
	XMVECTOR DistVec = XMVectorSubtract(EnemyPosition_, pPositionVec_);
	float Pointdist = XMVectorGetX(XMVector3Length(DistVec));

	XMVECTOR forward = RotateVecFront(this->transform_.rotate_.y,EnemyFrontDirection_);//�����̑O���x�N�g��(��]���������܂�)
	DistVec = XMVector3Normalize(DistVec);//�����Ƒ���̋����x�N�g�� ���ς̌v�Z�p
	XMVECTOR dot = XMVector3Dot(DistVec, forward);//����Ƃ̃x�N�g���Ǝ����̑O���x�N�g���̓��ς��Ƃ�
	float cosine = XMVectorGetX(dot);

	if (cosine > cosf(Eye_) && Pointdist < FrontLength_) //������60�x�ȓ�������Ƃ̋�����FrontLength��菬����
	{
		IsHit_ = true;
	}
	else {
		IsHit_ = false;
	}
	
	if (IsHit_)
	{
		EnemyState_ = S_CHASE;
	}
}

void Enemy::UpdateChase()
{
	//����(enemy)�Ƒ���(player)�̋������͂���(�x�N�g��)
	XMVECTOR DistVec = XMVectorSubtract(EnemyPosition_, pPositionVec_);
	float Pointdist = XMVectorGetX(XMVector3Length(DistVec));
	if (Pointdist > FrontLength_ + 10)
	{
		EnemyState_ = S_IDLE;
	}

	//�G�Ǝ��@�̋���
	XMFLOAT3 playerPos = pPlayer_->GetWorldPosition();//�v���C���[�̈ʒu�i���[���h���W�j
	ChasePoint_ = playerPos - this->transform_.position_;//�v���C���[�̈ʒu-�G�̈ʒu�ŋ������Ƃ�

	//�G�Ǝ��@�̉�]����
	XMVECTOR front = EnemyFrontDirection_;//�v�Z�p�̑O�����x�N�g���i�����l������j
	XMMATRIX mvec = transform_.matRotate_;//���݂̉�]���Ă�������i�����̉�]�s��j
	front = XMVector3Transform(front, mvec);
	XMVECTOR PlayerDist = XMLoadFloat3(&ChasePoint_);//�x�N�g���ɂ���
	XMVECTOR normDist = XMVector3Normalize(PlayerDist);//�v���C���[�Ƃ̋����𐳋K��
	XMVECTOR angle = XMVector3AngleBetweenVectors(normDist, front);//��̃x�N�g���Ԃ̃��W�A���p�����߂�
	XMVECTOR cross = XMVector3Cross(front, normDist);

	float crossY= XMVectorGetY(cross);//�O�ς�Y���i+��-�ō��E�ǂ���ɂ��邩���f�j
	
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
	XMVECTOR MoveVector = XMVectorScale(normDist, speed_  * DeltaTime);//�ړ��x�N�g��������
	XMVECTOR PrevPos = EnemyPosition_;
	XMVECTOR NewPos = PrevPos + MoveVector;
	
	XMStoreFloat3(&this->transform_.position_, NewPos);
	this->transform_.position_.y = 1.0f;

	

}

void Enemy::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Player")
	{
	//	pPlayer_->SetPosition({ 0,0,0 });
		EnemyState_ = S_IDLE;
	}
}