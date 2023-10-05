#include "MissileBullet.h"
#include "Easing.h"

MissileBullet::MissileBullet()
{
	model_.reset(Model::CreateFromOBJ("sphereNormalEnemy", true));
	for (int i = 0; i < AllBulletCount; i++) {
		EnemyBulletWorldTrans[i].scale_ = Vector3(0.5f, 0.5f, 0.5f);
		EnemyBulletWorldTrans[i].Initialize();
		isNearPlayer[i] = false;
		BulletSpeed[i] = 1.5f;
	}
	input_ = Input::GetInstance();
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

				Vector3 goPos = EndPos - EnemyBulletWorldTrans[i].translation_;
				goPos.normalize();
				BulletVelocity[i] = MyMath::lerp(BulletVelocity[i], goPos, BulletStartLerpTime);
				EnemyBulletWorldTrans[i].translation_ += BulletVelocity[i].norm() * BulletSpeed[i];
			}
			else {

				if (isNearPlayer[i] == false) {
					Vector3 goPos = EndPos - EnemyBulletWorldTrans[i].translation_;
					goPos.normalize();
					BulletSpeed[i] = Easing::easeOutCubic(BulletStartSpeed, BulletEndSpeed, BulletEasingTime[i], BulletMaxEasingTime[i]);
					BulletLerpSpeed[i] = Easing::easeOutCubic(BulletLerpStartSpeed, BulletLrrpEndSpeed, BulletLarpEasingTime[i], BulletLarpMaxEasingTime[i]);
					BulletVelocity[i] = MyMath::lerp(BulletVelocity[i], goPos, BulletLerpSpeed[i]);
					EnemyBulletWorldTrans[i].translation_ += BulletVelocity[i].norm() * BulletSpeed[i];

					// 中心点の距離の２乗 <= 半径の和の２乗　なら交差
					Vector3 tmp;
					tmp = EndPos - EnemyBulletWorldTrans[i].translation_;
					float dist = tmp.dot(tmp);
					float radius2 = 7.0f + 1.0f;
					radius2 *= radius2;

					if (dist <= radius2)
					{
						isNearPlayer[i] = true;
					}
				}
				else {
					Vector3 goPos = EndPos - EnemyBulletWorldTrans[i].translation_;
					goPos.normalize();
					BulletVelocity[i] = MyMath::lerp(BulletVelocity[i].norm(), goPos, 0.005f);

					EnemyBulletWorldTrans[i].translation_ += BulletVelocity[i].norm() * BulletSpeed[i];
				}
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

void MissileBullet::MakeSelectMissileBullet(Vector3& pos, Vector3& left, Vector3& top, Vector3& right, uint32_t& MakeCount)
{
	uint32_t harfCount = MakeCount / 2;

	float lerpPos = 1.0f / static_cast<float>(harfCount);

	uint32_t BulletCounter = 0;
	uint32_t BulletLerpTime = 0;
	uint32_t BulletLerpRightTime = 0;
	makeBulletCount = 0;
	while (makeBulletCount < MakeCount)
	{
		if (isBulletAlive[BulletCounter] == false) {
			BulletNotTrackingTime[BulletCounter] = 30;
			BulletLifeTime[BulletCounter] = 500;
			isBulletAlive[BulletCounter] = true;
			isStartTracking[BulletCounter] = false;
			isNearPlayer[BulletCounter] = false;
			EnemyBulletWorldTrans[BulletCounter].translation_ = pos;

			BulletSpeed[BulletCounter] = 1.0f;
			BulletStartSpeed = 0.8f;

			BulletEasingTime[BulletCounter] = 0;
			BulletMaxEasingTime[BulletCounter] = 80;

			BulletLarpEasingTime[BulletCounter] = 0;
			BulletLarpMaxEasingTime[BulletCounter] = 60;
			BulletLerpSpeed[BulletCounter] = BulletStartLerpTime;


			if (makeBulletCount < harfCount) {
				BulletVelocity[BulletCounter] = MyMath::lerp(left, top, lerpPos * BulletLerpTime).norm();
			}
			else {
				BulletLerpRightTime = BulletLerpTime - harfCount + 1;
				BulletVelocity[BulletCounter] = MyMath::lerp(top, right, lerpPos * BulletLerpRightTime).norm();
			}

			BulletLerpTime++;
			makeBulletCount++;
		}
		BulletCounter++;
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
			if (isStartTracking[i] == true) {
				if (BulletEasingTime[i] < BulletMaxEasingTime[i]) {
					BulletEasingTime[i]++;
				}
				if (BulletLarpEasingTime[i] < BulletLarpMaxEasingTime[i]) {
					BulletLarpEasingTime[i]++;
				}
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
			BulletNotTrackingTime[i] = 0;
			BulletLifeTime[i] = 500;
			isBulletAlive[i] = true;
			isStartTracking[i] = false;
			isNearPlayer[i] = false;
			EnemyBulletWorldTrans[i].translation_ = pos;

			BulletVelocity[i] = Vector3(0, 0, 0);
			BulletVelocity[i].y = 3.0f;

			break;
		}
	}
}
