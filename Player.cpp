#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

#include"Enemy.h"

#include <algorithm>
#include<list>

namespace {
	const float speed = 9.0f;
	const float Player_Gravity = 0.08; //0.16333f
	const float DeltaTime = 0.016f;
	const float FullAccelerate = 50.0f;
	XMVECTOR BackCameraPos = { 0,2,-10,0 };//BackCameraの値は変わるが毎フレームこの値にする（値が変わり続けるのを防ぐ）
	
	const float TreeCollision = 4.0f;
	float PrevHeight = 0.0f;
	
}

Player::Player(GameObject* parent)
	:GameObject(parent, "Player"), hPlayer_(-1), hLandingPoint_(-1), IsOnGround_(false), IsDash_(false),
	JumpSpeed_(0.0f), LandingPoint({ 0,0,0 }),
	Direction_({ 0,0,0 }), PlayerFrontDirection_({ 0,0,1 }), PlayerPosition_({ 0,0,0 }), Acceleration_(0.0f),
	BackCamera_(BackCameraPos), pGround_(nullptr), pTerrain_(nullptr),
	PlayerState_(S_IDLE), CanMove_(true),PlayerHeight_(0)
{
	cameraTransform_ = this->transform_;
	CameraPosition_ = { this->transform_.position_.x ,this->transform_.position_.y + 1, this->transform_.position_.z - 8 };
	CameraTarget_ = { this->transform_.position_.x,this->transform_.position_.y, this->transform_.position_.z };

	JumpTransform_.position_ = { 0.0f,1.0f,0.0f };

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

	SetStartPosition();

	pGround_ = (Ground*)FindObject("Ground");
	pTerrain_ = (Terrain*)FindObject("Terrain");
	
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

	//--------------カメラ追従--------------
	CameraTarget_ = { this->transform_.position_ };//カメラの位置は自機の位置に固定
	XMMATRIX rotY = XMMatrixRotationY(XMConvertToRadians(cameraTransform_.rotate_.y));//カメラの回転行列をつくり
	BackCamera_ = XMVector3TransformCoord(BackCamera_, rotY);//バックカメラのベクトルにかける
	XMStoreFloat3(&CameraPosition_, NewPos_ + BackCamera_);//移動ベクトルと加算

	Camera::SetPosition(CameraPosition_);//カメラの位置をセット 
	Camera::SetTarget(CameraTarget_);//カメラの焦点をセット
	BackCamera_ = { BackCameraPos };//バックカメラベクトルをリセット

}

void Player::Draw()
{
	Model::SetTransform(hPlayer_, transform_);
	Model::Draw(hPlayer_);

	Transform t;
	t.position_ = PlayerFront;

	//デバッグ用 正面に円の描画
	Model::SetTransform(hLandingPoint_, t);
	Model::Draw(hLandingPoint_);

	ImGui::Text("PositionX:%.3f", this->transform_.position_.x);
	ImGui::Text("PositionY:%.3f", this->transform_.position_.y);
	ImGui::Text("PositionZ:%.3f", this->transform_.position_.z);

	ImGui::Text("IsOnGround:%.1f", IsOnGround_);

	if (PlayerState_ == S_JUMPBEFORE)
	{
		Model::SetTransform(hLandingPoint_, JumpTransform_);
		Model::Draw(hLandingPoint_);
	}
}

void Player::Release()
{
}

void Player::OnCollision(GameObject* pTarget)
{
	/*if (pTarget->GetObjectName() == "Enemy")
	{
		transform_.position_ = StartPosition;
	}*/
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

	//プレイヤーの正面ベクトルを更新
	// //自分の前方ベクトル(回転した分も含む)
	XMVECTOR forward = RotateVecFront(this->transform_.rotate_.y, PlayerFrontDirection_);

	XMFLOAT3 rot = { 0,0,0 };
	XMStoreFloat3(&rot, forward);
	PlayerFront = { transform_.position_ + rot };

	//--------------ダッシュ関係--------------
	if (Input::IsKey(DIK_LSHIFT) || Input::IsKey(DIK_RSHIFT))
	{
		IsDash_ = true;
	}
	else
	{
		IsDash_ = false;
	}

	Dash();

	//プレイヤーのy回転をラジアン化して行列に
	XMMATRIX playerRot = XMMatrixRotationY(XMConvertToRadians(this->transform_.rotate_.y));

	//プレイヤーの進行方向をベクトル化
	XMVECTOR PrevDir = XMVectorSet(Direction_.x, Direction_.y, Direction_.z, 0.0f);

	//方向ベクトルを回転行列で変換
	PrevDir = XMVector3TransformCoord(PrevDir, playerRot);

	//単位ベクトル化する
	XMVECTOR norm = XMVector3Normalize(PrevDir);

	//移動ベクトル化する
	XMVECTOR MoveVector = XMVectorScale(norm, (speed + Acceleration_) * DeltaTime);

	//現在位置と移動ベクトルを加算
	XMVECTOR PrevPos = PlayerPosition_;
	NewPos_ = PrevPos + MoveVector;

	//int nextX, nextZ;
	//nextX = (int)XMVectorGetX(NewPos_) + 1.0;
	//nextY = (int)XMVectorGetY(NewPos_);
	//nextZ = (int)XMVectorGetZ(NewPos_) + 1.0;

	XMStoreFloat3(&this->transform_.position_, NewPos_);






	//--------------ジャンプ--------------
	//ボタンを押すとジャンプの着地先を表示
	//WASDで着地場所を細かく設定

	//地面にいるときは座標固定
	//LandGround();

	//通常ジャンプ
	if (Input::IsKeyDown(DIK_SPACE))
	{
		//PlayerState = S_JUMP;

		if (IsOnGround_)
		{
			IsOnGround_ = false;
			PrevHeight = transform_.position_.y;
			JumpSpeed_ = 1.2;//一時的にy方向にマイナスされている値を大きくする
		}


	}



	int hGroundModel = pGround_->GetModelHandle();    //モデル番号を取得
	RayCastData data;
	data.start = transform_.position_;
	//data.start.y = 0;
	data.dir = XMFLOAT3({ 0,-1,0 });
	Model::RayCast(hGroundModel, &data);
	if (data.hit == true)
	{
		transform_.position_.y -= data.dist;
		IsOnGround_ = true;
	}
	else
	{
		IsOnGround_ = false;
	}

	if (!IsOnGround_){
		JumpSpeed_ -= Player_Gravity;//重力分の値を引き、プレイヤーは常に下方向に力がかかっている
		this->transform_.position_.y += JumpSpeed_;
	}

	/*if (this->transform_.position_.y <= 0.0f) {
		this->transform_.position_.y = 0.0f;
		IsOnGround_ = false;
	}*/

	if (this->transform_.position_.y < -200) {
		this->transform_.position_.y = -200;
	}

	if (Input::IsKeyDown(DIK_ESCAPE))
	{
		SetStartPosition();
	}


	//if (this->transform_.position_.y <= 0.0f) //プレイヤーめりこみ防止に一定以下のy座標で値を固定
	//{
	//	this->transform_.position_.y = 0.0f;
	//	IsOnGround_ = true;
	//}

	
	
	//狙ったところにジャンプ
	if (Input::IsKeyDown(DIK_Q))
	{
		JumpTransform_ = this->transform_;
		LandingPoint = this->transform_.position_;
		PlayerState_ = S_JUMPBEFORE;
	}

	CameraControl();

	Direction_ = { 0,0,0 };//最後に進行方向のリセット毎フレーム行う
}

void Player::UpdateJumpBefore()
{
	CameraControl();
	if (Input::IsKeyDown(DIK_Z))
	{
		PlayerState_ = S_IDLE;
	}

	if (Input::IsKey(DIK_UP))
	{
		LandingPoint.z += 0.3f;
	}
	if (Input::IsKey(DIK_DOWN))
	{
		LandingPoint.z -= 0.3f;
	}
	if (Input::IsKey(DIK_LEFT))
	{
		LandingPoint.x -= 0.3f;
	}
	if (Input::IsKey(DIK_RIGHT))
	{
		LandingPoint.x += 0.3f;
	}

	JumpTransform_.position_ = LandingPoint;

	if (Input::IsKeyDown(DIK_SPACE))
	{
		//JumpSpeed_ = 1.2;
		PlayerStart_ = PlayerPosition_;
		JumpTarget_ = XMLoadFloat3(&JumpTransform_.position_);//ジャンプの着地点
		JumpLength_ = JumpTarget_ - PlayerPosition_;//ジャンプする距離
		XMStoreFloat3(&JumpValue, JumpLength_);


		//JumpLength_ = XMVector3Normalize(JumpLength_);

		//XMStoreFloat3(&this->transform_.position_, v);//一時的な移動処理
		PlayerState_ = S_JUMP;
	}
}

void Player::UpdateJump()
{
	XMStoreFloat3(&this->transform_.position_, JumpTarget_);
	PlayerState_ = S_IDLE;
}

void Player::UpdateHit()
{
}

void Player::CameraControl()
{
	if (Input::IsKey(DIK_X))
	{
		cameraTransform_.rotate_.y -= 5;
	}
	if (Input::IsKey(DIK_C))
	{
		cameraTransform_.rotate_.y += 5;
	}
	if (Input::IsKey(DIK_Z))//カメラを正面に戻す（方向に変化なし）
	{
		cameraTransform_.rotate_.y = 0;
		this->transform_.rotate_.y = 0;
	}
}
