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
#include "PlayerUI.h"
#include <FbxModel.h>
#include <FBXObject3d.h>


/// <summary>
/// プレイヤー
/// </summary>
class Player
{

public://基本関数
	Player();
	~Player();

	//初期化
	void Initialize(const Vector3& Pos,const ViewProjection* viewProjection);
	//更新
	void Update();
	//描画
	void Draw();
	//描画
	void FbxDraw();
	//スプライト描画
	void DrawSprite();

	//アタックアップデート
	void AttackUpdate(const Vector3& EnemyPos,bool& LockOn);

private:
	//プレーヤーの移動
	void Move();
	//プレイヤーの回転
	void PlayerRot();
	//プレーヤーの攻撃
	void PlayerAttack(const Vector3& EnemyPos,bool& LockOn);
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
	//スライドブースト方向決め
	void DeterminationDirection();
	//スライドブーストアップデート
	void SlideBoostUpdate();
	//角度を決める奴
	float AngleSelect(float& angle,float& selectAngle);

	//当たり判定チェック
	void CheckHitCollision();

	//HP処理
	void HPUpdate();

public://Setter
	//カメラの回転,撃つ場所への距離,プレイヤーとカメラの距離,アルファ値を決めるためのカメラ距離のマックス
	void SetCameraNeedInformation(const Vector2& CameraRot,const Vector3& cameradis,const float& distance,const float& CameraMaxDistance);
	//カメラモード
	void SetCameraModeNotFree(const bool& mode) {
		isCameraModeNotFree = mode;
	}
	//最初のスプライン
	void SetFirstMoveSpline(const std::vector<Vector3>& points) {
		FirstMoveSpline->SetNotSplineVector(points);
	}
	//二つ目のスプライン
	void SetSpline(const std::vector<Vector3>& points) {
		playerMoveSpline->SetNotSplineVector(points);
	}
	//最後のスプライン
	void SetFinalSpline(const std::vector<Vector3>& points) {
		FinalMoveSpline->SetNotSplineVector(points);
	}
	//レティクルポジションセット
	void SetReticlePosition(const Vector2& position) {
		playerUI->SetReticlePosition(position);
	}
public://Getter
	bool GetHitFirstRail()const {//最初のスプラインに当たったか
		return isHitFirstRail;
	}
	bool GetHit2ndRail()const {//二つ目のスプラインに当たったか
		return isHitRail;
	}
	bool GetHitFinalRail()const {//最後のスプラインに当たったか
		return isHitFinalRail;
	}
	bool GetHowReturnSpline(const uint32_t& HowIndex)const {//指定した場所に来たか
		return FirstMoveSpline->GetHowReturnIndex(HowIndex);
	}
	bool GetHowReturnSpline2ndRail(const uint32_t& HowIndex)const {//指定した場所に来たか
		return playerMoveSpline->GetHowReturnIndex(HowIndex);
	}
	bool GetHowReturnFainalSpline(const uint32_t& HowIndex)const {//指定した場所に来たか
		return FinalMoveSpline->GetHowReturnIndex(HowIndex);
	}
	bool GetFinishFirstSpline()const {//スプラインが終わったか
		return FirstMoveSpline->GetFinishSpline();
	}
	bool GetFinishMove()const {//スプラインが終わったか
		return isDieMoveFinish;
	}
	Vector3 GetPlayerPos()const {//ポジションゲット
		return MyMath::GetWorldTransform(playerWorldTrans.matWorld_);
	}

private://クラス関連
	Input* input_ = nullptr;
	std::unique_ptr<Model> model_;
	std::unique_ptr<FBXModel> fbxModel_;
	std::unique_ptr<FBXObject3d> fbxObj3d_;
	WorldTransform playerWorldTrans;
	WorldTransform playerWorldTransHed;
	WorldTransform playerWorldTransForBullet;
	WorldTransform StartingPointOfGrapple;
	WorldTransform DebugWorldTrans;
	const ViewProjection* viewProjection_ = nullptr;
	std::unique_ptr<NormalGun> playerNormalGun;
	// 照準スプライト
	std::unique_ptr<Sprite> AttackSprite;

	// コライダー
	BaseCollider* PlayerCollider = nullptr;
	//スプライン
	std::unique_ptr<SplinePosition> FirstMoveSpline;//最初のカメラスプライン
	std::unique_ptr<SplinePosition> playerMoveSpline;//途中のスプライン
	std::unique_ptr<SplinePosition> FinalMoveSpline;//最後のカメラスプライン

	//プレイヤーUIクラス
	std::unique_ptr<PlayerUI> playerUI;

private://イーナムクラス
	enum class AttackPhase
	{
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
	bool isStop = false;
	bool isPressing = false;
	bool isGrapple = false;
	bool firstPush = false;
	bool isSliding = false;
	bool isTakeMissileDamages = false;

	bool isPushW = false;
	bool isPushA = false;
	bool isPushS = false;
	bool isPushD = false;

	bool isAlive = true;
	bool isDieMoveFinish = false;

	uint32_t BulletNumber = 0;
	uint32_t SlidingNumber = 0;
	uint32_t nowAnmFCount_ = 0;
	uint32_t nowAnmNum_ = 7;
	uint32_t maxFcount = 25;

	float Radius = 1.0f;
	float playerSpeed = 0.9f;
	float PlayerToCameraDistance = 0.0f;
	float PlayerToCameraTargetVecDistance = 0.0f;
	float CameraMaxDistance = 0.0f;
	float Alpha = 0.0f;
	float PlayerToAimSaiteVecDistance = 0.0f;
	float GrappleSpeed = 0.0f;
	float SlidingSpeed = 0.0f;
	float MaxSlidingSpeed = 2.7f;
	float PlayerHP = 800;
	float PlayerMaxHP = 5000;

	Vector3 playerMoveMent;//移動量
	Vector3 TargetPosition;
	Vector3 DistanceNolm;
	Vector3 ReticlePos;
	Vector3 ShootVec;
	Vector3 PlayerToAimSaiteVec;
	Vector3 StartingPoint;
	Vector3 DownSlidingVelocity;
	//Vector3 angularVelocity;

	Vector4 fallVec;

};