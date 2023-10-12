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
/// ミサイルクラス
/// </summary>
class MissileBullet {

public://基本関数
	MissileBullet();
	~MissileBullet();

	void Initialize();
	void Update(const Vector3& EndPos);
	void Draw(ViewProjection& viewProjection_);


public:

	//弾を作り出す
	void MakeBullet(Vector3& pos);

	void MakeSelectMissileBullet(Vector3& pos, Vector3& left, Vector3& top, Vector3& right, uint32_t& MakeCount);
	void MakeSelectMissileBullet(Vector3& pos, Vector3& upLeft, Vector3& upRight, Vector3& downLeft, Vector3& downRight, uint32_t& MakeCount);

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
	bool GetExpandingBullet() { return isExpanding; }

public://Setter


private://const関連
	static const uint32_t AllBulletCount = 100;

private://クラス関連
	std::unique_ptr<Model> model_;
	Input* input_ = nullptr;
	WorldTransform EnemyBulletWorldTrans[AllBulletCount];

	//当たり判定
	BaseCollider* BulletCollider[AllBulletCount];
	CollisionManager* collisionManager = nullptr;

private://別クラスから値をもらう


private://クラス変数
	bool isBulletAlive[AllBulletCount];
	bool isExpanding = false;
	bool isMovingExpandingBullet = false;
	bool isStartTracking[AllBulletCount];
	bool isNearPlayer[AllBulletCount];

	uint32_t BulletLifeTime[AllBulletCount];
	uint32_t BulletNum_ = 0;
	uint32_t MaxBulletLifeTime = 200;
	uint32_t BulletCoolTime = 0;
	uint32_t MaxBulletCoolTime = 5;
	uint32_t ParticleFile = 60;
	uint32_t MackPaticleMax = 1;
	uint32_t DieMaxParticle = 5;
	uint32_t makeBulletCount = 0;
	uint32_t BulletNotTrackingTime[AllBulletCount];

	//イージング
	uint32_t BulletEasingTime[AllBulletCount];
	uint32_t BulletMaxEasingTime[AllBulletCount];
	uint32_t BulletLarpEasingTime[AllBulletCount];
	uint32_t BulletLarpMaxEasingTime[AllBulletCount];

	float EnemyBulletSpeed[AllBulletCount];
	float BulletRadius[AllBulletCount];
	float EnemyBulletMaxRadius = 5.0f;
	float LarpT = 0.0f;
	float BulletSpeed[AllBulletCount];
	float BulletStartSpeed = 1.0f;
	float BulletEndSpeed = 3.0f;
	float BulletLerpSpeed[AllBulletCount];
	float BulletLerpStartSpeed = 0.04f;
	float BulletLrrpEndSpeed = 1.5f;
	float BulletStartLerpTime = 0.04f;

	Vector3 BulletVector[AllBulletCount];//打ち出される方向
	Vector3 BulletOldPos[AllBulletCount];
	Vector3 BulletVelocity[AllBulletCount];
};