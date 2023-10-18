#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "Input.h"
#include <memory>
#include "ViewProjection.h"
#include "NormalGun.h"
#include <Sprite.h>
#include "SplinePosition.h"

/// <summary>
/// プレイヤー
/// </summary>
class Player {

public://基本関数
	Player();
	~Player();

	//初期化
	void Initialize(const Vector3& Pos,const ViewProjection* viewProjection);
	//更新
	void Update();
	//描画
	void Draw();
	//スプライト描画
	void DrawSprite();

	//アタックアップデート
	void AttackUpdate(const Vector3& EnemyPos, bool& LockOn);

private:
	//プレーヤーの移動
	void Move();
	//プレイヤーの回転
	void PlayerRot();
	//プレーヤーの攻撃
	void PlayerAttack(const Vector3& EnemyPos, bool& LockOn);
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

	//当たり判定チェック
	void CheckHitCollision();

	//HP処理
	void HPUpdate();

public://Setter
	//カメラの回転,撃つ場所への距離,プレイヤーとカメラの距離,アルファ値を決めるためのカメラ距離のマックス
	void SetCameraNeedInformation(const Vector2& CameraRot,const Vector3& cameradis,const float& distance,const float& CameraMaxDistance);
	void SetCameraModeNotFree(const bool& mode) { isCameraModeNotFree = mode; }//カメラモード
	void SetFirstMoveSpline(const std::vector<Vector3>& points) { FirstMoveSpline->SetNotSplineVector(points); }//最初のスプライン
	void SetSpline(const std::vector<Vector3>& points) { playerMoveSpline->SetNotSplineVector(points); }//二つ目のスプライン
	void SetFinalSpline(const std::vector<Vector3>& points) { FinalMoveSpline->SetNotSplineVector(points); }//最後のスプライン
public://Getter
	bool GetHitFirstRail()const { return isHitFirstRail; }//最初のスプラインに当たったか
	bool GetHit2ndRail()const { return isHitRail; }//二つ目のスプラインに当たったか
	bool GetHitFinalRail()const { return isHitFinalRail; }//最後のスプラインに当たったか
	bool GetHowReturnSpline(const uint32_t& HowIndex)const { return FirstMoveSpline->GetHowReturnIndex(HowIndex); }//指定した場所に来たか
	bool GetHowReturnSpline2ndRail(const uint32_t & HowIndex)const { return playerMoveSpline->GetHowReturnIndex(HowIndex); }//指定した場所に来たか
	bool GetHowReturnFainalSpline(const uint32_t& HowIndex)const { return FinalMoveSpline->GetHowReturnIndex(HowIndex); }//指定した場所に来たか
	bool GetFinishFirstSpline()const { return FirstMoveSpline->GetFinishSpline(); }//スプラインが終わったか
	Vector3 GetPlayerPos()const { return MyMath::GetWorldTransform(playerWorldTrans.matWorld_); }//ポジションゲット

private://クラス関連
	Input* input_ = nullptr;
	std::unique_ptr<Model> model_;
	WorldTransform playerWorldTrans;
	WorldTransform playerWorldTransHed;
	WorldTransform playerWorldTransForBullet;
	WorldTransform StartingPointOfGrapple;
	WorldTransform DebugWorldTrans;
	const ViewProjection* viewProjection_ = nullptr;
	std::unique_ptr<NormalGun> playerNormalGun;
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
	bool isTakeMissileDamages = false;

	bool isPushW = false;
	bool isPushA = false;
	bool isPushS = false;
	bool isPushD = false;

	uint32_t bulletNumber = 0;
	uint32_t SlidingTime = 0;
	uint32_t SlidingNumber = 0;
	uint32_t PlayerHP = 5000;

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
	Vector3 TargetPosition;
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