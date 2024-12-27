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
	Fov(XMConvertToRadians(EyeAngle)),EnemyFrontDirection({0,0,1})
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{

	hModel_Enemy = Model::Load("enemy.fbx");
	transform_.position_ = { 10,1,10 };
	//transform_.rotate_.y = 180;

	//��x�N�g��������@0,0,1
	//EnemyFrontDirection = XMVector3TransformCoord(EnemyFrontDirection, GetWorldMatrix());  //getworldmatrix�ŕϊ�


	pPlayer_ = (Player*)FindObject("Player");
	SphereCollider* col = new SphereCollider(XMFLOAT3(0, 0, 0), 0.1f);
	this->AddCollider(col);

}

void Enemy::Update()
{
	pPosition = pPlayer_->GetPosition();
	XMFLOAT3 tmp = pPlayer_->GetWorldPosition();
	pPositionVec = XMLoadFloat3(&tmp);
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

	//����(enmey)�Ƒ���(player)�̍��W��̋������͂���(��_�Ԃ̋���)
	//a.x = this->transform_.position_.x - pPosition.x;
	//a.y = this->transform_.position_.y - pPosition.y;
	//a.z = this->transform_.position_.z - pPosition.z;
	//float Pointdist = sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2));

	//����(enemy)�Ƒ���(player)�̋������͂���(�x�N�g��)
	XMVECTOR DistVec = XMVectorSubtract(EnemyPosition, pPositionVec);
	float Pointdist = XMVectorGetX(XMVector3Length(DistVec));

	XMVECTOR forward = RotateVecFront(this->transform_.rotate_.y,EnemyFrontDirection);//�����̑O���x�N�g��(��]���������܂�)
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


	XMVECTOR front = EnemyFrontDirection;//�v�Z�p�̑O�����x�N�g���i�����l������j
	XMMATRIX mvec = transform_.matRotate_;//���݂̉�]���Ă�������i�����̉�]�s��j
	front = XMVector3Transform(front, mvec);

	XMFLOAT3 playerPos = pPlayer_->GetWorldPosition();//�v���C���[�̈ʒu�i���[���h���W�j
	ChasePoint = playerPos - this->transform_.position_;//�v���C���[�̈ʒu-�G�̈ʒu�ŋ������Ƃ�
	XMVECTOR PlayerDist = XMLoadFloat3(&ChasePoint);//�x�N�g���ɂ���
	/*XMVECTOR PlayerDist = { playerPos.x - transform_.position_.x, playerPos.y - transform_.position_.y,
		playerPos.z - transform_.position_.z };*/
	XMVECTOR normDist = XMVector3Normalize(PlayerDist);//�v���C���[�Ƃ̋����𐳋K��
	XMVECTOR angle = XMVector3AngleBetweenVectors(normDist, front);//��̃x�N�g���Ԃ̃��W�A���p�����߂�
	XMVECTOR cross = XMVector3Cross(front, PlayerDist);

	//XMVECTOR normFront = XMVector3Normalize(front);//���ʃx�N�g���𐳋K��
	//XMVECTOR cross = XMVector3Cross(normFront,PlayerDist) ;//��L2�̊O�ς��o��
	//cross = XMVector3Normalize(cross);//���K��

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

	//ChasePoint = playerPos - this->transform_.position_;//�����ienemy�j�ƃv���C���[�̈ʒu������
	//XMVECTOR ChaseVec = XMLoadFloat3(&ChasePoint);//������x�N�g����
	//XMVECTOR norm = XMVector3Normalize(ChaseVec);//���K��

	XMVECTOR MoveVector = XMVectorScale(normDist, speed_  * DeltaTime);//�ړ��x�N�g��������
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