#pragma once
#include "Engine/GameObject.h"
#include "Engine/CsvReader.h"
#include "Ground.h"

//プレイヤー,敵クラスの共通事項クラス
class Character :
    public GameObject
{
private:
    //CsvReader csv_;//csvを読み込むインスタンス
protected:
    //----------初期状態----------
    XMFLOAT3 StartPosition_;//初期位置
    XMVECTOR FrontDirection_;//正面の初期値(ローカル座標系) ここからどれだけ回転したか

    //----------移動----------
    float Velocity_;//初速度 この速度に加速度が加算される
    float Acceleration_;//加速度
    float AcceleValue_;//Acceleration_上昇時、1fあたりの増加量
    float FullAccelerate_;//加速度の最大
    XMVECTOR ForwardVector_;//キャラクターから見た正面の方向(ワールド座標系) 自身のy軸回転量とかけて計算 正規化した値を入れる
    XMVECTOR MoveDirection_;//移動方向 この値に速さの要素をかけて移動ベクトル化する
    XMVECTOR NewPositon_;//移動後の位置ベクトル

    //----------回転----------
    float MoveRotateX;//移動時の1fの回転量
    float FastRotateX;//(チャージ中など)高速回転中の1fの回転量

    //----------空中----------
    float Gravity_; //重力 キャラクターの下方向にかかる力
    bool IsOnGround_;//地面にいるか
    float JumpSpeed_; //プレイヤーの上方向に向く力 +ならジャンプしている状態 -なら下降〜地面にいる状態
    float HeightLowerLimit_;//高さの下限
    float HeightUpperLimit_;//高さの上限


    //----------被弾----------
    float ColliderSize_; //当たり判定(球体)のサイズ
    XMFLOAT3 KnockBack_Direction_;//ノックバックする方向
    XMFLOAT3 KnockBack_Velocity_;//ノックバックする速度
    float KnockBackPower_; //ノックバックする強さ
    float DecelerationRate_;//ノックバック時の1fごとの減速率
    float KnockBackEnd_;//ノックバックを終了する値

    //ノックバックする速度= ノックバックする強さ(定数) * ノックバックする方向

    //----------壁の接触ダメージ----------
    int InvincibilityTime_;//ダメージ後の無敵時間 1fごとに減少
    bool IsInvincibility_;//無敵時間か
    int InvincibilityValue;//無敵時間の値　この値を超えると無敵時間終了

    
    //----------影付け----------
    int hShadow_;//影のモデルハンドル
    float ShadowHeight_;//影をつける高さ
    float ShadowCorrection_;//影の位置の調整値
    Transform ShadowTrans_;//影の描画トランスフォーム
    Ground* pGround_;//地面のインスタンス

public:
    Character(GameObject* parent);
    Character(GameObject* parent, const std::string& name);

    virtual ~Character();

    /// <summary>
    /// CSVから各ステータスを読み込みする
    /// Characterクラス限定
    /// </summary>
    /// <param name="_path">csvファイルのパス</param>
    void SetcsvStatus(std::string _path);

    //----------基本処理----------

    /// <summary>
    /// 初期位置の設定
    /// </summary>
    void SetStartPosition() { this->transform_.position_ = StartPosition_; }

    /// <summary>
    /// 重力処理
    /// </summary>
    void CharacterGravity();

    /// <summary>
    /// 影モデルの初期化
    /// </summary>
    void ShadowInit();

    /// <summary>
    /// 影付け（毎フレーム更新）
    /// </summary>
    void ShadowSet();

    /// <summary>
    /// 影モデル描画
    /// </summary>
    void ShadowDraw();

    /// <summary>
    /// キャラクターの移動処理(回転も行う)
    /// </summary>
    /// <param name="_direction">動かす方向ベクトル</param>
    void CharacterMoveRotate(XMVECTOR _direction, float rotateY);

    /// <summary>
    /// キャラクターの移動処理
    /// </summary>
    /// <param name="_direction">動かす方向ベクトル</param>
    void CharacterMove(XMVECTOR _direction);

    /// <summary>
    /// 移動ベクトルをつくる
    /// </summary>
    void CreateMoveVector();

    /// <summary>
    /// ノックバック移動処理
    /// </summary>
    void KnockBack();

    /// <summary>
    /// ダメージ後の無敵時間の計算
    /// </summary>
    void InvincibilityTimeCalclation();

    /// <summary>
    /// 通常X軸回転
    /// </summary>
    void MoveRotate(){ this->transform_.rotate_.x += MoveRotateX; }

    /// <summary>
    /// 高速X軸回転
    /// </summary>
    void FastRotate(){ this->transform_.rotate_.x += FastRotateX; }

    /// <summary>
    /// 回転を止める
    /// </summary>
    void RotateStop(){ this->transform_.rotate_.x = 0.0f; }

    /// <summary>
    /// 加速度の加算
    /// </summary>
    void Charging();


    /// <summary>
    /// Y軸の回転行列をベクトルに変換
    /// </summary>
    /// <param name="rotY">Y軸に回転したい角度（Degree）</param>
    /// <param name="front">正面ベクトル(ローカル空間)</param>
    /// <returns>変形したベクトル（ワールド空間）</returns>
    XMVECTOR RotateVecFront(float rotY, XMVECTOR front);

    /// <summary>
    /// 
    /// </summary>
    /// <param name="rotY"></param>
    /// <param name="front"></param>
    /// <returns></returns>
    XMVECTOR CalclationForward(float rotY, XMVECTOR front);


    //----------エフェクト処理----------

    /// <summary>
    /// チャージ状態エフェクトをつける
    /// </summary>
    void SetChargingEffect(std::string _path);

    /// <summary>
    /// 突撃エフェクトつける
    /// </summary>
    void SetAttackLocusEffect();

    /// <summary>
    /// 衝撃エフェクトつける
    /// </summary>
    void SetHitEffect();
};

