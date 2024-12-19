#include "Enemy.h"

#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"

namespace
{
	float speed_ = 3.0f;
	const int EyeAngle = 60;
}

Enemy::Enemy(GameObject* parent)
	:GameObject(parent,"Enemy"),hModel_Enemy(-1),pPlayer_(nullptr),IsHit_(false),
	Fov(XMConvertToRadians(EyeAngle))
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	hModel_Enemy = Model::Load("enemy.fbx");
	transform_.position_ = { 10,1,10 };
	pPlayer_ = (Player*)FindObject("Player");
	

	//SphereCollider* col_eye = new SphereCollider(XMFLOAT3(0, 0, 0), 5.0f);
	//this->AddCollider(col_eye);

}

void Enemy::Update()
{
	EnemyPosition = XMLoadFloat3(&this->transform_.position_);

	switch (enemy_state)
	{
	case Enemy::S_IDLE:
		UpdateIdle();
		break;
	case Enemy::S_SUSPICIOUS:
		break;
	case Enemy::S_CHASE:
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
	Model::SetTransform(hModel_Enemy, transform_);
	Model::Draw(hModel_Enemy);
}

void Enemy::Release()
{
}

void Enemy::UpdateIdle()
{
	XMFLOAT3 a;
	
	XMFLOAT3 pPosition = pPlayer_->GetPosition();
	XMVECTOR pPositionVec = pPlayer_->GetPlayerPosition();
	XMFLOAT3 x;
	XMStoreFloat3(&x, pPositionVec);

	//����(enmey)�Ƒ���(player)�̍��W��̋������͂���(��_�Ԃ̋���)
	//a.x = this->transform_.position_.x - pPosition.x;
	//a.y = this->transform_.position_.y - pPosition.y;
	//a.z = this->transform_.position_.z - pPosition.z;
	//float Pointdist = sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2));

	//����(enmey)�Ƒ���(player)�̋������͂���(�x�N�g��)
	XMVECTOR DistVec = XMVectorSubtract(EnemyPosition, pPositionVec);
	float Pointdist = XMVectorGetX(XMVector3Length(DistVec));

	XMVECTOR forward = RotateVecFront(this->transform_.rotate_.y);//�����̑O���x�N�g��(��]���������܂�)
	DistVec = XMVector3Normalize(DistVec);//�����Ƒ���̋����x�N�g�� ���ς̌v�Z�p
	XMVECTOR dot = XMVector3Dot(DistVec, forward);//����Ƃ̃x�N�g���Ǝ����̑O���x�N�g���̓��ς��Ƃ�
	float cosine = XMVectorGetX(dot);

	if (cosine > cosf(Fov) && Pointdist < FrontLength) //������60�x�ȓ���and����Ƃ̋�����FrontLength��菬����
	{
		IsHit_ = true;
	}
	else {
		IsHit_ = false;
	}
	
	if (IsHit_) {
		transform_.position_.y = 3.0f;
	}
	else {
		transform_.position_.y = 1.0f;
	}

}

void Enemy::UpdateChase()
{

}

void Enemy::OnCollision(GameObject* pTarget)
{
	/*if (pTarget->GetObjectName() == "Player")
	{
		transform_.position_.y = 2.0f;
	}*/
}

//bool Enemy::ComparePosition(XMFLOAT3 pos)
//{
//	//if(transform_.position_.x){}
//}
