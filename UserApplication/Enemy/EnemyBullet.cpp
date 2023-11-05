#include "EnemyBullet.h"
#include <imgui.h>
#include <CollisionManager.h>
#include <SphereCollider.h>
#include <CollisionAttribute.h>
#include <Numbers.h>

EnemyBullet::EnemyBullet()
{
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		isBulletAlive[i] = static_cast< uint32_t >( Numbers::Zero );
		BulletLifeTime[i] = MaxBulletLifeTime;
		BulletRadius[i] = FloatNumber(fNumbers::fPointFour);
		EnemyBulletSpeed[i] = FloatNumber(fNumbers::fZero);
		BulletCollider[i] = nullptr;
	}
}

EnemyBullet::~EnemyBullet()
{

}

void EnemyBullet::Initialize()
{

	model_.reset(Model::CreateFromOBJ("sphereBulletEnemy", true));
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		EnemyBulletWorldTrans[i].Initialize();
		EnemyBulletWorldTrans[i].scale_ = { BulletRadius[i],BulletRadius[i],BulletRadius[i] };
		EnemyBulletWorldTrans[i].TransferMatrix();

		isBulletAlive[i] = static_cast< uint32_t >( Numbers::Zero );
	}


	for (uint32_t i = 0; i < AllBulletCount; i++) {
		// コリジョンマネージャに追加
		BulletCollider[i] = new SphereCollider(Vector4(0, BulletRadius[i], 0, 0), BulletRadius[i]);
		CollisionManager::GetInstance()->AddCollider(BulletCollider[i]);
		BulletCollider[i]->SetAttribute(COLLISION_ATTR_ATTACK);
	}
	collisionManager = CollisionManager::GetInstance();

	//死んでたら格納
	SetNotAlivePosition();
}

void EnemyBullet::Update()
{
	//前回の位置を記録
	OldPosUpdate();
	//タイマー更新
	BulletAliveTimerUpdate();
	//時間が切れているかどうか
	CheckBulletAlive();
	//生きていたらアプデ
	BulletUpdate();
	//死んでたら格納
	SetNotAlivePosition();

	for (uint32_t i = 0; i < AllBulletCount; i++) {
		BulletCollider[i]->Update(EnemyBulletWorldTrans[i].matWorld_, BulletRadius[i], EnemyBulletSpeed[i], BulletVector[i]);
		BulletCollider[i]->SetAttribute(COLLISION_ATTR_ENEMY_BULLET_ATTACK);
	}
}

void EnemyBullet::Draw(ViewProjection& viewProjection_)
{
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		if (isBulletAlive[i] == true) {
			model_->Draw(EnemyBulletWorldTrans[i], viewProjection_);
		}
	}
}


void EnemyBullet::BulletUpdate()
{

	for (uint32_t i = 0; i < AllBulletCount; i++) {
		if (isBulletAlive[i] == true) {
			EnemyBulletMoveMent[i] = BulletVector[i] * EnemyBulletSpeed[i];
			if (BulletCollider[i]->GetSphereMeshHit()) {
				isBulletAlive[i] = false;
				BulletCollider[i]->SphereMeshHitReset();
			}
		}
	}


	WorldTransUpdate();
}

uint32_t EnemyBullet::MakeEnemyBullet(const Vector3& MakeBulletPos, const Vector3& BulletVec, const float& bulletSpeed, const float& bulletLife)
{
	if (BulletCoolTime <= 0) {
		for (uint32_t i = 0; i < AllBulletCount; i++) {
			if (isBulletAlive[i] == false) {
				isBulletAlive[i] = true;
				EnemyBulletWorldTrans[i].translation_ = MakeBulletPos;
				BulletVector[i] = BulletVec;
				BulletLifeTime[i] = static_cast<uint32_t>(bulletLife);
				BulletRadius[i] = BulletRadiusMax;
				EnemyBulletSpeed[i] = bulletSpeed;
				EnemyBulletWorldTrans[i].scale_ = Vector3(BulletRadius[i], BulletRadius[i], BulletRadius[i]);
				BulletCoolTime = MaxBulletCoolTime;
				return i;
			}
		}
	}
	return 0;
}

void EnemyBullet::BulletControl(const uint32_t& BulletNum, const Vector3& BulletVec)
{
	BulletVector[BulletNum] = BulletVec;
}

void EnemyBullet::MakeExpandingStunBullet()
{
	if (isExpanding == false) {
		for (uint32_t i = 0; i < AllBulletCount; i++) {
			if (isBulletAlive[i] == false) {
				isBulletAlive[i] = true;
				BulletLifeTime[i] = MaxBulletLifeTime;
				BulletRadius[i] = BulletRadiusMax;
				isExpanding = true;
				BulletNum_ = i;
				break;
			}
		}
	}
}

void EnemyBullet::UpdateWhileExpanding(const Vector3& MakeBulletPos, const Vector3& BulletVec)
{
	if (isExpanding) {
		EnemyBulletWorldTrans[BulletNum_].translation_ = MakeBulletPos;
		BulletVector[BulletNum_] = BulletVec;
	}
}

void EnemyBullet::WorldTransUpdate()
{
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		EnemyBulletWorldTrans[i].translation_ += EnemyBulletMoveMent[i];
		EnemyBulletWorldTrans[i].TransferMatrix();
	}
}

void EnemyBullet::BulletAliveTimerUpdate()
{
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		if (isBulletAlive[i] == true) {
			if (BulletLifeTime[i] > 0) {
				BulletLifeTime[i]--;
			}
		}
	}
	if (BulletCoolTime > 0) {
		BulletCoolTime--;
	}
}

void EnemyBullet::CheckBulletAlive()
{
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		if (BulletLifeTime[i] <= 0) {
			isBulletAlive[i] = false;
			BulletVector[ i ] = Vec3Number(fNumbers::fZero);
		}
	}
}

void EnemyBullet::SetNotAlivePosition()
{
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		if (isBulletAlive[i] == false) {
			EnemyBulletWorldTrans[i].translation_ = Vector3(FloatNumber(fNumbers::fZero), -50,FloatNumber(fNumbers::fZero));
			EnemyBulletWorldTrans[i].TransferMatrix();
		}
	}
}


void EnemyBullet::OldPosUpdate()
{
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		BulletOldPos[i] = EnemyBulletWorldTrans[i].translation_;
	}
}
