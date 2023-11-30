#include "NormalGun.h"
#include <Numbers.h>
#include <FbxLoader.h>
#include "ExplosionGun.h"

NormalGun::NormalGun(const unsigned short Attribute)
{
	for ( auto&& Bullet : explosionBullet )
	{
		Bullet = std::make_unique<ExplosionBullet>(Attribute);
	}
}

NormalGun::~NormalGun()
{
}

void NormalGun::Initialize(const Vector3& Pos,Model* BulletModel)
{
	model_.reset(Model::CreateFromOBJ("NormalGun",true));

	fbxModel_.reset(FbxLoader::GetInstance()->LoadModelFromFile("NormalGunFbx",true));
	fbxObj3d_ = FBXObject3d::Create();
	fbxObj3d_->SetModel(fbxModel_.get());
	fbxObj3d_->Update();

	GunTrans.Initialize();
	GunTrans.translation_ = Pos;
	UpdatePosition();

	for ( auto&& Bullet : explosionBullet )
	{
		Bullet->Initialize(BulletModel);
	}
}

void NormalGun::Update(const Vector3& Pos,const Vector3& rot)
{
	GunTrans.translation_ = Pos;
	GunTrans.SetRot(rot);
	UpdatePosition();
	fbxObj3d_->Update();
	TimeUpdate();

	for ( auto&& Bullet : explosionBullet )
	{
		Bullet->Update();
	}
}

void NormalGun::Draw(const ViewProjection& viewProjection_)
{
	//model_->Draw(GunTrans,viewProjection_);
	for ( auto&& Bullet : explosionBullet )
	{
		Bullet->Draw(viewProjection_);
	}
}

void NormalGun::FbxDraw(const ViewProjection& viewProjection_)
{
	fbxObj3d_->Draw(GunTrans,viewProjection_);
}

void NormalGun::ShotBullet(const Vector3& BulletVec)
{
	if ( CoolTime == static_cast< uint32_t >( Numbers::Zero ) )
	{
		for ( auto&& Bullet : explosionBullet )
		{
			if ( !Bullet->GetBulletAlive() )
			{
				CoolTime = MaxCoolTime;
				Vector3 shootVec = BulletVec - MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(0) * GunTrans.matWorld_);
				Bullet->MakeBullet(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(0) * GunTrans.matWorld_),shootVec.norm(),BulletSpeed);
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

WorldTarnsLook NormalGun::GetLook() const
{
	return GunTrans.LookVelocity;
}

void NormalGun::CSUpdate(ID3D12GraphicsCommandList* commandList)
{
	for ( auto&& Bullet : explosionBullet )
	{
		Bullet->CSUpadate(commandList);
	}
}

void NormalGun::ParticleDraw(const ViewProjection& viewProjection_)
{
	for ( auto&& Bullet : explosionBullet )
	{
		Bullet->Draw(viewProjection_);
	}
}
