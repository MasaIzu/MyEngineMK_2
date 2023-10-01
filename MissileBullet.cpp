#include "MissileBullet.h"

MissileBullet::MissileBullet()
{
	model_.reset(Model::CreateFromOBJ("sphereNormalEnemy", true));
	for (int i = 0; i < AllBulletCount; i++) {
		EnemyBulletWorldTrans[i].scale_ = Vector3(0.5f, 0.5f, 0.5f);
		EnemyBulletWorldTrans[i].Initialize();
	}
}

MissileBullet::~MissileBullet()
{
}

void MissileBullet::Initialize()
{
}

void MissileBullet::Update(const Vector3& EndPos)
{
	CheckBulletAlive();

	for (int i = 0; i < AllBulletCount; i++) {
		if (isBulletAlive[i] == true) {
			if (isStartTracking[i] == false) {
				EnemyBulletWorldTrans[i].translation_ += EnemyBulletMoveMent[i];
				BulletTrackingStartPos[i] = EnemyBulletWorldTrans[i].translation_;
			}
			else {
				EnemyBulletWorldTrans[i].translation_ = MyMath::lerp(EnemyBulletWorldTrans[i].translation_, EndPos, 0.05f);
			}
		}
	}

	WorldTransUpdate();
}

void MissileBullet::Draw(ViewProjection& viewProjection_)
{
	for (int i = 0; i < AllBulletCount; i++) {
		if (isBulletAlive[i] == true) {
			model_->Draw(EnemyBulletWorldTrans[i], viewProjection_);
		}
	}
}

void MissileBullet::WorldTransUpdate()
{
	for (int i = 0; i < AllBulletCount; i++) {
		EnemyBulletWorldTrans[i].TransferMatrix();
	}
}

void MissileBullet::CheckBulletAlive()
{
	for (int i = 0; i < AllBulletCount; i++) {
		if (isBulletAlive[i] == true) {
			if (BulletNotTrackingTime[i] > 0) {
				BulletNotTrackingTime[i]--;
			}
			else {
				isStartTracking[i] = true;
			}
			if (BulletLifeTime[i] > 0) {
				BulletLifeTime[i]--;
			}
			else {
				isBulletAlive[i] = false;
			}
		}
	}
}

void MissileBullet::SetNotAlivePosition()
{
}

void MissileBullet::MakeBullet(Vector3& pos)
{
	for (int i = 0; i < AllBulletCount; i++) {
		if (isBulletAlive[i] == false) {
			BulletNotTrackingTime[i] = 60;
			BulletLifeTime[i] = 500;
			isBulletAlive[i] = true;
			isStartTracking[i] = false;
			EnemyBulletWorldTrans[i].translation_ = pos;
			EnemyBulletMoveMent[i] = MyMath::RandomCenterVec3(Uint32Vector2(0, 20), Uint32Vector2(0, 0), Uint32Vector2(0, 20)) / 10;
			EnemyBulletMoveMent[i].y = 3;
		}
	}
}
