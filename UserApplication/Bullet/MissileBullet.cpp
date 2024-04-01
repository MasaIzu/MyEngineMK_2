#include "MissileBullet.h"
#include "Easing.h"
#include "SphereCollider.h"
#include <CollisionAttribute.h>
#include <Numbers.h>

MissileBullet::MissileBullet(const unsigned short Attribute_)
{
	BulletWorldTrans.scale_ = Vector3(BulletRadius,BulletRadius,BulletRadius);
	BulletWorldTrans.Initialize();
	isNearPlayer = false;
	// コリジョンマネージャに追加
	float sphereF = FloatNumber(fNumbers::fZero);
	BulletCollider = new SphereCollider(Vector4(sphereF,BulletRadius,sphereF,sphereF),BulletRadius);
	CollisionManager::GetInstance()->AddCollider(BulletCollider);
	BulletCollider->SetAttribute(COLLISION_ATTR_NOTATTACK);
	Attribute = Attribute_;

	particleKisekiParticle = std::make_unique<ParticleEditor>();
	particleKisekiParticle->Initialize(makeBulletParticleCount,true,"KisekiEnemy");
	particleKisekiParticle->SetTextureHandle(TextureManager::Load("sprite/effect4.png"));
}

MissileBullet::~MissileBullet()
{

	CollisionManager::GetInstance()->RemoveCollider(BulletCollider);

}

void MissileBullet::Initialize(Model* BulletModel)
{
	model_ = BulletModel;
}

void MissileBullet::Update(const Vector3& EndPos)
{
	CheckBulletAlive();


	if ( isBulletAlive == true )
	{
		if ( BulletCollider->GetSphereMeshHit() )
		{
			isBulletAlive = false;
			BulletCollider->SetAttribute(COLLISION_ATTR_NOTATTACK);
			BulletCollider->SphereMeshHitReset();
		}
		if ( isStartTracking == false )
		{

			Vector3 goPos = EndPos - BulletWorldTrans.translation_;
			goPos.normalize();
			BulletVelocity = MyMath::lerp(BulletVelocity,goPos,BulletStartLerpTime);
			BulletWorldTrans.translation_ += BulletVelocity.norm() * BulletStartSpeed;

			if ( BulletCollider->GetHit() )
			{
				isBulletAlive = false;
				BulletCollider->SetAttribute(COLLISION_ATTR_NOTATTACK);
			}
		}
		else
		{
			if ( isNearPlayer == false )
			{
				Vector3 goPos = EndPos - BulletWorldTrans.translation_;
				goPos.normalize();
				BulletSpeed = Easing::easeOutCubic(BulletStartSpeed,BulletEndSpeed,BulletEasingTime,BulletMaxEasingTime);
				BulletLerpSpeed = Easing::easeOutCubic(BulletLerpStartSpeed,BulletLrrpEndSpeed,BulletLarpEasingTime,BulletLarpMaxEasingTime);
				BulletVelocity = MyMath::lerp(BulletVelocity,goPos,BulletLerpSpeed);
				BulletWorldTrans.translation_ += BulletVelocity.norm() * BulletSpeed;

				// 中心点の距離の２乗 <= 半径の和の２乗　なら交差
				Vector3 tmp;
				tmp = EndPos - BulletWorldTrans.translation_;
				float dist = tmp.dot(tmp);
				float radius2 = NotSearchArea;
				radius2 *= radius2;

				if ( dist <= radius2 )
				{
					isNearPlayer = true;
				}
			}
			else
			{
				Vector3 goPos = EndPos - BulletWorldTrans.translation_;
				goPos.normalize();
				BulletVelocity = MyMath::lerp(BulletVelocity.norm(),goPos,LerpStrength);

				BulletWorldTrans.translation_ += BulletVelocity.norm() * BulletSpeed;
			}
			if ( BulletCollider->GetHit() )
			{
				isBulletAlive = false;
				BulletCollider->Reset();
				BulletCollider->SetAttribute(COLLISION_ATTR_NOTATTACK);
			}
		}
	}
	BulletWorldTrans.TransferMatrix();
	BulletCollider->Update(BulletWorldTrans.matWorld_,BulletRadius,BulletSpeed,BulletVelocity);

}

void MissileBullet::CSUpadate(ID3D12GraphicsCommandList* commandList)
{
	particleKisekiParticle->CSUpdate(commandList,MyMath::Vec3ToVec4(BulletWorldTrans.translation_),isBulletAlive);
}

void MissileBullet::ParticleDraw(const ViewProjection& viewProjection_)
{
	particleKisekiParticle->Draw(viewProjection_);
}

void MissileBullet::Draw(const ViewProjection& viewProjection_,const ViewProjection& LightViewProjection_)
{
	if ( isBulletAlive == true )
	{
		model_->Draw(BulletWorldTrans,viewProjection_,LightViewProjection_);
	}
}

void MissileBullet::MakeMissileBullet(const Vector3& pos,const Vector3& velocity,const float& speed)
{
	if ( isBulletAlive == false )
	{
		isBulletAlive = true;
		isStartTracking = false;
		isNearPlayer = false;

		BulletNotTrackingTime = MaxBulletNotTrackingTime;
		BulletEasingTime = 0;
		BulletLarpEasingTime = 0;
		BulletLifeTime = MaxBulletLifeTime;

		BulletStartSpeed = speed;

		BulletWorldTrans.translation_ = pos;
		BulletVelocity = velocity.norm();
		BulletCollider->SetAttribute(COLLISION_ATTR_ENEMY_BULLET_ATTACK);
		BulletCollider->SphereMeshHitReset();
	}
}

void MissileBullet::WorldTransUpdate()
{
	BulletWorldTrans.TransferMatrix();
}

void MissileBullet::CheckBulletAlive()
{

	if ( isBulletAlive == true )
	{
		if ( BulletNotTrackingTime > 0 )
		{
			BulletNotTrackingTime--;
		}
		else
		{
			isStartTracking = true;
		}
		if ( BulletLifeTime > 0 )
		{
			BulletLifeTime--;
		}
		else
		{
			BulletCollider->SetAttribute(COLLISION_ATTR_NOTATTACK);
			isBulletAlive = false;
		}
		if ( isStartTracking == true )
		{
			if ( BulletEasingTime < BulletMaxEasingTime )
			{
				BulletEasingTime++;
			}
			if ( BulletLarpEasingTime < BulletLarpMaxEasingTime )
			{
				BulletLarpEasingTime++;
			}
		}
	}
}

bool MissileBullet::GetBulletAlive() const {
	return isBulletAlive;
}