#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "Uint32Vec2.h"
#include <Input.h>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Easing.h"

class GameCamera {

public:

	GameCamera(uint32_t window_width, uint32_t window_height);

	~GameCamera();

	void Initialize(ViewProjection* viewProjection_,const float& cameraAngle);
	void InitializeCameraPosition(const float& cameraAngle);

	void Update();

	void PlaySceneCamera();

	void MultiplyMatrix(Matrix4& matrix);

	void Collision();

	void Reset();

	void MousePositionReset();

private:

	void CameraAngle(const float& x, const float& z);

public://ゲッターセッター

	Matrix4 GetCameraRot() { return this->CameraRot; }
	Vector3 GetCameraRotVec3() { return this->rot; }
	float GetFovAngle() { return MyMath::GetAngle(Fov); }

	float GetCameraAngle()const { return mouseMoved.y - MyMath::PI; }

	void SetCameraPosition(const Vector3& pos) { playerPos_ = pos; }
	void SetPlayerMoveMent(Vector3& playerMoveMent) { PlayerMoveMent = playerMoveMent; }
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

	// スケーリング
	float scaleX_ = 1.0f;
	float scaleY_ = 1.0f;

	Vector3 vTargetEye;
	Vector3 vUp;
	Vector3 playerPos_;
	Vector3 target;

	bool cameraMode = false;

	uint32_t cameraType = 0;

	uint32_t winWidth = 0;
	uint32_t winHeight = 0;
	Vector2 MouseMove;
	Vector2 mouseMoved{ 0,MyMath::PI };

	float angleAroundPlayer; // プレイヤーの周りを回転する角度
	Matrix4 CameraRot;


	float playerCameraDistance = 5.5f;

	uint32_t cameraTime = 0;
	uint32_t MaxCameraTime = 0;


	//カメラモード(tekito-)
	uint32_t cameraMode_ = 0;
	//カメラ距離関係
	const float  MAX_CAMERA_DISTANCE = 20.0f;
	const float  MIN_CAMERA_DISTANCE = 7.0f;
	float cameraDistance_ = 20.0f;
	const float MAX_CHANGE_TIMER = 30;
	uint32_t cameraModeChangeCountTimer = 30;
	float cameraHeight_ = 6;

	bool isShake = false;
	uint32_t shakeTime = 0;

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

	float CameraDistanceMinus = 0.0f;

	float CameraMouseMoved = 0.0f;

	float OldMouseMoved = 0.0f;

	Vector2 Mous_UP_DOWN;

	bool cameraDown = false;
	bool cameraUp = false;

	Uint32Vector2 windowWH;


	ViewProjection* viewProjection = nullptr;
	uint32_t HowMachMovePointer = 0;
};