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

void MissileGun::ShotBullet(const Vector3& BulletVec)
{
	if ( CoolTime == 0 )
	{
		for ( auto&& Bullet : normalBullet )
		{
			if ( !Bullet->GetBulletAlive() )
			{
				CoolTime = MaxCoolTime;
				Vector3 shootVec = BulletVec - GunTrans.translation_;
				Bullet->MakeMissileBullet(GunTrans.translation_,shootVec.norm(),BulletSpeed);
				break;
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
