#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "Input.h"
#include "affin.h"
#include <memory>
#include "ViewProjection.h"
#include <BaseCollider.h>
#include <CollisionManager.h>
#include "ParticleManager.h"
#include "Uint32Vector2.h"

class PlayerBullet {

public://基本関数
	PlayerBullet();
	~PlayerBullet();

	void Initialize();
	void Update();
	void Draw(ViewProjection& viewProjection_);
	void CSUpdate(ID3D12GraphicsCommandList* cmdList);
	void ParticleDraw(ViewProjection& viewProjection_);
	void CopyParticle();

public:
	//打ち出す球を作る
	uint32_t MakePlayerBullet(const Vector3& MakeBulletPos,const Vector3& BulletVec);
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
	//パーティクルを出す
	void MakeParticle(Vector3& pos, Vector3& BulletVelocity,const float& BulletSpeed);
	//1フレーム前のポジション
	void OldPosUpdate();

public://Getter
	Vector3 GetPlayerBulletPos(const uint32_t& bulletCount)const { return MyMath::GetWorldTransform(playerBulletWorldTrans[bulletCount].matWorld_); }
	bool GetExpandingBullet() { return isExpanding; }

public://Setter
	

private://const関連
	static const uint32_t AllBulletCount = 30;

private://クラス関連
	std::unique_ptr<Model> model_;
	WorldTransform playerBulletWorldTrans[AllBulletCount];

	//当たり判定
	BaseCollider* BulletCollider[AllBulletCount];
	CollisionManager* collisionManager = nullptr;
	std::unique_ptr<ParticleManager> ParticleMan;
private://別クラスから値をもらう
	

private://プレイヤークラス変数
	bool isBulletAlive[AllBulletCount];
	bool isExpanding = false;
	bool isMovingExpandingBullet = false;

	uint32_t BulletLifeTime[AllBulletCount];
	uint32_t BulletNum_ = 0;
	uint32_t MaxBulletLifeTime = 60;
	uint32_t BulletCoolTime = 0;
	uint32_t MaxBulletCoolTime = 5;
	uint32_t ParticleFile = 60;
	uint32_t MackPaticleMax = 1;

	float playerBulletSpeed = 10.0f;
	float BulletRadius[AllBulletCount];
	float playerBulletMaxRadius = 5.0f;
	float PlayerParticleSpeed = 0.02f;

	Vector3 playerBulletMoveMent[AllBulletCount];//移動量
	Vector3 BulletVector[AllBulletCount];//打ち出される方向
	Vector3 BulletOldPos[AllBulletCount];//1フレーム前のポジション
};