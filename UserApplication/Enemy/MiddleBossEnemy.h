#pragma once
#include "Vector3.h"
#include "Model.h"
#include "WorldTransform.h"
#include <Input.h>
#include "SplinePosition.h"
#include "NormalGun.h"
#include "Player.h"
#include "MissileGun.h"
#include <Sprite3D.h>
#include "EnemyHP3DUI.h"
#include "EnemyHP2DUI.h"
#include "Damage.h"
#include "EnemyBoostParticle.h"
#include "ParticleEditor.h"

/// <summary>
/// 中ボス
/// </summary>
class MiddleBossEnemy {
private:
	enum class AttackType
	{
		Nomal,//4つバラマキ
		Missile,//ミサイル
		MoveingAttack,//移動しながら
		Move,//移動
		NotAttack,//攻撃しない
		UltPreparationForBack,//ウルト準備のために戻る
		UltPreparation,//ウルト準備
		PutUltDown,//ウルト下ろす
		Ult,//ウルト
		Back//戻る
	};

public://基本関数
	MiddleBossEnemy(AudioManager* audioManager_);
	~MiddleBossEnemy();

	//初期化
	void Initialize(Player* player);
	//更新
	void Update();
	//描画
	void Draw(const ViewProjection& viewProjection_);
	//描画
	void FbxDraw(const ViewProjection& viewProjection_);
	//パーティクル描画
	void ParticleDraw(const ViewProjection& viewProjection_);
	//スプライト描画
	void DrawSprite(const ViewProjection& viewProjection_);
	//ムービー用更新
	bool MovieUpdate(const Vector3& StartPos, Vector3& EndPos);
	//タイトルムービー用更新
	void TitleMovieUpdate(const Vector3& startPos,Vector3& endPos);
	//タイトル用アップデート
	void TitleUpdate(const Vector3& TrackingLocation);
	//タイトルの弾を作る
	void MakeTitleMissileBullet();
	//タイトルリセット
	void ResetTitleMove();
	//CSアップデート
	void CSUpdate(ID3D12GraphicsCommandList* cmdList);

private://関数
	//タイマー
	void Timer();
	//生きてる時のアップデート
	void AliveUpdate();
	//アタック
	void Attack();
	//考える時間
	void ThinkingTime();
	//移動の値更新
	void WorldTransUpdate();
	//アタックタイプ変更
	void CheckAttackType();
	//タイプのランダム
	uint32_t RandomType(uint32_t& NoUseType);
	//当たり判定の場所のアップデート
	void ColTransUpdate();
	//当たり判定のアップデート
	void ColUpdate();
	//死んだときの爆発までのアップデート
	void DieMotionUpdate();
	//スタートに戻る関数
	void BackStartPoint(const Vector3& tmp);

public://Setter
	//スプラインセット
	void SetStageMoveSpline(const std::vector<Vector3>& points) { MoveSpline->SetNotSplineVector(points); }

public://Getter
	//指定位置にいるかどうか
	bool GetHowReturnSpline(const uint32_t& HowIndex)const { return MoveSpline->GetHowReturnIndex(HowIndex); }
	//スプラインが終わったか
	bool GetFinishSpline()const { return MoveSpline->GetFinishSpline(); }
	//死んだか
	bool GetIsDead()const { return isDead; }
	//死んだか
	bool GetIsDieMotion()const {
		return isDeadMotion;
	}
	//ポジションリターン
	Vector3 GetPosition() const;

	Vector3 GetEndPos() const;

	bool GetIsUltPreparation() const { return isUltPreparation; }
	bool GetIsUltChargeFin() const { return isUltChargeFin; }
	bool GetIsUlting() const { return isUlting; }
	bool GetIsUltExplosion() const { return isUltExplosion; }
	//ポジションリターン
	Vector4 GetUltPreparationPosition() const;

	float GetUltParticleSpeed()const {return UltDustParticle->GetShaderDetailParameters().Speed; }

private://const関連
	static const uint32_t BossEnemyBodyCount = 20;

	static const uint32_t AttackedKeepCount = 5;

	const uint32_t MoveFirstTime = 120;
	const uint32_t MoveOneMoreTime = 120;
	const uint32_t MaxAttackCoolTime = 50;
	const uint32_t MaxNomalTime = 50;
	const uint32_t MaxNomalBulletCoolTime = 5;
	const uint32_t MaxMissileTime = 100;
	const uint32_t MaxMissileBulletCoolTime = 50;
	const uint32_t MaxMoveingAttackTime = 40;
	const uint32_t MaxMoveingAttackBulletTime = 1;

	static const uint32_t ColCount = 9;

private://クラス関連
	Input* input_ = nullptr;
	std::unique_ptr<FBXModel> fbxModel_;
	std::unique_ptr<FBXObject3d> fbxObj3d_;
	std::unique_ptr<Model> model_;
	std::unique_ptr<Model> HeriHaneModel_;
	WorldTransform BossWorldTrans;
	WorldTransform HeriHaneLeftTrans;
	WorldTransform HeriHaneRightTrans;

	//当たり判定Position
	WorldTransform EnemyCenterWorldTrans;
	WorldTransform EnemyLeftHaneWorldTrans;
	WorldTransform EnemyRightHaneWorldTrans;
	WorldTransform EnemyLeftNormalWorldTrans;
	WorldTransform EnemyRightNormalWorldTrans;
	WorldTransform EnemyLeftMissileWorldTrans;
	WorldTransform EnemyRightMissileWorldTrans;
	WorldTransform EnemyNecWorldTrans;
	WorldTransform EnemyHedWorldTrans;

	WorldTransform UltWorldTrans;

	//スプライン
	std::unique_ptr<SplinePosition> MoveSpline;//途中のスプライン

	//弾
	std::unique_ptr<NormalGun> normalGunLeft;
	std::unique_ptr<NormalGun> normalGunRight;
	std::unique_ptr<MissileGun> missileGunLeft;
	std::unique_ptr<MissileGun> missileGunRight;
	std::unique_ptr<EnemyHP2DUI> enemyHP2DUI;
	std::unique_ptr<EnemyHP3DUI> enemyHP3DUI;
	// コライダー
	std::array<BaseCollider*,ColCount> MiddleBossCollider;
	BaseCollider* MiddleBossUltCollider;

	//プレイヤーのダメージ
	PlayerAttackDamage Damage;

	//パーティクル
	std::unique_ptr<Explosion> explosion;
	std::unique_ptr<EnemyBoostParticle> enemyBoostParticleLeft;
	std::unique_ptr<EnemyBoostParticle> enemyBoostParticleRight;

	std::unique_ptr<ParticleEditor> particleEditorLeft;
	std::unique_ptr<ParticleEditor> particleEditorRight;
	std::unique_ptr<ParticleEditor> UltDustParticle;
	std::unique_ptr<ParticleEditor> EnemyUltPreparationDownParticle;
	std::unique_ptr<ParticleEditor> EnemyUltDownExplosion;

	AudioManager* audioManager = nullptr;

private://イーナムクラス


private://別クラスから値をもらう
	Player* player = nullptr;

private://EnemyBossクラス変数

	bool isStart = false;
	bool isTurn = false;
	bool isSporn = false;
	bool isDead = false;
	bool isExplosion = false;
	bool isAllReadyExplosion = false;
	bool isDeadMotion = false;
	bool isAttack = false;
	bool isMoveing = false;
	bool isAngleGet = false;
	bool isOneMoreTime = false;
	bool isBackSponePos = false;
	bool isTitleShot = false;
	bool isDownSpeed = false;
	bool isDownSpeedFinish = false;
	bool isTitleBoss = false;
	bool isLightActive = true;
	bool isUltTime = false;
	bool isUltPreparation = false;
	bool isUltChargeFin = false;
	bool isUltDown = false;
	bool isUltExplosion = false;
	bool isUltShoot = false;
	bool isUlting = false;

	uint32_t BulletCoolTime = 0;
	uint32_t MoveingTimer = 0;
	uint32_t MaxMoveingTimer = 0;
	uint32_t MaxBulletCoolTime = 0;
	uint32_t AttackCooltime = 0;
	uint32_t AttackTypeCount = static_cast<uint32_t>(AttackType::Move);
	uint32_t AllAttackTypeCount = static_cast<uint32_t>(AttackType::NotAttack);
	uint32_t KeepAttackingTime = 0;
	uint32_t RotTime = 0;
	uint32_t mveType = 0;
	uint32_t MoveTimes = 0;
	uint32_t MaxMoveTimes = 3;
	uint32_t BackPosCounter = 1;
	uint32_t BackMissileFirstCoolTime = 20;
	uint32_t BackMissileCoolTime = 60;
	uint32_t BackMissileTimes = 0;
	uint32_t BackMissileMaxTimes = 10;
	uint32_t BulletMake = 10;
	uint32_t DownCount = 0;
	uint32_t MaxDownCount = 60;
	uint32_t BoostPosLeftStart = 8;
	uint32_t BoostPosLeftEnd = 9;
	uint32_t BoostPosLeftBackStart = 10;
	uint32_t BoostPosLeftBackEnd = 11;
	uint32_t BoostPosRightStart = 12;
	uint32_t BoostPosRightEnd = 13;
	uint32_t BoostPosRightBackStart = 14;
	uint32_t BoostPosRightBackEnd = 15;
	uint32_t LightLeftOne = 0;
	uint32_t LightLeftTwo = 0;
	uint32_t LightRightOne = 0;
	uint32_t LightRightTwo = 0;
	uint32_t LightBoostLeft = 0;
	uint32_t LightBoostRight = 0;
	uint32_t CircleShadowCount = 0;
	uint32_t DieMotionCount = 0;
	uint32_t DieMaxMotionCount = 150;
	uint32_t MaxParticleCountC = 300000;
	uint32_t ExplosionMaxTime = 300;
	uint32_t ExplosionTime = 0;

	//オーディオ
	uint32_t HeriSound = 0;
	float soundVol = 0.5f;
	float MaxSoundVol = 0.3f;
	float soundDistance = 0.0f;
	float SoundRadius = 300.0f;

	float Scale = 4.0f;
	float EnemySplineUpdate = 0.015f;
	float MaxScale = 10.0f;
	float MovieUpdateTimes = 0.0f;
	float MaxMovieUpdateTimes = 120.0f;
	float Radius = 12.0f;
	float UltRadius = 20.0f;
	float UltMaxRadius = 300.0f;
	float UltAddRadiusPow = 5.0f;
	float Angle = 0.0f;
	float AngleSize = 0.0f;
	float RotSpeed = 1.0f;
	float BulletSpeed = 6.0f;
	float MoveSafeRadius = 150.0f;
	float jampHeight = 30.0f;
	float BackStrength = 30.0f;
	float BackSpeed = 1.0f;
	float MovePower = 50.0f;
	float BackBosPower = 50.0f;
	float HeriHaneRotYLeft = 0.0f;
	float HeriHaneRotYRight = 0.0f;
	float HeriHaneRotSpeed = 0.1f;
	float BackLarpStrength = 0.1f;
	float BackPosRadius = 5.0f;
	float BoneColRadius = 9.0f;
	float NormalGunBackCol = 10.0f;
	float MaxMiddleBossHp = 7000.0f;
	float MiddleBossHp = MaxMiddleBossHp;
	float BoostEndPower = 0.7f;
	float LightDistance = 100.0f;
	float RotY = 0.0f;
	float RotAddY = 0.004f;
	float UltYPos = 150.0f;
	float UltYPosChargeFinPos = 100.0f;
	float UltYUpPow = 0.3f;
	float UltDownYPos = 3.0f;
	float UltDownYPow = 0.4f;

	Vector2 HpPosition = { 500.0f,45.0f };
	Vector2 LightAngle = { 0.0f,2.0f };

	Vector3 BonePos;
	Vector3 EndPos;
	Vector3 Velocity;
	Vector3 MovePos;
	Vector3 MoveStartPos;
	Vector3 BackLerpPos;
	Vector3 OldPos;
	Vector3 DownVelocity;
	Vector3 Coladjustment = { 0,3,0 };
	Vector3 HaneColadjustment = { 0,7,0 };
	Vector3 normalGunLeftPos;
	Vector3 normalGunRightPos;
	Vector3 missileGunLeftPos;
	Vector3 missileGunRightPos;
	Vector3 EnemyBoostLeftPosLight;
	Vector3 EnemyBoostRightPosLight;
	Vector3 DieMotionDown = { 0.0f,0.15f,0.0f };
	Vector3 LightAtten = { 0.0f,0.0f,0.1f };
	Vector3 UltPos;

	Vector4 LightDir = { 0,1,0,0 };

	AttackType attackType = AttackType::UltPreparationForBack;
	std::array<AttackType,AttackedKeepCount>oldAttackType;

	std::vector<Vector3> BackPoints{};
	MyStruct::EnemyBoostPos EnemyBoostLeftPos;
	MyStruct::EnemyBoostPos EnemyBoostRightPos;
};