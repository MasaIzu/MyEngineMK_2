#include "MissileGun.h"
#include <Numbers.h>

MissileGun::MissileGun(const unsigned short Attribute)
{
	for ( auto&& Bullet : missileBullet )
	{
		Bullet = std::make_unique<MissileBullet>(Attribute);
	}
}

MissileGun::~MissileGun()
{
}

void MissileGun::Initialize(const Vector3& Pos,Model* GunModel,Model* BulletModel)
{
	model_ = GunModel;

	GunTrans.Initialize();
	GunTrans.translation_ = Pos;
	UpdatePosition();

	for ( auto&& Bullet : missileBullet )
	{
		Bullet->Initialize(BulletModel);
	}
}

void MissileGun::Update(const Vector3& GunPos,const Vector3& inductionPos,const Vector3& rot)
{
	GunTrans.translation_ = GunPos;
	GunTrans.SetRot(rot);
	UpdatePosition();

	TimeUpdate();

	for ( auto&& Bullet : missileBullet )
	{
		Bullet->Update(inductionPos);
	}
}

void MissileGun::Draw(const ViewProjection& viewProjection_)
{
	//model_->Draw(GunTrans,viewProjection_);
	for ( auto&& Bullet : missileBullet )
	{
		Bullet->Draw(viewProjection_);
	}
}

void MissileGun::ShotBullet()
{
	if ( CoolTime == static_cast< uint32_t >( Numbers::Zero ) )
	{
		float lerpPos = FloatNumber(fNumbers::fOnePointZero) / static_cast< float >( MissileBulletCountHalf );
		NowMissileBulletCount = static_cast< uint32_t >( Numbers::Zero );
		for ( uint32_t i = static_cast< uint32_t >( Numbers::Zero ); i < MissileBulletCount; i++ )
		{
			for ( auto&& Bullet : missileBullet )
			{
				if ( !Bullet->GetBulletAlive() )
				{
					CoolTime = MaxCoolTime;

					if ( NowMissileBulletCount < MissileBulletCountHalf )
					{
						BulletVelocity = MyMath::lerp(GunTrans.LookVelocity.lookLeft.norm(),GunTrans.LookVelocity.lookUp.norm(),lerpPos * static_cast<float>(NowMissileBulletCount)).norm();
					}
					else
					{
						uint32_t CountReset = NowMissileBulletCount - MissileBulletCountHalf + static_cast< uint32_t >( Numbers::One );
						BulletVelocity = MyMath::lerp(GunTrans.LookVelocity.lookUp.norm(),GunTrans.LookVelocity.lookRight.norm(),lerpPos * static_cast<float>( CountReset )).norm();
					}
					Bullet->MakeMissileBullet(GunTrans.translation_,BulletVelocity.norm(),BulletSpeed);
					NowMissileBulletCount++;
					break;
				}
			}
		}
	}
}

void MissileGun::UpdatePosition()
{
	GunTrans.TransferMatrix();
}

void MissileGun::TimeUpdate()
{
	if ( CoolTime > static_cast< uint32_t >( Numbers::Zero ) )
	{
		CoolTime--;
	}
}
