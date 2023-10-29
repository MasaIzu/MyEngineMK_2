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
#include "CollisionAttribute.h"

/// <summary>
/// ノーマルバレットクラス
/// </summary>
class NormalBullet
{
public://基本関数
	NormalBullet(const unsigned short Attribute_);
	~NormalBullet();

	//初期化
	void Initialize(Model* BulletModel);
	//更新
	void Update();
	//描画
	void Draw(const ViewProjection& viewProjection_);


public:

	//弾を作り出す
	void MakeBullet(const Vector3& pos,const Vector3& BulletVelocity, const float& BulletSpeed);

private:

	//弾の情報更新
	void WorldTransUpdate();
	//生きているかどうか
	void CheckBulletAlive();

public://Getter
	//Aliveのゲッター
	bool GetBulletAlive() const;
	Vector3 GetEnemyBulletPos() const { return MyMath::GetWorldTransform(BulletWorldTrans.matWorld_); }

public://Setter


private://const関連


private://クラス関連
	Model* model_ = nullptr;
	WorldTransform BulletWorldTrans;

	//当たり判定
	BaseCollider* BulletCollider;
	CollisionManager* collisionManager = nullptr;

private://別クラスから値をもらう


private://クラス変数

	unsigned short Attribute;

	bool isBulletAlive = false;
	bool isMovingExpandingBullet = false;

	uint32_t BulletLifeTime = 0;
	uint32_t BulletNum_ = 0;
	uint32_t MaxBulletLifeTime = 40;
	uint32_t BulletCoolTime = 0;
	uint32_t MaxBulletCoolTime = 5;
	uint32_t ParticleFile = 60;
	uint32_t MackPaticleMax = 1;
	uint32_t DieMaxParticle = 5;
	uint32_t makeBulletCount = 0;

	float EnemyBulletSpeed = 0.0f;
	float BulletRadius = 0.0f;


	Vector3 EnemyBulletMoveMent;//移動量
	Vector3 BulletVector;//打ち出される方向
	Vector3 BulletOldPos;//1フレーム前のポジション
};