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
class MissileBullet
{

public://基本関数
	MissileBullet();
	~MissileBullet();

	//初期化
	void Initialize();
	//更新
	void Update(const Vector3& EndPos);
	//描画
	void Draw(const ViewProjection& viewProjection_);


public:

	//弾を作り出す
	void MakeBullet(Vector3& pos);
	void MakeSelectMissileBullet(Vector3& pos,Vector3& left,Vector3& top,Vector3& right,uint32_t& MakeCount);
	void MakeSelectMissileBullet(Vector3& pos,Vector3& upLeft,Vector3& upRight,Vector3& downLeft,Vector3& downRight,uint32_t& MakeCount);

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
	//バレットの位置ゲット
	Vector3 GetEnemyBulletPos(const uint32_t& bulletCount)const {
		return MyMath::GetWorldTransform(EnemyBulletWorldTrans[ bulletCount ].matWorld_);
	}

public://Setter


private://const関連
	static const uint32_t AllBulletCount = 100;

private://クラス関連
	std::unique_ptr<Model> model_;
	Input* input_ = nullptr;
	std::array<WorldTransform,AllBulletCount> EnemyBulletWorldTrans;

	//当たり判定
	std::array<BaseCollider* , AllBulletCount> BulletCollider;
	CollisionManager* collisionManager = nullptr;

private://別クラスから値をもらう


private://クラス変数
	std::array<bool,AllBulletCount> isBulletAlive;
	bool isMovingExpandingBullet = false;
	std::array<bool,AllBulletCount> isStartTracking;
	std::array<bool,AllBulletCount> isNearPlayer;

	std::array<uint32_t,AllBulletCount> BulletLifeTime;
	uint32_t BulletNum_ = 0;
	uint32_t MaxBulletLifeTime = 200;
	uint32_t BulletCoolTime = 0;
	uint32_t MaxBulletCoolTime = 5;
	uint32_t ParticleFile = 60;
	uint32_t MackPaticleMax = 1;
	uint32_t DieMaxParticle = 5;
	uint32_t makeBulletCount = 0;
	std::array<uint32_t,AllBulletCount> BulletNotTrackingTime;

	//イージング
	std::array<uint32_t,AllBulletCount> BulletEasingTime;
	std::array<uint32_t,AllBulletCount> BulletMaxEasingTime;
	std::array<uint32_t,AllBulletCount> BulletLarpEasingTime;
	std::array<uint32_t,AllBulletCount> BulletLarpMaxEasingTime;

	std::array<float,AllBulletCount> BulletRadius;
	float EnemyBulletMaxRadius = 5.0f;
	float LarpT = 0.0f;
	std::array<float,AllBulletCount> BulletSpeed;
	float BulletStartSpeed = 1.0f;
	float BulletEndSpeed = 3.0f;
	std::array<float,AllBulletCount> BulletLerpSpeed;
	float BulletLerpStartSpeed = 0.04f;
	float BulletLrrpEndSpeed = 1.5f;
	float BulletStartLerpTime = 0.04f;

	std::array<Vector3,AllBulletCount> BulletOldPos;
	std::array<Vector3,AllBulletCount> BulletVelocity;
};