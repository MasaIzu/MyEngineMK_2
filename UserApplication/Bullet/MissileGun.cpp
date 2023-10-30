#include "MissileGun.h"

MissileGun::MissileGun(const unsigned short Attribute)
{
	for ( auto&& Bullet : normalBullet )
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

	for ( auto&& Bullet : normalBullet )
	{
		Bullet->Initialize(BulletModel);
	}
}

void MissileGun::Update(const Vector3& GunPos,const Vector3& inductionPos)
{
	GunTrans.translation_ = GunPos;
	UpdatePosition();

	TimeUpdate();

	for ( auto&& Bullet : normalBullet )
	{
		Bullet->Update(inductionPos);
	}
}

void MissileGun::Draw(const ViewProjection& viewProjection_)
{
	model_->Draw(GunTrans,viewProjection_);
	for ( auto&& Bullet : normalBullet )
	{
		Bullet->Draw(viewProjection_);
	}
}

void MissileGun::ShotBullet()
{
	if ( CoolTime == 0 )
	{
		float lerpPos = 1.0f / static_cast< float >( MissileBulletCountHalf );
		NowMissileBulletCount = 0;
		for ( uint32_t i = 0; i < MissileBulletCount; i++ )
		{
			for ( auto&& Bullet : normalBullet )
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
						uint32_t CountReset = NowMissileBulletCount - MissileBulletCountHalf + 1;
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
	if ( CoolTime > 0 )
	{
		CoolTime--;
	}
}
