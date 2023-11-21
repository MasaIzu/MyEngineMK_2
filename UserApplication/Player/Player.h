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
#include "PlayerEnum.h"
#include "Animation.h"
#include "PlayerStruct.h"
#include "PlayerMovement.h"
#include <ParticleHandHanabi.h>
#include "PlayerDamageHitUI.h"
#include "Damage.h"

struct PlayerAnimTime
{
	const uint32_t Step = 30;
	const uint32_t DieMotion = 120;
	const uint32_t BladeAttack = 60;
};

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

	//パーティクルを出す用
	void CSUpdate(ID3D12GraphicsCommandList* cmdList);
	void ParticleDraw();

private:
	
	//プレイヤーの回転
	void PlayerRot(const bool& Attack,const bool& BladeAttack);
	//プレーヤーの攻撃
	void PlayerAttack(const Vector3& EnemyPos,bool& LockOn);
	//プレーヤーの攻撃
	void PlayerBladeAttack(const Vector3& EnemyPos,bool& LockOn);
	//プレーヤーの移動の値更新
	void WorldTransUpdate();
	// プレイヤーの当たり判定
	void CheckPlayerCollider();
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
	//レティクルポジションセット
	void SetReticlePosition(const Vector2& position) {
		playerUI->SetReticlePosition(position);
	}
public://Getter

	bool GetFinishMove()const {//スプラインが終わったか
		return isDieMoveFinish;
	}
	Vector3 GetPlayerPos()const; //ポジションゲット


private://クラス関連
	Input* input_ = nullptr;
	std::unique_ptr<Model> model_;
	WorldTransform playerWorldTrans;
	WorldTransform playerRotWorldTrans;
	WorldTransform StartingPointOfGrapple;
	WorldTransform DebugWorldTrans;
	const ViewProjection* viewProjection_ = nullptr;
	std::unique_ptr<NormalGun> playerNormalGun;

	// コライダー
	BaseCollider* PlayerCollider = nullptr;
	BaseCollider* PlayerBladeAttackCollider = nullptr;

	//プレイヤーUIクラス
	std::unique_ptr<PlayerUI> playerUI;

	//アニメーションクラス
	std::unique_ptr<Animation> animation;
	//移動クラス
	std::unique_ptr<PlayerMovement> playerMovement;
	std::unique_ptr<ParticleHandHanabi> Particle;

	//敵のダメージ
	EnemyAttackDamage Damege;

	//ダメージUI
	std::unique_ptr<PlayerDamageHitUI> DamageUI;

private://ストラクトやイーナムクラス
	//アニメーションタイム
	PlayerAnimTime playerAnimTime;
	//ローテーション
	PlayerMoveRot playerMoveRot;
	//アタックフェーズ
	AttackPhase AttackPhase_ = AttackPhase::Nothing;
	//アニメーション
	PlayerAnimation PlayerAnimation_ = PlayerAnimation::Not1;

private://別クラスから値をもらう
	Vector2 cameraRot;

private://コンスト

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
	bool isBladeAttack = false;
	bool isAlive = true;
	bool isDieMotion = false;
	bool isDieMoveFinish = false;
	bool isBladeAttacking = false;
	bool isPreparation = false;

	uint32_t BulletNumber = 0;
	uint32_t SlidingNumber = 0;
	uint32_t LeftBoneNum = 41;
	uint32_t RightBoneNum = 28;
	uint32_t BladeAttackTime = 30;
	uint32_t BladeAttackEndPos = 31;
	uint32_t BladeMaxAttackTime = 40;

	float PlayerRadius = 3.5f;
	float PlayerBladeRadius = 2.0f;
	float FlontRadius = 1.0f;
	float playerSpeed = 0.9f;
	float PlayerToCameraDistance = 0.0f;
	float PlayerToCameraTargetVecDistance = 0.0f;
	float CameraMaxDistance = 0.0f;
	float Alpha = 0.0f;
	float PlayerToAimSaiteVecDistance = 0.0f;
	float GrappleSpeed = 0.0f;
	float SlidingSpeed = 0.0f;
	float MaxSlidingSpeed = 2.0f;
	float PlayerHP = 5000;
	float PlayerMaxHP = 5000;
	float RotLimit = 0.1f;
	float FixedAngle = 0.0f;
	float BladeAttackBoostSpeed = 1.2f;
	float BladeAttackSpeed = 0.7f;

	Vector3 TargetPosition;
	Vector3 DistanceNolm;
	Vector3 PlayerMoveMent;
	Vector3 ReticlePos;
	Vector3 ShootVec;
	Vector3 PlayerToAimSaiteVec;
	Vector3 StartingPoint;
	Vector3 DownSlidingVelocity;
	Vector3 RotKeep;
	Vector3 BladeAttackVelocity;

	Vector4 fallVec;

};