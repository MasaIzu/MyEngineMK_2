#pragma once
#include <ViewProjection.h>
#include <WorldTransform.h>
#include <Model.h>
#include <Input.h>
#include <BaseCollider.h>
#include <CollisionManager.h>
#include "SplinePosition.h"
#include "EnemyBullet.h"

/// <summary>
/// 弾を撃つ雑魚敵クラス
/// </summary>
class BulletShotEnemy {

public://基本関数
	BulletShotEnemy(const Vector3& BonePos_, Model* model);
	~BulletShotEnemy();

	//初期化
	void Initialize();
	//更新
	void Update(const Vector3& PlayerPos);
	//描画
	void Draw(ViewProjection& viewProjection_);

public://getter
	//死んでるかどうか
	bool GetIsDead() { return isDead; }
	//ポジションゲット
	Vector3 GetBulletShotEnemyPos() const { return MyMath::GetWorldTransform(BulletShotEnemyWorldTrans.matWorld_); };
private:
	//動きについての関数
	void PlayerBulletHit();//プレイヤーの弾が当たった時の処理
	void PlayerNotSpottedMove();//敵を見つけてない時の動き
	void PlayerSpottedMove();//敵を見つけた時の動き
	void Attack();

	//敵の行動についての関数タイマー
	void PlayerNotSpottedMoveTimer();//敵を見つけてない時
	void PlayerSpottedMoveTimer();//敵を見つけた時

	// 敵の地面の当たり判定
	void CheckCollider();

	//敵の視線切れているか
	bool CheckBetweenToPlayerCollider();

	//雑務系関数
	bool GetIsAttackArea();//攻撃できる範囲かどうか
	void SearchingPlayer();//プレイヤーのサーチ関数
	void GetPlayerForEnemyAngle();//角度をとる
	void WorldTransUpdate();//Enemyだけの移動の値更新
	uint32_t Random(const uint32_t& low, const uint32_t& high);//ランダム


private://const関連
	static const uint32_t ColliderSphereCount = 2;

private://クラス変数
	Input* input_ = nullptr;
	Model* model_ = nullptr;
	std::unique_ptr<Model> modelDebug_;
	WorldTransform BulletShotEnemyWorldTrans;
	WorldTransform BulletShotEnemyWorldTransHed;
	WorldTransform DebugWorldTrans;

	//当たり判定
	BaseCollider* BulletShotEnemyCollider[ColliderSphereCount];
	CollisionManager* collisionManager = nullptr;

	//スプライン曲線
	std::unique_ptr<SplinePosition> splinePosition;

	//バレット
	std::unique_ptr<EnemyBullet> enemyBullet;

private://イーナム
	enum class NotSpottedPhase {
		Walk,//歩く
		Stop,//止まる
		Interruption,//中断
		ForcedWalking,//強制歩き
		SpottedPlayer,//プレイヤーを見つけた
		Nothing,//何もしない
	};
	//見つけてないときの動きフェーズ
	NotSpottedPhase NotSpottedPhase_ = NotSpottedPhase::Walk;

	enum class SpottedPhase {
		Intimidation,//威嚇
		Walk,//歩く
		Stop,//止まる
		Attack,//攻撃
		Wait,//待つ
		LoseSightofPlayer,//プレイヤーを見失なった時
		Turn,//プレイヤーの方を向く
		Nothing,//何もしない
	};
	//見つけた時の動きフェーズ
	SpottedPhase SpottedPhase_ = SpottedPhase::Nothing;

	enum class AttackPhase {
		NormalAttack,//普通の攻撃
		RunAttack,//走って攻撃
		Nothing,//何もしない
	};
	//見つけた時の動きフェーズ
	AttackPhase AttackPhase_ = AttackPhase::Nothing;

private://外から持ってきたものの格納
	Vector3 playerPos;

private:
	bool IsPlayerSpotted = false;
	bool IsAlive = false;
	bool IsAttack = false;
	bool IsEnemyHasADestination = false;
	bool IsNeverSpotted = true;
	bool onGround = false;
	bool isLanding = false;
	bool isDead = false;

	uint32_t WalkTime = 0;
	uint32_t StopTime = 0;
	uint32_t AttackWalkTime = 0;
	uint32_t AttackStopTime = 0;
	uint32_t TurnTimes = 60;
	uint32_t RunAttackTime = 60;
	uint32_t AttackDelayTime = 0;
	uint32_t MaxAttackDelayTime = 120;
	uint32_t bulletNumber = 0;

	float TerritoryRadius = 20.0f;
	float AttackAreaRadius = 5.0f;
	float SearchingAreaRadius = 92.0f;
	float SpottedLookingPlayerRadius = 150.0f;
	float EnemySpeed = 0.09f;
	float SpottedEnemySpeed = 0.1f;
	float EnemyRadius = 1.0f;
	float EnemyToPlayerAngle = 0;
	float BackAngle = 180.0f;
	float HowAboutFarAway = 0.0f;
	float RunAttackSpeed = 0.4f;
	float LittleFar = 5.0f;
	float Distance = 0.0f;
	float EnemyBulletSpeed = 0.5f;
	//判定をするよう
	float Rot = 0.0f;
	float dist = 0.0f;
	float radius = 0.0f;
	float NormalAttackSpeed = 0.02f;

	Vector3 ReticlePos;
	Vector3 DistanceNolm;
	Vector3 enemyMoveMent;
	Vector3 BonePos;
	Vector3 tmp;
	Vector3 EnemyToPlayerVec;
	Vector3 BackBonePos;
	Vector3 DestinationPos;
	Vector3 OldTansPos;

	//スプライン曲線に必要なメンバ変数
	Vector3 start = { 0.0f, 0.0f, 0.0f };	//スタート地点
	Vector3 p1 = { 0.0f, 0.0f, 0.0f };		//制御点その1
	Vector3 p2 = { 0.0f, 0.0f, 0.0f };		//制御点その2
	Vector3 p3 = { 0.0f, 0.0f, 0.0f };		//制御点その3
	Vector3 end = { 0.0f, 0.0f, 0.0f };		//ゴール地点
	//std::vector<Vector3> points{ start, start, p1, p2, end, end };見本

	Vector4 fallVec;
};