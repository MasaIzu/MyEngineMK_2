#include "NormalGun.h"
#include <Numbers.h>

NormalGun::NormalGun(const unsigned short Attribute)
{
	for ( auto&& Bullet : normalBullet )
	{
		Bullet = std::make_unique<NormalBullet>(Attribute);
	}
}

NormalGun::~NormalGun()
{
}

void NormalGun::Initialize(const Vector3& Pos,Model* BulletModel)
{
	model_.reset(Model::CreateFromOBJ("NormalGun",true));

	GunTrans.Initialize();
	GunTrans.translation_ = Pos;
	UpdatePosition();

	for ( auto&& Bullet : normalBullet )
	{
		Bullet->Initialize(BulletModel);
	}
}

void NormalGun::Update(const Vector3& Pos,const Vector3& rot)
{
	GunTrans.translation_ = Pos;
	GunTrans.SetRot(rot);
	UpdatePosition();

	TimeUpdate();

	for ( auto&& Bullet : normalBullet )
	{
		Bullet->Update();
	}
}

void NormalGun::Draw(const ViewProjection& viewProjection_)
{
	model_->Draw(GunTrans,viewProjection_);
	for ( auto&& Bullet : normalBullet )
	{
		Bullet->Draw(viewProjection_);
	}
}

void NormalGun::ShotBullet(const Vector3& BulletVec)
{
	if ( CoolTime == static_cast< uint32_t >( Numbers::Zero ) )
	{
		for ( auto&& Bullet : normalBullet )
		{
			if ( !Bullet->GetBulletAlive() )
			{
				CoolTime = MaxCoolTime;
				Vector3 shootVec = BulletVec - GunTrans.translation_;
				Bullet->MakeBullet(GunTrans.translation_,shootVec.norm(),BulletSpeed);
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
	if ( CoolTime > static_cast< uint32_t >( Numbers::Zero ) )
	{
		CoolTime--;
	}
}
