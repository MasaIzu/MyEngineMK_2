#include "NormalGun.h"

NormalGun::NormalGun()
{
}

NormalGun::~NormalGun()
{
}

void NormalGun::Initialize(const Vector3& Pos,Model* GunModel,Model* BulletModel)
{
	model_ = GunModel;

	GunTrans.Initialize();
	GunTrans.translation_ = Pos;
	UpdatePosition();

	for ( auto&& Bullet : normalBullet )
	{
		Bullet.Initialize(BulletModel);
	}
}

void NormalGun::Update(const Vector3& Pos)
{
	GunTrans.translation_ = Pos;
	UpdatePosition();

	TimeUpdate();

	for ( auto&& Bullet : normalBullet )
	{
		Bullet.Update();
	}
}

void NormalGun::Draw(const ViewProjection& viewProjection_)
{
	model_->Draw(GunTrans,viewProjection_);
	for ( auto&& Bullet : normalBullet )
	{
		Bullet.Draw(viewProjection_);
	}
}

void NormalGun::ShotBullet(const Vector3& BulletVec)
{
	if ( CoolTime == 0 )
	{
		for ( auto&& Bullet : normalBullet )
		{
			if ( !Bullet.GetBulletAlive() )
			{
				CoolTime = MaxCoolTime;
				Vector3 shootVec = BulletVec - GunTrans.translation_;
				Bullet.MakePlayerBullet(GunTrans.translation_,shootVec.norm());
				break;
			}
		}
	}
}

void NormalGun::UpdatePosition()
{
	GunTrans.TransferMatrix();
}

void NormalGun::TimeUpdate()
{
	if ( CoolTime > 0 )
	{
		CoolTime--;
	}
}
