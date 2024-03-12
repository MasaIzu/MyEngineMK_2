#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include <memory>
#include "ViewProjection.h"
#include <BaseCollider.h>
#include <CollisionManager.h>
#include "Uint32Vector2.h"

/// <summary>
/// ミサイルクラス
/// </summary>
class MissileBullet
{

public://基本関数
	MissileBullet(const unsigned short Attribute_);
	~MissileBullet();

	//初期化
	void Initialize(Model* BulletModel);
	//更新
	void Update(const Vector3& EndPos);
	//描画
	void Draw(const ViewProjection& viewProjection_);


public:

	//弾を作り出す
	void MakeMissileBullet(const Vector3& pos,const Vector3& velocity,const float& speed);

private:
	//弾の情報更新
	void WorldTransUpdate();
	//生きているかどうか
	void CheckBulletAlive();

public://Getter
	bool GetBulletAlive() const;
	//バレットの位置ゲット
	Vector3 GetEnemyBulletPos()const {
		return MyMath::GetWorldTransform(BulletWorldTrans.matWorld_);
	}

public://Setter


private://const関連

private://クラス関連
	Model* model_ = nullptr;
	WorldTransform BulletWorldTrans;

	//当たり判定
	BaseCollider* BulletCollider = nullptr;
	CollisionManager* collisionManager = nullptr;

private://別クラスから値をもらう


private://クラス変数

	unsigned short Attribute = 0;

	bool isBulletAlive = false;
	bool isMovingExpandingBullet = false;
	bool isStartTracking = false;
	bool isNearPlayer = false;

	uint32_t BulletLifeTime = 0;
	uint32_t BulletNum_ = 0;
	uint32_t MaxBulletLifeTime = 200;
	uint32_t BulletCoolTime = 0;
	uint32_t MaxBulletCoolTime = 5;
	uint32_t ParticleFile = 60;
	uint32_t MackPaticleMax = 1;
	uint32_t DieMaxParticle = 5;
	uint32_t makeBulletCount = 0;
	uint32_t BulletNotTrackingTime = 0;
	uint32_t MaxBulletNotTrackingTime = 30;

	//イージング
	uint32_t BulletEasingTime = 80;
	uint32_t BulletMaxEasingTime = 80;
	uint32_t BulletLarpEasingTime = 60;
	uint32_t BulletLarpMaxEasingTime = 60;

	float BulletRadius = 0.5f;
	float EnemyBulletMaxRadius = 5.0f;
	float LarpT = 0.0f;
	float BulletSpeed = 1.0f;
	float BulletStartSpeed = 1.0f;
	float BulletEndSpeed = 2.0f;
	float BulletLerpSpeed = 0.04f;
	float BulletLerpStartSpeed = 0.04f;
	float BulletLrrpEndSpeed = 1.0f;
	float BulletStartLerpTime = 0.04f;
	float NotSearchArea = 25.0f;
	float LerpStrength = 0.005f;

	Vector3 BulletOldPos;
	Vector3 BulletVelocity;
};