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

public://基本関数
	MiddleBossEnemy();
	~MiddleBossEnemy();

	void Initialize(Player* player);
	void Update();
	void Draw(ViewProjection& viewProjection_);

	bool MovieUpdate(const Vector3& StartPos, Vector3& EndPos);

private://関数

	void WorldTransUpdate();//移動の値更新

public://Setter

	void SetStageMoveSpline(const std::vector<Vector3>& points) { MoveSpline->SetNotSplineVector(points); }

public://Getter

	bool GetHowReturnSpline(const uint32_t& HowIndex)const { return MoveSpline->GetHowReturnIndex(HowIndex); }
	bool GetFinishSpline()const { return MoveSpline->GetFinishSpline(); }

private://const関連
	static const uint32_t BossEnemyBodyCount = 20;

private://クラス関連
	Input* input_ = nullptr;
	std::unique_ptr<Model> model_;
	WorldTransform BossWorldTrans;

	WorldTransform DebugWorldTrans;
	ViewProjection* viewProjection_ = nullptr;

	//スプライン
	std::unique_ptr<SplinePosition> MoveSpline;//途中のスプライン

	//弾
	std::unique_ptr<MultiBullet> multiBullet;
	std::unique_ptr<MissileBullet> missileBullet;

private://イーナムクラス


private://別クラスから値をもらう
	Player* player = nullptr;

private://EnemyBossクラス変数

	bool isStart = false;
	bool isSporn = false;

	uint32_t BulletCoolTime = 0;

	float EnemySplineUpdate = 0.015f;
	float MaxScale = 10.0f;
	float MovieUpdateTimes = 0.0f;

	Vector3 BonePos;
	Vector3 EndPos;
	Vector3 Velocity;

};