#include "MissileBullet.h"
#include "Easing.h"
#include "SphereCollider.h"
#include <CollisionAttribute.h>

MissileBullet::MissileBullet()
{
	model_.reset(Model::CreateFromOBJ("sphereNormalEnemy", true));
	for (int i = 0; i < AllBulletCount; i++) {
		EnemyBulletWorldTrans[i].scale_ = Vector3(0.5f, 0.5f, 0.5f);
		EnemyBulletWorldTrans[i].Initialize();
		isNearPlayer[i] = false;
		BulletSpeed[i] = 1.5f;
		BulletRadius[ i ] = 0.5f;
		// コリジョンマネージャに追加
		BulletCollider[ i ] = new SphereCollider(Vector4(0,BulletRadius[ i ],0,0),BulletRadius[ i ]);
		CollisionManager::GetInstance()->AddCollider(BulletCollider[ i ]);
		BulletCollider[ i ]->SetAttribute(COLLISION_ATTR_NOTATTACK);
	}
	input_ = Input::GetInstance();
}

MissileBullet::~MissileBullet()
{
	for ( auto&& bulletCollider : BulletCollider )
	{
		CollisionManager::GetInstance()->RemoveCollider(bulletCollider);
	}
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

				if ( BulletCollider[ i ]->GetHit() )
				{
					isBulletAlive[ i ] = false;
					BulletCollider[ i ]->SetAttribute(COLLISION_ATTR_NOTATTACK);
				}
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
				if ( BulletCollider[ i ]->GetHit() )
				{
					isBulletAlive[ i ] = false;
					BulletCollider[ i ]->Reset();
					BulletCollider[ i ]->SetAttribute(COLLISION_ATTR_NOTATTACK);
				}
			}
		}
		EnemyBulletWorldTrans[ i ].TransferMatrix();
		BulletCollider[ i ]->Update(EnemyBulletWorldTrans[ i ].matWorld_,BulletRadius[ i ],BulletSpeed[ i ],BulletVelocity[ i ]);
	}
}

void MissileBullet::Draw(const ViewProjection& viewProjection_)
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
			BulletLifeTime[BulletCounter] = MaxBulletLifeTime;
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
			BulletCollider[ BulletCounter ]->SetAttribute(COLLISION_ATTR_ENEMY_BULLET_ATTACK);
			BulletLerpTime++;
			makeBulletCount++;
		}
		BulletCounter++;
	}
}

void MissileBullet::MakeSelectMissileBullet(Vector3& pos,Vector3& upLeft,Vector3& upRight,Vector3& downLeft,Vector3& downRight,uint32_t& MakeCount)
{
	uint32_t changeCount = MakeCount / 3;
	uint32_t MakeCountMissile = changeCount * 3 + 1;
	float lerpPos = 1.0f / static_cast< float >( changeCount );

	uint32_t BulletCounter = 0;
	uint32_t BulletLerpTime = 1;
	uint32_t BulletLerpRightTime = 0;
	makeBulletCount = 0;
	while ( makeBulletCount < MakeCountMissile )
	{
		if ( isBulletAlive[ BulletCounter ] == false )
		{
			BulletNotTrackingTime[ BulletCounter ] = 30;
			BulletLifeTime[ BulletCounter ] = MaxBulletLifeTime;
			isBulletAlive[ BulletCounter ] = true;
			isStartTracking[ BulletCounter ] = false;
			isNearPlayer[ BulletCounter ] = false;
			EnemyBulletWorldTrans[ BulletCounter ].translation_ = pos;

			BulletSpeed[ BulletCounter ] = 1.0f;
			BulletStartSpeed = 0.8f;

			BulletEasingTime[ BulletCounter ] = 0;
			BulletMaxEasingTime[ BulletCounter ] = 80;

			BulletLarpEasingTime[ BulletCounter ] = 0;
			BulletLarpMaxEasingTime[ BulletCounter ] = 60;
			BulletLerpSpeed[ BulletCounter ] = BulletStartLerpTime;


			if ( makeBulletCount < changeCount )
			{
				BulletVelocity[ BulletCounter ] = MyMath::lerp(upLeft.norm(),upRight.norm(),lerpPos * BulletLerpTime).norm();
			}
			else if(makeBulletCount < changeCount * 2 + 1)
			{
				BulletLerpRightTime = BulletLerpTime - (changeCount) - 1;
				BulletVelocity[ BulletCounter ] = MyMath::lerp(upLeft.norm(),downLeft.norm(),lerpPos * BulletLerpRightTime).norm();
			}
			else
			{
				BulletLerpRightTime = BulletLerpTime - (changeCount * 2 + 1);
				BulletVelocity[ BulletCounter ] = MyMath::lerp(upRight.norm(),downRight.norm(),lerpPos * BulletLerpRightTime).norm();
			}
			BulletCollider[ BulletCounter ]->SetAttribute(COLLISION_ATTR_ENEMY_BULLET_ATTACK);
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
				BulletCollider[ i ]->SetAttribute(COLLISION_ATTR_NOTATTACK);
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
			BulletLifeTime[i] = MaxBulletLifeTime;
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
