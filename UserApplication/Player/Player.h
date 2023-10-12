#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "Input.h"
#include <memory>
#include "ViewProjection.h"
#include "PlayerBullet.h"
#include <Sprite.h>
#include "SplinePosition.h"

/// <summary>
/// プレイヤー
/// </summary>
class Player {

public://基本関数
	Player();
	~Player();

	void Initialize(const Vector3& Pos, ViewProjection* viewProjection);
	void Update();
	void Draw(ViewProjection& viewProjection_);
	void DrawSprite();

	//パーティクルを出す用
	void CopyParticle();

	void AttackUpdate(const Vector3& EnemyPos, bool& LockOn);

private:
	//プレーヤーの移動
	void Move();
	//プレイヤーの回転
	void PlayerRot();
	//プレーヤーの攻撃
	void PlayerAttack(const Vector3& velocity, bool& LockOn);
	//プレーヤーの移動の値更新
	void WorldTransUpdate();
	// プレイヤーの当たり判定
	void CheckPlayerCollider();
	//落下
	void Fall();
	//スプラインアップデート
	void SplineUpdate();
	//レティクルアップデート
	void UpdateReticle();

	//角度を決める奴
	float AngleSelect(float& angle, float& selectAngle);

public://Setter
	void SetCameraModeNotFree(const bool& mode) { isCameraModeNotFree = mode; }
	void SetCameraRot(const Vector2& CameraRot) { cameraRot = CameraRot; }
	void SetEyeToTagetVecDistance(const Vector3& cameradis) { Distance = cameradis; }
	void SetCameraDistance(const float& distance) { PlayerToCameraDistance = distance; }
	void SetCameraMaxDistance(const float& CameraMaxDistance) { this->cameraMaxDistance = CameraMaxDistance; }
	void SetFirstMoveSpline(const std::vector<Vector3>& points) { FirstMoveSpline->SetNotSplineVector(points); }
	void SetSpline(const std::vector<Vector3>& points) { playerMoveSpline->SetNotSplineVector(points); }
	void SetFinalSpline(const std::vector<Vector3>& points) { FinalMoveSpline->SetNotSplineVector(points); }
public://Getter
	bool GetIsPlayerSetUp()const { return isPlayerSetUp; }
	bool GetHitFirstRail()const { return isHitFirstRail; }
	bool GetHit2ndRail()const { return isHitRail; }
	bool GetHitFinalRail()const { return isHitFinalRail; }
	bool GetHowReturnSpline(const uint32_t& HowIndex)const { return FirstMoveSpline->GetHowReturnIndex(HowIndex); }
	bool GetHowReturnSpline2ndRail(const uint32_t & HowIndex)const { return playerMoveSpline->GetHowReturnIndex(HowIndex); }
	bool GetHowReturnFainalSpline(const uint32_t& HowIndex)const { return FinalMoveSpline->GetHowReturnIndex(HowIndex); }
	bool GetFinishFirstSpline()const { return FirstMoveSpline->GetFinishSpline(); }
	Vector3 GetPlayerPos()const { return MyMath::GetWorldTransform(playerWorldTrans.matWorld_); }
	WorldTarnsLook GetPlayerLook()const { return playerWorldTrans.LookVelocity; }

private://クラス関連
	Input* input_ = nullptr;
	std::unique_ptr<Model> model_;
	WorldTransform playerWorldTrans;
	WorldTransform playerWorldTransHed;
	WorldTransform playerWorldTransForBullet;
	WorldTransform StartingPointOfGrapple;
	WorldTransform DebugWorldTrans;
	ViewProjection* viewProjection_ = nullptr;
	std::unique_ptr<PlayerBullet> playerBullet;
	// 照準スプライト
	std::unique_ptr<Sprite> AttackSprite;
	//WASDスプライト
	std::unique_ptr<Sprite> W_FontSprite[2];
	std::unique_ptr<Sprite> A_FontSprite[2];
	std::unique_ptr<Sprite> S_FontSprite[2];
	std::unique_ptr<Sprite> D_FontSprite[2];
	std::unique_ptr<Sprite> AttackFontSprite[2];
	// コライダー
	BaseCollider* PlayerCollider = nullptr;
	//スプライン
	std::unique_ptr<SplinePosition> FirstMoveSpline;//最初のカメラスプライン
	std::unique_ptr<SplinePosition> playerMoveSpline;//途中のスプライン
	std::unique_ptr<SplinePosition> FinalMoveSpline;//最後のカメラスプライン

private://イーナムクラス
	enum class AttackPhase {
		AttackCombo1,//
		AttackCombo2,//
		AttackCombo3,//
		AttackCombo4,//
		AttackUlt,//
		Nothing,//何もしない
	};
	//見つけてないときの動きフェーズ
	AttackPhase AttackPhase_ = AttackPhase::Nothing;

private://別クラスから値をもらう
	Vector2 cameraRot;

private://プレイヤークラス変数
	bool isPlayerSetUp = false;
	bool onGround = false;
	bool isHitRail = false;
	bool isHitFirstRail = false;
	bool isHitFinalRail = false;
	bool isCameraModeNotFree = false;
	bool isAttack = false;
	bool Stop = false;
	bool isPressing = false;
	bool isGrapple = false;
	bool firstPush = false;
	bool isSliding = false;

	bool isPushW = false;
	bool isPushA = false;
	bool isPushS = false;
	bool isPushD = false;

	uint32_t bulletNumber = 0;
	uint32_t SlidingTime = 0;
	uint32_t SlidingNumber = 0;

	float Radius = 1.0f;
	float playerSpeed = 0.9f;
	float diagonalPlayerSpeed = 0.7f;
	float PlayerToCameraDistance = 0.0f;
	float PlayerToCameraTargetVecDistance = 0.0f;
	float cameraMaxDistance = 0.0f;
	float alpha = 0.0f;
	float PlayerToAimSaiteVecDistance = 0.0f;
	float GrappleSpeed = 0.0f;
	float SlidingSpeed = 0.0f;
	float DownSlidingTimes = 0.0f;

	Vector3 playerMoveMent;//移動量
	Vector3 Distance;
	Vector3 DistanceNolm;
	Vector3 ReticlePos;
	Vector3 ShootVec;
	Vector3 PlayerToAimSaiteVec;
	Vector3 StartingPoint;
	//振り子角度
	Vector3 angle;
	Vector3 pendulumLengthVec;
	Vector3 SlidingVelocity;
	Vector3 DownSlidingVelocity;
	//Vector3 angularVelocity;

	Vector4 fallVec;


	// 振り子のパラメータ
	const float g = 9.81f;     // 重力加速度 (m/s^2)
	const float pendulumLength = 2.0f; // 振り子の長さ (メートル)
	float theta = MyMath::GetAngle(45.0f);   // 初期角度 (ラジアン)
	float angularVelocity = 0.0f; // 初期角速度 (ラジアン/秒)
	const float timeStep = 0.01f; // 時間ステップ (秒)


	float PendulumLength = 0.0f;

};