#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4.h"
#include <Input.h>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Easing.h"

class GameCamera {

public:

	GameCamera(int window_width, int window_height);

	~GameCamera();

	void Initialize();

	void Update(ViewProjection* viewProjection_);

	void PlaySceneCamera(ViewProjection* viewProjection_);
	void PlayerLockOnCamera(ViewProjection* viewProjection_);

	void MultiplyMatrix(Matrix4& matrix);

	void Collision();

private:
	// カメラの位置を計算する関数
	Vector3 calculateCameraPosition(ViewProjection* viewProjection_,float distance, float angle);
	// カメラが向く位置を計算する関数
	Vector3 calculateLookAtPosition(Vector3 target, Vector3 camera);

	void CameraAngle(float x, float z);

public://ゲッターセッター

	Vector3 GetEye();
	Vector3 GetUp() { return vUp; }
	Vector3 GetTarget() { return target; }
	Matrix4 GetCameraRot() { return this->CameraRot; }
	Vector3 GetCameraRotVec3() { return this->rot; }
	float GetFovAngle() { return MyMath::GetAngle(Fov); }
	// カメラの垂直方向の角度を計算する関数
	float getPitch(ViewProjection* viewProjection_) {
		return -atan2(playerPos_.y - viewProjection_->eye.y, playerCameraDistance);
	}
	float GetCameraAngle() { return angle; }
	// カメラの水平方向の角度を計算する関数
	float getYaw(Vector3 position, Vector3 lookAt) {
		Vector3 direction = lookAt - position;
		direction.norm();
		float playerYaw = atan2(direction.x, direction.z);
		return playerYaw;
	}

	void GetEnemyPos(Vector3 EnemyPos) { EnemyPos_ = EnemyPos; }
	void SetCameraPosition(Vector3 pos) { playerPos_ = pos; }
	void SetSpaceInput(bool isSpaceInput) { spaceInput = isSpaceInput; }
	void SetPlayerMoveMent(Vector3 playerMoveMent) { PlayerMoveMent = playerMoveMent; }
private:

	Easing* easing_;

	// カメラ注視点までの距離
	float distance_ = 10;
	Vector2 mousePos = { 0.0f,0.0f };
	Vector2 oldMousePos = { 0.0f,0.0f };
	Input* input_ = nullptr;

	// 回転行列
	Matrix4 matRot;
	Vector3 rot;
	float fTheta = 4.57f;//カメラ横方向角度
	float fDelta = 0.43f;//カメラ縦方向角度

	float mousepoint_a;//マウス位置
	float mousepoint_b;//マウス位置
	bool dirty = false;

	bool spaceInput = false;

	// スケーリング
	float scaleX_ = 1.0f;
	float scaleY_ = 1.0f;

	Vector3 vTargetEye;
	Vector3 vUp;
	Vector3 playerPos_;
	Vector3 target;

	bool cameraMode = false;

	int cameraType = 0;

	int winWidth = 0;
	int winHeight = 0;
	Vector2 MouseMove;
	Vector2 mouseMoved;
	WorldTransform worldTransform_;
	WorldTransform EnemyWorld_;
	Vector3 EnemyPos_;

	float angleAroundPlayer; // プレイヤーの周りを回転する角度
	Matrix4 CameraRot;


	float playerCameraDistance = 5.5f;

	int cameraTime = 0;
	int MaxCameraTime = 0;


	//カメラモード(tekito-)
	int cameraMode_ = 0;
	//カメラ距離関係
	const float  MAX_CAMERA_DISTANCE = 20.0f;
	const float  MIN_CAMERA_DISTANCE = 7.0f;
	float cameraDistance_ = 20.0f;
	const float MAX_CHANGE_TIMER = 30;
	int cameraModeChangeCountTimer = 30;
	float cameraHeight_ = 6;

	bool isShake = false;
	int shakeTime = 0;

	float angle = 0.0f;

	//遅延カメラ
	// カメラの現在の位置
	Vector3 cameraPos;

	// カメラの速度
	float cameraSpeed_ = 3;

	// カメラが追跡する際の遅延量
	float cameraDelay = 0.1f;

	float cameraDis = 45.0f;

	Vector3 oldCameraPos;

	Vector3 LatePlayerPos = { 0,0,0 };
	float playerCameraDelay = 0.1f;
	float playerCameraSpeed_ = 3;

	Vector3 PlayerMoveMent;

	Vector3 CameraTarget;
	float TargetCameraDelay = 0.05f;
	float TargetCameraSpeed_ = 1.0;

	Vector3 higth = { 0,10,0 };

	float Fov = 45.0f;

};