#pragma once
#include "Vector3.h"
#include "Model.h"
#include "WorldTransform.h"
#include <Input.h>
#include "SplinePosition.h"
#include "MultiBullet.h"
#include "Player.h"
#include "MissileBullet.h"


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

	void Initialize(Player* player);
	void TitleInitialize();
	void Update();
	void Draw(ViewProjection& viewProjection_);

	bool MovieUpdate(const Vector3& StartPos, Vector3& EndPos);

	void TitleUpdate(const Vector3& TrackingLocation);
	void MakeMissileBullet();
	void ResetTitleMove();

private://関数
	void Timer();
	void Attack();
	void ThinkingTime();
	void WorldTransUpdate();//移動の値更新
	void CheckAttackType();
	uint32_t RandomType(uint32_t& NoUseType);

public://Setter

	void SetStageMoveSpline(const std::vector<Vector3>& points) { MoveSpline->SetNotSplineVector(points); }

public://Getter

	bool GetHowReturnSpline(const uint32_t& HowIndex)const { return MoveSpline->GetHowReturnIndex(HowIndex); }
	bool GetFinishSpline()const { return MoveSpline->GetFinishSpline(); }

	bool GetIsDead()const { return isDead; }

	Vector3 GetPosition() const;

private://const関連
	static const uint32_t BossEnemyBodyCount = 20;

	static const uint32_t AttackedKeepCount = 8;

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

	// コライダー
	BaseCollider* MiddleBossCollider = nullptr;

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
	uint32_t MiddleBossHp = 20;
	uint32_t AttackCooltime = 0;
	uint32_t AttackTypeCount = static_cast<uint32_t>(AttackType::Move);
	uint32_t AllAttackTypeCount = static_cast<uint32_t>(AttackType::NotAttack) + 1;
	uint32_t KeepAttackingTime = 0;
	uint32_t RotTime = 0;
	uint32_t mveType = 0;
	uint32_t MoveTimes = 0;
	uint32_t MaxMoveTimes = 5;
	uint32_t BackPosCounter = 1;

	float EnemySplineUpdate = 0.015f;
	float MaxScale = 10.0f;
	float MovieUpdateTimes = 0.0f;
	float MaxMovieUpdateTimes = 120.0f;
	float Radius = 10.0f;
	float Angle = 0.0f;
	float AngleSize = 0.0f;
	float RotSpeed = 1.0f;
	float BulletSpeed = 6.0f;
	float MoveSafeRadius = 40.0f;
	float jampHeight = 20.0f;
	float BackStrength = 30.0f;
	float BackSpeed = 2.0f;

	Vector3 BonePos;
	Vector3 EndPos;
	Vector3 Velocity;
	Vector3 MovePos;
	Vector3 MoveStartPos;
	Vector3 BackLerpPos;

	AttackType attackType = AttackType::NotAttack;
	AttackType oldAttackType[AttackedKeepCount];

	std::vector<Vector3> BackPoints{};
};