#pragma once
#include "Vector3.h"
#include "Model.h"
#include "WorldTransform.h"
#include <Input.h>
#include "SplinePosition.h"
#include "MultiBullet.h"
#include "Player.h"
#include "MissileBullet.h"
#include <Sprite3D.h>
#include "EnemyHPUI.h"

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
	};

public://基本関数
	MiddleBossEnemy();
	~MiddleBossEnemy();

	//初期化
	void Initialize(Player* player);
	//タイトル用初期化
	void TitleInitialize();
	//更新
	void Update();
	//描画
	void Draw(const ViewProjection& viewProjection_);
	//スプライト描画
	void DrawSprite(const ViewProjection& viewProjection_);

	//ムービー用更新
	bool MovieUpdate(const Vector3& StartPos, Vector3& EndPos);

	//タイトル用アップデート
	void TitleUpdate(const Vector3& TrackingLocation);
	//弾を作る
	void MakeMissileBullet();
	//タイトルリセット
	void ResetTitleMove();

private://関数
	//タイマー
	void Timer();
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
	//ポジションリターン
	Vector3 GetPosition() const;

private://const関連
	static const uint32_t BossEnemyBodyCount = 20;

	static const uint32_t AttackedKeepCount = 5;

private://クラス関連
	Input* input_ = nullptr;
	std::unique_ptr<Model> model_;
	WorldTransform BossWorldTrans;

	WorldTransform DebugWorldTrans;

	//スプライン
	std::unique_ptr<SplinePosition> MoveSpline;//途中のスプライン

	//弾
	std::unique_ptr<MultiBullet> multiBullet;
	std::unique_ptr<MissileBullet> missileBullet;
	std::unique_ptr<EnemyHPUI> enemyHPUI;
	// コライダー
	BaseCollider* MiddleBossCollider = nullptr;

	std::unique_ptr<Sprite3D> HP;
	std::unique_ptr<Sprite3D> HPBackSprite;
	std::unique_ptr<Sprite3D> HPBarBackBarSprite;


private://イーナムクラス


private://別クラスから値をもらう
	Player* player = nullptr;

private://EnemyBossクラス変数

	bool isStart = false;
	bool isTurn = false;
	bool isSporn = false;
	bool isDead = false;
	bool isAttack = false;
	bool isMoveing = false;
	bool isAngleGet = false;
	bool isOneMoreTime = false;
	bool isBackSponePos = false;
	bool isTitleShot = false;

	uint32_t BulletCoolTime = 0;
	uint32_t MoveingTimer = 0;
	uint32_t MaxMoveingTimer = 0;
	uint32_t MaxBulletCoolTime = 0;
	uint32_t MiddleBossHp = 1000;
	uint32_t AttackCooltime = 0;
	uint32_t AttackTypeCount = static_cast<uint32_t>(AttackType::Move);
	uint32_t AllAttackTypeCount = static_cast<uint32_t>(AttackType::NotAttack) + 1;
	uint32_t KeepAttackingTime = 0;
	uint32_t RotTime = 0;
	uint32_t mveType = 0;
	uint32_t MoveTimes = 0;
	uint32_t MaxMoveTimes = 3;
	uint32_t BackPosCounter = 1;
	uint32_t BackMissileFirstCoolTime = 20;
	uint32_t BackMissileCoolTime = 6;
	uint32_t BackMissileTimes = 0;
	uint32_t BackMissileMaxTimes = 10;

	float EnemySplineUpdate = 0.015f;
	float MaxScale = 10.0f;
	float MovieUpdateTimes = 0.0f;
	float MaxMovieUpdateTimes = 120.0f;
	float Radius = 10.0f;
	float Angle = 0.0f;
	float AngleSize = 0.0f;
	float RotSpeed = 0.01f;
	float BulletSpeed = 6.0f;
	float MoveSafeRadius = 100.0f;
	float jampHeight = 20.0f;
	float BackStrength = 30.0f;
	float BackSpeed = 2.0f;
	float MovePower = 30.0f;
	float BackBosPower = 50.0f;

	Vector3 BonePos;
	Vector3 EndPos;
	Vector3 Velocity;
	Vector3 MovePos;
	Vector3 MoveStartPos;
	Vector3 BackLerpPos;

	AttackType attackType = AttackType::NotAttack;
	std::array<AttackType,AttackedKeepCount>oldAttackType;

	std::vector<Vector3> BackPoints{};
};