#include "Enemy.h"

#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"

namespace
{
	float speed_ = 6.0f;
	const int EyeAngle = 60;
	const float DeltaTime = 0.016f;
}

Enemy::Enemy(GameObject* parent)
	:GameObject(parent,"Enemy"),hModel_Enemy(-1),pPlayer_(nullptr),IsHit_(false),
	Fov(XMConvertToRadians(EyeAngle)),EnemyDirection({0,0,1})
{
	//��x�N�g��������@0,0,1
	EnemyDirection = XMVector3TransformCoord(EnemyDirection,GetWorldMatrix());  //getworldmatrix�ŕϊ�
	//EP�̊m�x���v�Z�@XMVector3AngleBetweenVectors
	//���K������
	//���̕���
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	hModel_Enemy = Model::Load("enemy.fbx");
	transform_.position_ = { 10,1,10 };
	//transform_.rotate_.y = 180;
	pPlayer_ = (Player*)FindObject("Player");
	

	SphereCollider* col = new SphereCollider(XMFLOAT3(0, 0, 0), 0.1f);
	this->AddCollider(col);

}

void Enemy::Update()
{
	pPosition = pPlayer_->GetPosition();
	pPositionVec = pPlayer_->GetPlayerPosition();
	EnemyPosition = XMLoadFloat3(&this->transform_.position_);

	switch (EnemyState)
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
	Model::SetTransform(hModel_Enemy, transform_);
	Model::Draw(hModel_Enemy);
}

void Enemy::Release()
{
}

void Enemy::UpdateIdle()
{	

	XMFLOAT3 x;
	XMStoreFloat3(&x, pPositionVec);

	//����(enmey)�Ƒ���(player)�̍��W��̋������͂���(��_�Ԃ̋���)
	//a.x = this->transform_.position_.x - pPosition.x;
	//a.y = this->transform_.position_.y - pPosition.y;
	//a.z = this->transform_.position_.z - pPosition.z;
	//float Pointdist = sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2));

	//����(enemy)�Ƒ���(player)�̋������͂���(�x�N�g��)
	XMVECTOR DistVec = XMVectorSubtract(EnemyPosition, pPositionVec);
	float Pointdist = XMVectorGetX(XMVector3Length(DistVec));

	XMVECTOR forward = RotateVecFront(this->transform_.rotate_.y,EnemyDirection);//�����̑O���x�N�g��(��]���������܂�)
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
	
	if (IsHit_)
	{
		EnemyState = S_CHASE;
	}
}

void Enemy::UpdateChase()
{
	//�ύX
	XMVECTOR PlayerDist = XMVectorSubtract(pPositionVec,EnemyPosition);
	PlayerDist = XMVector3Normalize(PlayerDist);//�v���C���[�Ƃ̋����𐳋K��
	XMVECTOR normFront = XMVector3Normalize(EnemyDirection);//���ʃx�N�g���𐳋K��
	XMVECTOR cross = XMVector3Cross(normFront,PlayerDist) ;//��L2�̊O�ς��o��
	float crossY= XMVectorGetY(cross);

	
	
	if (crossY > 0)
	{
		transform_.rotate_.y += 1.0f;
	}
	else if (crossY < 0)
	{
		transform_.rotate_.y -= 1.0f;
	}
	else if(fabs(crossY < 1e-6))
	{

	}
	

	//�ύX

	XMFLOAT3 pPos = pPlayer_->GetPosition();//�v���C���[�̈ʒu
	ChasePoint = pPos - this->transform_.position_;//�����ienemy�j�ƃv���C���[�̈ʒu������
	XMVECTOR ChaseVec = XMLoadFloat3(&ChasePoint);//������x�N�g����

	//ChaseVec = XMVector3TransformCoord(ChaseVec, EnemyRot);//��]�s���������
	XMVECTOR norm = XMVector3Normalize(ChaseVec);//���K��

	XMVECTOR MoveVector = XMVectorScale(norm, speed_  * DeltaTime);//�ړ��x�N�g��������
	XMVECTOR PrevPos = EnemyPosition;
	XMVECTOR NewPos = PrevPos + MoveVector;
	XMStoreFloat3(&this->transform_.position_, NewPos);
}

void Enemy::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Player")
	{
		pPlayer_->SetPosition({ 0,0,0 });
		EnemyState = S_IDLE;
	}
}

//bool Enemy::ComparePosition(XMFLOAT3 pos)
//{
//	//if(transform_.position_.x){}
//}
