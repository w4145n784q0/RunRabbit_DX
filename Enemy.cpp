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
	transform_.position_ = { 10,0.5,10 };
	//transform_.rotate_.y = 180;

	//基準ベクトルをつくる　0,0,1
	//EnemyFrontDirection = XMVector3TransformCoord(EnemyFrontDirection, GetWorldMatrix());  //getworldmatrixで変換


	pPlayer_ = (Player*)FindObject("Player");
	SphereCollider* col = new SphereCollider(XMFLOAT3(0, 0, 0), 0.1f);
	this->AddCollider(col);

}

void Enemy::Update()
{
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
	//自分(enemy)と相手(player)の距離をはかる(ベクトル)
	XMVECTOR DistVec = XMVectorSubtract(EnemyPosition_, pPositionVec_);
	float Pointdist = XMVectorGetX(XMVector3Length(DistVec));

	XMVECTOR forward = RotateVecFront(this->transform_.rotate_.y,EnemyFrontDirection_);//自分の前方ベクトル(回転した分も含む)
	DistVec = XMVector3Normalize(DistVec);//自分と相手の距離ベクトル 内積の計算用
	XMVECTOR dot = XMVector3Dot(DistVec, forward);//相手とのベクトルと自分の前方ベクトルの内積をとる
	float cosine = XMVectorGetX(dot);

	if (cosine > cosf(Eye_) && Pointdist < FrontLength_) //距離は60度以内か相手との距離がFrontLengthより小さい
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
	//自分(enemy)と相手(player)の距離をはかる(ベクトル)
	XMVECTOR DistVec = XMVectorSubtract(EnemyPosition_, pPositionVec_);
	float Pointdist = XMVectorGetX(XMVector3Length(DistVec));
	if (Pointdist > FrontLength_ + 10)
	{
		EnemyState_ = S_IDLE;
	}

	//敵と自機の距離
	XMFLOAT3 playerPos = pPlayer_->GetWorldPosition();//プレイヤーの位置（ワールド座標）
	ChasePoint_ = playerPos - this->transform_.position_;//プレイヤーの位置-敵の位置で距離をとる

	//敵と自機の回転処理
	XMVECTOR front = EnemyFrontDirection_;//計算用の前向きベクトル（初期値が入る）
	XMMATRIX mvec = transform_.matRotate_;//現在の回転している方向（自分の回転行列）
	front = XMVector3Transform(front, mvec);
	XMVECTOR PlayerDist = XMLoadFloat3(&ChasePoint_);//ベクトルにする
	XMVECTOR normDist = XMVector3Normalize(PlayerDist);//プレイヤーとの距離を正規化
	XMVECTOR angle = XMVector3AngleBetweenVectors(normDist, front);//二つのベクトル間のラジアン角を求める
	XMVECTOR cross = XMVector3Cross(front, normDist);

	float crossY= XMVectorGetY(cross);//外積のY軸（+か-で左右どちらにいるか判断）
	
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
	XMVECTOR MoveVector = XMVectorScale(normDist, speed_  * DeltaTime);//移動ベクトル化する
	XMVECTOR PrevPos = EnemyPosition_;
	XMVECTOR NewPos = PrevPos + MoveVector;
	
	XMStoreFloat3(&this->transform_.position_, NewPos);
	this->transform_.position_.y = 0.5f;

	

}

void Enemy::OnCollision(GameObject* pTarget)
{
	EnemyState_ = S_IDLE;
	pPlayer_->SetStartPosition();
}