#pragma once
#include <ViewProjection.h>
#include <WorldTransform.h>
#include <Model.h>
#include <Input.h>

class TutorialEnemy {

public://基本関数
	TutorialEnemy(const Vector3& BonePos_);
	~TutorialEnemy();

	void Initialize();
	void Update();
	void Draw(ViewProjection& viewProjection_);

private:
	//敵を見つけた時の動きと見つけてない時の動き
	void PlayerFoundMove();
	void PlayerNotFoundMove();
	//上の関数のタイマー
	void PlayerFoundMoveTimer();
	void PlayerNotFoundMoveTimer();

	//プレーヤーの移動の値更新
	void WorldTransUpdate();
	//ランダム
	uint32_t Random(const uint32_t& low, const uint32_t& high);

private://クラス変数
	Input* input_ = nullptr;
	std::unique_ptr<Model> model_;
	std::unique_ptr<Model> modelDebug_;
	WorldTransform enemyWorldTrans;
	WorldTransform DebugWorldTrans;
private:
	enum class NotFoundPhase {
		Walk,//歩く
		Stop,//止まる
		Interruption,//中断
		ForcedWalking,//強制歩き
	};
	//見つけてないときの動きフェーズ
	NotFoundPhase NotFoundPhase_ = NotFoundPhase::Walk;

	enum class FoundPhase {
		Walk,//歩く
		Stop,//止まる
		Attack,//攻撃
	};
	//見つけた時の動きフェーズ
	FoundPhase FoundPhase_ = FoundPhase::Walk;

private:
	bool isPlayerFound = 0;

	uint32_t WalkTime = 0;
	uint32_t StopTime = 0;
	uint32_t AttackWalkTime = 0;
	uint32_t AttackStopTime = 0;

	float TerritoryRadius = 20.0f;
	float EnemySpeed = 0.12f;
	float Rot = 0.0f;
	float dist = 0.0f;
	float radius2 = 0.0f;

	Vector3 enemyMoveMent;
	Vector3 BonePos;
	Vector3 tmp;
	

};