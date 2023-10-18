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
/// プレイヤー用弾クラス
/// </summary>
class NormalBullet {

public://基本関数
	NormalBullet();
	~NormalBullet();

	//初期化
	void Initialize(Model* model);
	//更新
	void Update();
	//描画
	void Draw(const ViewProjection& viewProjection_);


public:
	//打ち出す球を作る
	void MakePlayerBullet(const Vector3& MakeBulletPos,const Vector3& BulletVec);

private:
	//プレーヤーの移動
	void BulletUpdate();
	//弾の情報更新
	void WorldTransUpdate();
	//生きているかどうか
	void CheckBulletAlive();
	//死んでるときは奥深くに格納
	void SetNotAlivePosition();

	//1フレーム前のポジション
	void OldPosUpdate();

public://Getter
	//バレット場所をゲット
	Vector3 GetPlayerBulletPos()const { return MyMath::GetWorldTransform(playerBulletWorldTrans.matWorld_); }
	bool GetBulletAlive()const {
		return isBulletAlive;
	}

public://Setter
	

private://const関連
	

private://クラス関連
	Model* model_ = nullptr;
	WorldTransform playerBulletWorldTrans;

	//当たり判定
	BaseCollider* BulletCollider;
	CollisionManager* collisionManager = nullptr;

private://別クラスから値をもらう
	

private://プレイヤークラス変数
	bool isBulletAlive;
	bool isMovingExpandingBullet = false;
	bool isBulletDownSpeed;

	uint32_t BulletLifeTime;
	uint32_t BulletSpeedDownTime;
	uint32_t BulletNum_ = 0;
	uint32_t MaxBulletLifeTime = 60;
	uint32_t BulletCoolTime = 0;
	uint32_t MaxBulletCoolTime = 7;
	uint32_t ParticleFile = 60;
	uint32_t MackPaticleMax = 1;
	uint32_t AttackMaxParticle = 10;
	uint32_t MakeBulletMaxParticle = 8;
	uint32_t DieMaxParticle = 30;
	uint32_t DieMaxParticleLife = 40;

	float playerBulletSpeed;
	float BulletRadius;
	float playerBulletMaxRadius = 5.0f;
	float PlayerParticleSpeed = 0.004f;
	float PlayerBulletMakeParticleSpeed = 0.2f;
	float PlayerParticleDieSpeed = 0.2f;

	Vector3 playerBulletMoveMent;//移動量
	Vector3 BulletVector;//打ち出される方向
	Vector3 BulletOldPos;//1フレーム前のポジション
};