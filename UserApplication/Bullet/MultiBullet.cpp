#include "MultiBullet.h"

MultiBullet::MultiBullet()
{
	model_.reset(Model::CreateFromOBJ("sphereNormalEnemy", true));
	for (int i = 0; i < AllBulletCount; i++) {
		EnemyBulletWorldTrans[i].scale_ = Vector3(0.5f, 0.5f, 0.5f);
		EnemyBulletWorldTrans[ i ].Initialize();
		BulletCollider[ i ] = nullptr;
		BulletLifeTime[ i ] = 0;
		BulletRadius[ i ] = 0;
		EnemyBulletSpeed[ i ] = 0;
		isBulletAlive[ i ] = false;
	}
}

MultiBullet::~MultiBullet()
{
}

void MultiBullet::Initialize()
{

}

void MultiBullet::Update()
{
	CheckBulletAlive();

	for (int i = 0; i < AllBulletCount; i++) {
		if (isBulletAlive[i] == true) {
			EnemyBulletWorldTrans[i].translation_ += EnemyBulletMoveMent[i];
		}
	}

	WorldTransUpdate();
}

void MultiBullet::Draw(ViewProjection& viewProjection_)
{
	for (int i = 0; i < AllBulletCount; i++) {
		if (isBulletAlive[i] == true) {
			model_->Draw(EnemyBulletWorldTrans[i], viewProjection_);
		}
	}
}

void MultiBullet::WorldTransUpdate()
{
	for (int i = 0; i < AllBulletCount; i++) {
		EnemyBulletWorldTrans[i].TransferMatrix();
	}
}

void MultiBullet::CheckBulletAlive()
{
	for (int i = 0; i < AllBulletCount; i++) {
		if (isBulletAlive[i] == true) {
			if (BulletLifeTime[i] > 0) {
				BulletLifeTime[i]--;
			}
			else {
				isBulletAlive[i] = false;
			}
		}
	}
}

void MultiBullet::SetNotAlivePosition()
{
}

void MultiBullet::MakeBullet(Vector3& pos, Vector3& BulletVelocity, const float& BulletSpeed)
{
	makeBulletCount = 0;
	for (int i = 0; i < AllBulletCount; i++) {
		if (isBulletAlive[i] == false) {
			if (makeBulletCount < 5) {
				BulletLifeTime[i] = 100;
				isBulletAlive[i] = true;
				EnemyBulletWorldTrans[i].translation_ = pos;
				EnemyBulletMoveMent[i] = (BulletVelocity + (MyMath::RandomCenterVec3(0, 20) / 1000)) * BulletSpeed;
			}
			makeBulletCount++;
		}
	}
}
