#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "Input.h"
#include <memory>
#include "ViewProjection.h"
#include <BaseCollider.h>
#include <CollisionManager.h>
#include "Uint32Vector2.h"

/// <summary>
/// 敵の弾用クラス
/// </summary>
class EnemyBullet {

public://基本関数
	EnemyBullet();
	~EnemyBullet();

	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw(ViewProjection& viewProjection_);

public:
	//打ち出す球を作る
	uint32_t MakeEnemyBullet(const Vector3& MakeBulletPos, const Vector3& BulletVec, const float& bulletSpeed, const float& bulletLife);
	//弾のコントロール
	void BulletControl(const uint32_t& BulletNum, const Vector3& BulletVec);
	//長押しで曲げれるように
	void MakeExpandingStunBullet();
	//長押ししたバージョンのバレット拡大中更新
	void UpdateWhileExpanding(const Vector3& MakeBulletPos, const Vector3& BulletVec);

private:
	//プレーヤーの移動
	void BulletUpdate();
	//弾の情報更新
	void WorldTransUpdate();
	//タイマー更新
	void BulletAliveTimerUpdate();
	//生きているかどうか
	void CheckBulletAlive();
	//死んでるときは奥深くに格納
	void SetNotAlivePosition();
	//1フレーム前のポジション
	void OldPosUpdate();

public://Getter
	Vector3 GetEnemyBulletPos(const uint32_t& bulletCount)const { return MyMath::GetWorldTransform(EnemyBulletWorldTrans[bulletCount].matWorld_); }
	//弾を大きくするか
	bool GetExpandingBullet() { return isExpanding; }

public://Setter


private://const関連
	static const uint32_t AllBulletCount = 10;

private://クラス関連
	std::unique_ptr<Model> model_;
	WorldTransform EnemyBulletWorldTrans[AllBulletCount];

	//当たり判定
	BaseCollider* BulletCollider[AllBulletCount];
	CollisionManager* collisionManager = nullptr;
	
private://別クラスから値をもらう


private://クラス変数
	bool isBulletAlive[AllBulletCount];
	bool isExpanding = false;
	bool isMovingExpandingBullet = false;

	uint32_t BulletLifeTime[AllBulletCount];
	uint32_t BulletNum_ = 0;
	uint32_t MaxBulletLifeTime = 40;
	uint32_t BulletCoolTime = 0;
	uint32_t MaxBulletCoolTime = 5;
	uint32_t ParticleFile = 60;
	uint32_t MackPaticleMax = 1;
	uint32_t DieMaxParticle = 5;

	float EnemyBulletSpeed[AllBulletCount];
	float BulletRadius[AllBulletCount];
	float BulletRadiusMax = 0.4f;
	float EnemyBulletMaxRadius = 5.0f;
	float EnemyParticleSpeed = 0.02f;
	float EnemyParticleDieSpeed = 0.2f;

	Vector3 EnemyBulletMoveMent[AllBulletCount];//移動量
	Vector3 BulletVector[AllBulletCount];//打ち出される方向
	Vector3 BulletOldPos[AllBulletCount];//1フレーム前のポジション
};