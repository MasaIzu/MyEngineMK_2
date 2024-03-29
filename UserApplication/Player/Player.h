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
#include <ParticleBoost.h>
#include "MyStruct.h"
#include "ExplosionParticleSmokeManager.h"
#include "ExplosionGun.h"
#include "Explosion.h"
#include "ParticleEditor.h"
#include <PlayerState.h>

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
	void Initialize(const Vector3& Pos,const ViewProjection* viewProjection,AudioManager* audioManager_);
	//更新
	void Update();
	//描画
	void Draw(const ViewProjection& LightViewProjection_);
	//描画
	void FbxDraw(const ViewProjection& lightViewProjection_);
	//描画
	void FbxShadowDraw(const ViewProjection& lightViewProjection_);
	//スプライト描画
	void DrawSprite();

	//アタックアップデート
	void AttackUpdate(const Vector3& EnemyPos,bool& LockOn);

	//パーティクルを出す用
	void CSUpdate(ID3D12GraphicsCommandList* cmdList);
	void ParticleDraw();

	//状態移行
	void TransitionTo(PlayerState* state);

	//移動関数
	void PlayerMove(const Vector3& Velocity);

private:
	//プレイヤーの回転
	void PlayerRot(const bool& Attack,const bool& BladeAttack,const bool& isMissileAttack);
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
	//剣の当たり判定属性更新
	void BladeAttributeSet(const unsigned short Attribute_);

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

	bool GetFinishDieDirection()const {
		return isDieDirectionFinish;
	}
	Vector3 GetPlayerPos()const; //ポジションゲット

	PlayerStateNeedMaterial GetStateMaterial() {
		return playerStateNeedMaterial;
	}

private://コンスト
	static const uint32_t AttackColSphereCount = 4;

	const float MaxBladeColDetection = 100.0f;

private://クラス関連
	Input* input_ = nullptr;
	std::unique_ptr<Model> model_;
	WorldTransform playerWorldTrans;
	WorldTransform playerRotWorldTrans;
	std::array<WorldTransform,AttackColSphereCount> BladeColWorldTrans;
	WorldTransform DebugWorldTrans;
	const ViewProjection* viewProjection_ = nullptr;
	std::unique_ptr<NormalGun> playerNormalGun;
	std::unique_ptr<ExplosionGun> playerExplosionGun;
	PlayerState* state_;
	// コライダー
	BaseCollider* PlayerCollider = nullptr;
	std::array<BaseCollider*,AttackColSphereCount> PlayerBladeAttackCollider;

	//プレイヤーUIクラス
	std::unique_ptr<PlayerUI> playerUI;

	//アニメーションクラス
	std::unique_ptr<Animation> animation;
	//移動クラス
	std::unique_ptr<PlayerMovement> playerMovement;

	//敵のダメージ
	EnemyAttackDamage Damege;

	//ダメージUI
	std::unique_ptr<PlayerDamageHitUI> DamageUI;

	//パーティクル
	std::unique_ptr<ParticleHandHanabi> ParticleHanabi;
	std::unique_ptr<ParticleBoost> ParticleBooster;
	std::unique_ptr<ExplosionParticleSmokeManager> ParticleExplosion;
	std::unique_ptr<ParticleEditor> particleEditor;
	std::unique_ptr<ParticleEditor> particleLeftLegHibanaParticle;
	std::unique_ptr<ParticleEditor> particleRightLegHibanaParticle;
	std::unique_ptr<ParticleEditor> HitEffectParticle;

	PlayerStateNeedMaterial playerStateNeedMaterial;
	AudioManager* audioManager = nullptr;

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
	bool isBladeAttacking = false;
	bool isPreparation = false;
	bool isShotRightShoulder = false;
	bool isMissileAttack = false;
	bool isPlayerExplosion = false;
	bool isDieDirectionFinish = false;
	bool isG = false;
	bool isLightActive = true;
	bool isLegParticle = true;
	bool isTakeDamage = false;

	uint32_t BulletNumber = 0;
	uint32_t SlidingNumber = 0;
	uint32_t LeftBoneNum = 52;
	uint32_t RightBoneNum = 34;
	uint32_t kataLeftBoneNumber = 50;
	uint32_t BladeAttackTime = 30;
	uint32_t BladeAttackEndPos = 39;
	uint32_t BladeMaxAttackTime = 40;
	uint32_t RightStart = 65;//ブースト
	uint32_t RightEnd = 66;
	uint32_t CenterRightStart = 75;
	uint32_t CenterRightEnd = 76;
	uint32_t CenterLeftStart = 72;
	uint32_t CenterLeftEnd = 73;
	uint32_t LeftStart = 69;
	uint32_t LeftEnd = 70;
	uint32_t DieDirectionTimer = 0;
	uint32_t DieDirectionMaxTimer = 80;
	uint32_t CircleShadowCount = 0;
	uint32_t LegParticleCount = 1024;
	uint32_t HitEffectParticleCount = 20000;

	//足から出てくるパーティクルのボーンポジション
	uint32_t LeftLegBoneNum = 8;
	uint32_t RightLegBoneNum = 14;
	uint32_t SenterBoneNum = 2;
	uint32_t AttackNormalGun = 0;

	//オーディオ
	uint32_t BoostSound = 0;
	float soundVol = 0.2f;
	float GunSound = 0.1f;

	float PlayerRadius = 3.5f;
	float PlayerBladeRadius = 1.0f;
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
	float PlayerHP = 900000;
	float PlayerMaxHP = PlayerHP;
	float RotLimit = 0.1f;
	float FixedAngle = 0.0f;
	float BladeAttackBoostSpeed = 1.2f;
	float BladeAttackSpeed = 0.7f;
	float BladeColEndHasten = 15.0f;
	float LightDistance = 100.0f;

	Vector2 LightAngle = { 0.0f,2.0f };

	Vector3 TargetPosition;
	Vector3 DistanceNolm;
	Vector3 Moved;
	Vector3 ReticlePos;
	Vector3 ShootVec;
	Vector3 PlayerToAimSaiteVec;
	Vector3 StartingPoint;
	Vector3 DownSlidingVelocity;
	Vector3 RotKeep;
	Vector3 BladeAttackVelocity;
	Vector3 BladeColRatio;
	Vector3 LightAtten = { 0.0f,0.1f,0.0f };
	Vector3 EnemyPos_;

	Vector4 fallVec;
	Vector4 ParticleStartPos;
	Vector4 ParticleEndPos;
	Vector4 ParticleEndPos_;
	Vector4 BoostStartPos;
	Vector4 LightDir = { 0,1,0,0 };

	MyStruct::BoostPos bonePos;
};