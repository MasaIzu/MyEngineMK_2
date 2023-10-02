#include "MissileBullet.h"

MissileBullet::MissileBullet()
{
	model_.reset(Model::CreateFromOBJ("sphereNormalEnemy", true));
	for (int i = 0; i < AllBulletCount; i++) {
		EnemyBulletWorldTrans[i].scale_ = Vector3(0.5f, 0.5f, 0.5f);
		EnemyBulletWorldTrans[i].Initialize();
		isNearPlayer[i] = false;
	}
	BulletSpeed = 3.0f;
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
				EnemyBulletWorldTrans[i].translation_ += BulletVelocity[i] * BulletSpeed;

				BulletOldPos[i] = EnemyBulletWorldTrans[i].translation_;
			}
			else {

				if (isNearPlayer[i] == false) {
					Vector3 goPos = EndPos - EnemyBulletWorldTrans[i].translation_;
					goPos.normalize();

					BulletVelocity[i] = MyMath::lerp(BulletVelocity[i], goPos, 0.1f);
					float VelocityY = BulletVelocity[i].y;
					BulletVelocity[i] = MyMath::lerp(BulletVelocity[i], goPos, 1.0f);
					BulletVelocity[i].y = VelocityY;

					EnemyBulletWorldTrans[i].translation_ += BulletVelocity[i].norm() * BulletSpeed;

					// ’†S“_‚Ì‹——£‚Ì‚Qæ <= ”¼Œa‚Ì˜a‚Ì‚Qæ@‚È‚çŒð·
					Vector3 tmp;
					tmp = EndPos - EnemyBulletWorldTrans[i].translation_;
					float dist = tmp.dot(tmp);
					float radius2 = 20.0f + 1.0f;
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

					EnemyBulletWorldTrans[i].translation_ += BulletVelocity[i].norm() * BulletSpeed;;
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
