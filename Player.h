#pragma once
#include "Engine/GameObject.h"
#include"Ground.h"

namespace {
	const XMFLOAT3 StartPosition = { 0.0f,0.5f,0.0f };
}

class Player :
    public GameObject
{
private:
	//モデルハンドル
	int hPlayer_;

	int hPlayerIdle_;//待機モーション
	int hPlayerMove_;//移動
	int hPlayerDash_;//ダッシュ
	int hPlayerHit_;//攻撃を食らった
	int hPlayerDead_;//倒れる


	//音ハンドル
	int hCollisionSound_;

	//インスタンス関係
	Ground* pGround_;

	//ステート
	enum State
	{
		S_IDLE,
		S_HIT,
		S_CHARGE,
		S_ATTACK,
		S_OUT,
		S_WALLHIT,
		S_DEAD,
		S_MAX
	};
	State PlayerState_;

	//ジャンプ関係
	bool IsOnGround_;
	float JumpSpeed_;//+ならジャンプしている状態 -なら下降〜地面にいる状態


	//ダッシュ関係
	bool IsDash_; //ダッシュ中か
	bool IsDashStart_ = false;//ダッシュ開始したか
	float Acceleration_;//加速度
	
	//移動関係
	bool CanMove_;//移動できるか
	int PlayerHeight_;//プレイヤーの高さ
	XMFLOAT3 Direction_;//プレイヤーの方向(xzどこに進むか)
	XMVECTOR PlayerPosition_;//位置ベクトル
	XMVECTOR NewPos_;//プレイヤーの移動先
	XMFLOAT3 PlayerFront;//プレイヤーの正面位置 当たり判定用
	XMVECTOR ForwardVector_;//プレイヤーの正面ベクトル

	//カメラ関係
	XMFLOAT3 CameraPosition_;
	XMFLOAT3 CameraTarget_;
	Transform cameraTransform_;//カメラのTransform 回転だけ使う
	XMVECTOR BackCamera_;//プレイヤーの後ろに置くカメラの位置

	//被弾(HIT)関係
	XMFLOAT3 KnockBack_Direction_;//ノックバックする方向
	XMFLOAT3 KnockBack_Velocity_;//ノックバックする速度

	//ダメージ関係
	int deadTimer_;//復活までの時間
	int InvincibilityTime_;//ダメージ後の無敵時間
	bool IsInvincibility_;//無敵時間か

public:

	XMVECTOR GetPlayerDirection() { return ForwardVector_; }//方向ベクトル取得
	XMVECTOR GetPlayerPosition() { return PlayerPosition_; }//位置ベクトル取得

	Player(GameObject* parent);
	~Player();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;

	void UpdateIdle();
	//void UpdateJump();
	void UpdateHit();
	void UpdateCharge();
	void UpdateAttack();
	void UpdateOut();
	void UpdateWallHit();
	void UpdateDead();
	void Dash();
	void PlayerStop() { PlayerState_ = S_MAX; }
	
	/// <summary>
	/// カメラの操作（一部のステートのみ）
	/// </summary>
	void CameraControl();

	/// <summary>
	/// 敵からはじかれる処理
	/// </summary>
	/// <param name="_vector">敵からノックバックする方向(正規化済)</param>
	/// <param name="_IsAttack">敵が攻撃中か</param>

	void EnemyReflect(XMVECTOR _vector, bool _IsAttack);

	void SetStartPosition() { this->transform_.position_ = StartPosition; }
};

