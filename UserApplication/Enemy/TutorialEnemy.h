#pragma once
#include <ViewProjection.h>
#include <WorldTransform.h>
#include <Model.h>
#include <Input.h>
#include <BaseCollider.h>
#include <CollisionManager.h>

class TutorialEnemy {

public://基本関数
	TutorialEnemy(const Vector3& BonePos_);
	~TutorialEnemy();

	void Initialize();
	void Update(const Vector3& PlayerPos);
	void Draw(ViewProjection& viewProjection_);

private:
	//プレイヤーの弾が当たった時の処理
	void PlayerBulletHit();

	//敵を見つけた時の動きと見つけてない時の動き
	void PlayerNotFoundMove();
	void PlayerFoundMove();
	//上の関数のタイマー
	void PlayerNotFoundMoveTimer();
	void PlayerFoundMoveTimer();
	//プレイヤーのサーチ関数
	void SearchingPlayer();
	//角度をとる
	void GetPlayerForEnemyAngle();
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

	//当たり判定
	BaseCollider* TutorialEnemyCollider = nullptr;
	CollisionManager* collisionManager = nullptr;

private:
	enum class NotFoundPhase {
		Walk,//歩く
		Stop,//止まる
		Interruption,//中断
		ForcedWalking,//強制歩き
		FoundPlayer,//プレイヤーを見つけた
		Nothing,//何もしない
	};
	//見つけてないときの動きフェーズ
	NotFoundPhase NotFoundPhase_ = NotFoundPhase::Walk;

	enum class FoundPhase {
		Intimidation,//威嚇
		Walk,//歩く
		Stop,//止まる
		Attack,//攻撃
		Turn,//プレイヤーの方を向く
		Nothing,//何もしない
	};
	//見つけた時の動きフェーズ
	FoundPhase FoundPhase_ = FoundPhase::Nothing;

private://外から持ってきたものの格納
	Vector3 playerPos;

private:
	bool isPlayerFound = false;
	bool isAlive = false;

	uint32_t WalkTime = 0;
	uint32_t StopTime = 0;
	uint32_t AttackWalkTime = 0;
	uint32_t AttackStopTime = 0;
	uint32_t TurnTimes = 60;

	float TerritoryRadius = 20.0f;
	float SearchingAreaRadius = 25.0f;
	float EnemySpeed = 0.12f;
	float Rot = 0.0f;
	float dist = 0.0f;
	float radius = 0.0f;
	float EnemyRadius = 1.0f;
	float dotPlayerAndEnemy = 0;
	float BackAngle = 180.0f;
	

	Vector3 enemyMoveMent;
	Vector3 BonePos;
	Vector3 tmp;
	

};