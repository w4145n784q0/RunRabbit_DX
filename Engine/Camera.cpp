#include "camera.h"
#include "Direct3D.h"

XMFLOAT3 _position;
XMFLOAT3 _target;
XMMATRIX _view;
XMMATRIX _proj;
XMMATRIX _billBoard;

bool IsCameraShake;//カメラが振動してるか
float ShakeTimer;//カメラの振動カウント
const float ShakeSpeed = 300.0f;//振動スピード
const float ShakeWidth = 0.5f;//振動幅
const float frame = 0.016f;//1フレーム

//初期化（プロジェクション行列作成）
void Camera::Initialize()
{
	_position = XMFLOAT3(0, 3, -10);	//カメラの位置
	_target = XMFLOAT3( 0, 0, 0);	//カメラの焦点

	//プロジェクション行列
	_proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)Direct3D::screenWidth_ / (FLOAT)Direct3D::screenHeight_, 0.1f, 1000.0f);
}

//更新（ビュー行列作成）
void Camera::Update()
{
	//ビュー行列
	_view = XMMatrixLookAtLH(XMVectorSet(_position.x, _position.y, _position.z, 0),
		XMVectorSet(_target.x, _target.y, _target.z, 0), XMVectorSet(0, 1, 0, 0));


	//ビルボード行列
	//（常にカメラの方を向くように回転させる行列。パーティクルでしか使わない）
	//http://marupeke296.com/DXG_No11_ComeOnBillboard.html
	_billBoard = XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), XMLoadFloat3(&_target)- XMLoadFloat3(&_position), XMVectorSet(0, 1, 0, 0));
	_billBoard = XMMatrixInverse(nullptr, _billBoard);
}

//焦点を設定
void Camera::SetTarget(XMFLOAT3 target) { _target = target; }

//位置を設定
void Camera::SetPosition(XMFLOAT3 position) { _position = position; }

//焦点を取得
XMFLOAT3 Camera::GetTarget() { return _target; }

//位置を取得
XMFLOAT3 Camera::GetPosition() { return _position; }

//ビュー行列を取得
XMMATRIX Camera::GetViewMatrix() { return _view; }

//プロジェクション行列を取得
XMMATRIX Camera::GetProjectionMatrix() { return _proj; }

//ビルボード用回転行列を取得
XMMATRIX Camera::GetBillboardMatrix(){	return _billBoard; }

//カメラ振動(Y座標のみ)
float Camera::CameraShake()
{
	float cameraY = 0.0f;
	if (IsCameraShake)
	{
		ShakeTimer -= frame;
		if (ShakeTimer <= 0.0f)
		{
			IsCameraShake = false;
			ShakeTimer = 0.0f;
		}
		else
		{
			cameraY = sinf(ShakeTimer * ShakeSpeed)* ShakeTimer * ShakeWidth;
		}
	}
	return cameraY;
}

//カメラ振動(XY座標)
XMFLOAT3 Camera::CameraShakeFloat3()
{
	XMFLOAT3 camera = {0,0,0};
	if (IsCameraShake)
	{
		ShakeTimer -= frame;
		if (ShakeTimer <= 0.0f)
		{
			IsCameraShake = false;
			ShakeTimer = 0.0f;
		}
		else
		{
			camera.x = sinf(ShakeTimer * ShakeSpeed) * ShakeTimer * ShakeWidth;
			camera.y = sinf(ShakeTimer * ShakeSpeed) * ShakeTimer * ShakeWidth;
		}
	}
	return camera;
}

//振動開始
//_shaketime 振動させる時間
void Camera::CameraShakeStart(float _shaketime)
{
	IsCameraShake = true;
	ShakeTimer = _shaketime;

}