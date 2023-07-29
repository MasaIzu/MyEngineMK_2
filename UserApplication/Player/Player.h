#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "Input.h"
#include "affin.h"
#include <memory>
#include "ViewProjection.h"
#include "PlayerBullet.h"
#include <Sprite.h>
#include "SplinePosition.h"

class Player {

public://基本関数
	Player();
	~Player();

	void Initialize(const Vector3& Pos);
	void Update();
	void Draw(ViewProjection& viewProjection_);
	void DrawSprite(ViewProjection& viewProjection_);

	//パーティクルを出す用
	void CSUpdate(ID3D12GraphicsCommandList* cmdList);
	void ParticleDraw(ViewProjection& viewProjection_);
	void CopyParticle();

private:
	//プレーヤーの移動
	void Move();
	//プレイヤーの回転
	void PlayerRot();
	//プレーヤーの攻撃
	void PlayerAttack();
	//プレーヤーの移動の値更新
	void WorldTransUpdate();
	// プレイヤーの当たり判定
	void CheckPlayerCollider();
	//落下
	void Fall();
	//スプラインアップデート
	void SplineUpdate();

public://Setter
	void SetCameraModeNotFree(const bool& mode) { isCameraModeNotFree = mode; }
	void SetCameraRot(const Vector2& CameraRot) { cameraRot = CameraRot; }
	void SetEyeToTagetVecDistance(const Vector3& cameradis) { Distance = cameradis; }
	void SetCameraDistance(const float& Distance) { PlayerToCameraDistance = Distance; }
	void SetCameraMaxDistance(const float& cameraMaxDistance) { this->cameraMaxDistance = cameraMaxDistance; }
	void SetFirstMoveSpline(const std::vector<Vector3>& points) { FirstMoveSpline->SetNotSplineVector(points); }
	void SetSpline(const std::vector<Vector3>& points) { playerMoveSpline->SetNotSplineVector(points); }
	void SetFinalSpline(const std::vector<Vector3>& points) { FinalMoveSpline->SetNotSplineVector(points); }
public://Getter
	bool GetIsPlayerSetUp()const { return isPlayerSetUp; }
	bool GetHitFirstRail()const { return isHitFirstRail; }
	bool GetHitFinalRail()const { return isHitFinalRail; }
	bool GetHowReturnSpline(const uint32_t& HowIndex)const { return FirstMoveSpline->GetHowReturnIndex(HowIndex); }
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
	WorldTransform DebugWorldTrans;
	std::unique_ptr<PlayerBullet> playerBullet;
	// 照準スプライト
	std::unique_ptr<Sprite> AttackSprite;
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
	bool Stop = false;

	uint32_t bulletNumber = 0;

	float Radius = 1.0f;
	float playerSpeed = 0.6f;
	float diagonalPlayerSpeed = 0.7f;
	float PlayerToCameraDistance = 0.0f;
	float PlayerToCameraTargetVecDistance = 0.0f;
	float cameraMaxDistance = 0.0f;
	float alpha = 0.0f;

	Vector3 playerMoveMent;//移動量
	Vector3 ReticlePos;
	Vector3 Distance;
	Vector3 DistanceNolm;

	Vector4 fallVec;
};