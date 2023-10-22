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
/// マルチバレットクラス
/// </summary>
class MultiBullet {

public://基本関数
	MultiBullet();
	~MultiBullet();

	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw(const ViewProjection& viewProjection_);


public:

	//弾を作り出す
	void MakeBullet(Vector3& pos, Vector3& BulletVelocity, const float& BulletSpeed);

private:

	//弾の情報更新
	void WorldTransUpdate();
	//生きているかどうか
	void CheckBulletAlive();
	//死んでるときは奥深くに格納
	void SetNotAlivePosition();
	//1フレーム前のポジション
	//void OldPosUpdate();

public://Getter
	Vector3 GetEnemyBulletPos(const uint32_t& bulletCount)const { return MyMath::GetWorldTransform(EnemyBulletWorldTrans[bulletCount].matWorld_); }

public://Setter


private://const関連
	static const uint32_t AllBulletCount = 100;

private://クラス関連
	std::unique_ptr<Model> model_;
	WorldTransform EnemyBulletWorldTrans[AllBulletCount];

	//当たり判定
	BaseCollider* BulletCollider[AllBulletCount];
	CollisionManager* collisionManager = nullptr;

private://別クラスから値をもらう


private://クラス変数
	bool isBulletAlive[AllBulletCount];
	bool isMovingExpandingBullet = false;

	uint32_t BulletLifeTime[AllBulletCount];
	uint32_t BulletNum_ = 0;
	uint32_t MaxBulletLifeTime = 40;
	uint32_t BulletCoolTime = 0;
	uint32_t MaxBulletCoolTime = 5;
	uint32_t ParticleFile = 60;
	uint32_t MackPaticleMax = 1;
	uint32_t DieMaxParticle = 5;
	uint32_t makeBulletCount = 0;

	float EnemyBulletSpeed[AllBulletCount];
	float BulletRadius[AllBulletCount];


	Vector3 EnemyBulletMoveMent[AllBulletCount];//移動量
	Vector3 BulletVector[AllBulletCount];//打ち出される方向
	Vector3 BulletOldPos[AllBulletCount];//1フレーム前のポジション
};