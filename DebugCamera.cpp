#include "DebugCamera.h"
#include "MyMath.h"
#include "WinApp.h"

void DebugCamera::Initialize(ViewProjection* viewProjection)
{

	this->viewProjection = viewProjection;

	input_ = Input::GetInstance();
}

void DebugCamera::Update()
{
	isAction = false;
	angleX = 0;
	angleY = 0;

	MouseUpdate();

	MouseRot();

	viewProjection->UpdateMatrix();
}

void DebugCamera::MouseRot()
{
	if (input_->MouseInputing(0)) {
		
	}

	if (isAction == true) {
		// 追加回転分の回転行列を生成
		Matrix4 matRotNew = MyMath::Initialize();
		matRotNew *= MyMath::Rotation(Vector3(-angleX, 0, 0), 1);
		matRotNew *= MyMath::Rotation(Vector3(-angleY, 0, 0), 2);
		// 累積の回転行列を合成
		// ※回転行列を累積していくと、誤差でスケーリングがかかる危険がある為
		// クォータニオンを使用する方が望ましい
		CameraRot = matRotNew * CameraRot;

		// 注視点から視点へのベクトルと、上方向ベクトル
		Vector3 vTargetEye = { 0.0f, 0.0f, -CameraDistance };
		Vector3 vUp = { 0.0f, 1.0f, 0.0f };

		// ベクトルを回転
		vTargetEye = MyMath::MatVector(CameraRot, vTargetEye);
		vUp = MyMath::MatVector(CameraRot, vUp);

		// 注視点からずらした位置に視点座標を決定
		const Vector3& target = viewProjection->target;
		viewProjection->eye = Vector3(target.x + vTargetEye.x, target.y + vTargetEye.y, target.z + vTargetEye.z);
		viewProjection->up = Vector3(vUp.x, vUp.y, vUp.z);
	}

}

void DebugCamera::MouseUpdate()
{

	// マウスの入力を取得
	Vector3 mouseMove = input_->GetMouseWheelMove();

	// ホイール入力で距離を変更
	if (mouseMove.z != 0) {
		CameraDistance -= mouseMove.z / 50.0f;
		CameraDistance = max(CameraDistance, 1.0f);
	}

	// マウスの中ボタンが押されていたらカメラを並行移動させる
	if (input_->MouseInputing(2)) {
		float dx = mouseMove.x / 100.0f;
		float dy = mouseMove.y / 100.0f;

		Vector3 move = { -dx, +dy, 0 };
		move = MyMath::MatVector(CameraRot, move);

		MoveVector(move);
	}

}

void DebugCamera::MoveVector(const Vector3& move)
{
	// 視点と注視点座標を移動
	viewProjection->eye.x += move.x;
	viewProjection->eye.y += move.y;
	viewProjection->eye.z += move.z;

	viewProjection->target.x += move.x;
	viewProjection->target.y += move.y;
	viewProjection->target.z += move.z;

}
