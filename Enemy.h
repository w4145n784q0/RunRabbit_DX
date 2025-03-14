#pragma once
#include "Engine/GameObject.h"
#include"Player.h"

class Player;

class Enemy :
    public GameObject
{
private:
	//モデルハンドル
    int hEnemy_;

	//インスタンス関係
	Player* pPlayer_;

	//ステート
	enum State {
		S_IDLE = 0,//待機
		//S_SUSPICIOUS,//怪しむ
		S_CHASE,//追いかける
		S_ATTACK,//攻撃
		S_MOVE,//移動
		S_WINCE,//ひるむ
		S_HIT,
		S_MAX
	};
	State EnemyState_;

	//追跡関係
	XMVECTOR EnemyFrontDirection_;//敵の正面ベクトル
	XMVECTOR EnemyPosition_;//敵の位置ベクトル
	float RotateY_;//回転角度（基本Ｙ軸のみ）
	float FrontLength_;//敵の視界の長さ
	float Eye_;
	bool IsHit_;
	bool IsOnGround_;//地面にいるか
	float JumpSpeed_;//+ならジャンプしている状態 -なら下降〜地面にいる状態
	XMVECTOR pPositionVec_;//プレイヤーの位置を保管
	XMFLOAT3 ChasePoint_;

	//ヒット関係
	XMFLOAT3 ReflectMove;
public:
	Enemy(GameObject* parent);
	~Enemy();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void UpdateIdle();
	void UpdateSuspicious();
	void UpdateChase();
	void UpdateAttack();
	void UpdateMove();
	void UpdateWince();
	void UpdateHit();

	void OnCollision(GameObject* pTarget) override;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="_vector">プレイヤーから渡されたノックバックする方向(正規化済)</param>
	void PlayerReflect(XMVECTOR _vector);

	void SetState(State s) { EnemyState_ = s; }
	State GetState() { return EnemyState_; }
};

