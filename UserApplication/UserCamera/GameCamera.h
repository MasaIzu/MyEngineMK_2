#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "Uint32Vector2.h"
#include <Input.h>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include <BaseCollider.h>

/// <summary>
/// ゲームプレイカメラ
/// </summary>
class GameCamera {

public://基本関数

	GameCamera(uint32_t window_width, uint32_t window_height);
	~GameCamera();

	//初期化
	void Initialize(ViewProjection* viewProjection_, const float& cameraAngle, const Vector3& pos);
	//カメラポジション初期化
	void InitializeCameraPosition(const float& cameraAngle);
	//更新
	void Update();

public:
	//リセット
	void MousePositionReset();
	void MousePositionReset(Vector2& vector2);//少し細工する

private:
	void HowMuchMoved();//どのくらい動いたか
	void CheckCameraWhichWayMove();//どっちに動いたか
	void PlaySceneCamera();//プレイヤーがシーンのカメラ
	void SceneCamera();//シーン状態のカメラ
	void Collision();//当たり判定
	void CameraAngle(const float& x, const float& z);//カメラの角度

	//カメラの視線切れているか
	bool CheckBetweenToCameraCollider();

public://getter
	float GetFovAngle();//アングル
	float GetCameraDistanse();//離れた距離
	float GetMaxDistance();//マックスの離れた距離
	Vector2 GetCameraAngle() const;//カメラのアングルゲット
	Vector3 GetCameraRotVec3() { return this->rot; }//ロットゲット
	Vector3 GetEyeToTagetVecDistance(const float& distance) const;//アイとターゲットの距離
	Vector3 GetPlayerDistanceEyePos(const Vector3& playerPos_);//プレイヤーとの距離
	Matrix4 GetCameraRot() { return this->CameraRot; }//カメラの回転Matrix4

public://setter
	void SetCameraMode(const bool& mode);//カメラのモード
	void SetPlayerPosition(const Vector3& pos);//プレイヤーのポジション
	void SetFreeCamera(const bool& mode);//フリーカメラ状態か
	void SetCameraTargetAndPos(const Vector3& target, const Vector3& eye);//ターゲットとアイの位置をセット

private://クラス関連
	
	Input* input_ = nullptr;
	ViewProjection* viewProjection = nullptr;

	// コライダー
	BaseCollider* CameraCollider = nullptr;

private://プレイヤークラス変数
	bool cameraMode = false;
	bool isShake = false;
	bool cameraDown = false;
	bool cameraUp = false;
	bool FreeCamera = false;

	uint32_t cameraType = 0;
	uint32_t winWidth = 0;
	uint32_t winHeight = 0;
	uint32_t cameraTime = 0;
	uint32_t MaxCameraTime = 0;
	uint32_t cameraMode_ = 0;//カメラモード
	uint32_t HowMachMovePointer = 0;
	uint32_t shakeTime = 0;
	uint32_t cameraModeChangeCountTimer = 30;

	float scaleX_ = 1.0f;// スケーリング
	float scaleY_ = 1.0f;
	float angleAroundPlayer = 0.0f; // プレイヤーの周りを回転する角度
	float cameraDistance_ = 35.0f;
	float angle = 0.0f;
	float cameraSpeed_ = 3;// カメラの速度
	float cameraDelay = 0.1f;// カメラが追跡する際の遅延量
	float playerCameraDelay = 0.1f;
	float playerCameraSpeed_ = 3;
	float kand = 400.0f;
	float Fov = 45.0f;
	float TargetCameraDelay = 0.05f;
	float TargetCameraSpeed_ = 1.0;
	float CameraDistanceMinus = 0.0f;
	float CameraMouseMoved = 0.0f;
	float OldMouseMoved = 0.0f;
	float CameraCollisionRadius = 0.1f;
	float CameraRayCollisionRadius = 0.2f;
	float PlayerToCameraVecDistance = 0.0f;
	float LerpStrength = 0.085f;

	Uint32Vector2 windowWH;

	Vector2 mousePos;
	Vector2 oldMousePos;
	Vector2 Mous_UP_DOWN;
	Vector2 MouseMove;
	Vector2 mouseMoved{ 0, MyMath::PI };

	Vector3 rot;
	Vector3 eye;
	Vector3 vUp;
	Vector3 playerPos_;
	Vector3 target;
	Vector3 cameraHigh = { 0,10,0 };
	Vector3 PlayerToCameraVec;
	//セットされたVec
	Vector3 SetTargetVec;
	Vector3 SetEyeVec;
	//遅延カメラ
	Vector3 LerpPlayerPosition;

	Matrix4 CameraRot;
	Matrix4 matRot;// 回転行列


};