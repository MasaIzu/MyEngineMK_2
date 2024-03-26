#include "ExplosionGun.h"
#include <Numbers.h>
#include <FbxLoader.h>
#include "ExplosionGun.h"

ExplosionGun::ExplosionGun(const unsigned short Attribute)
{
	for ( auto&& Bullet : explosionBullet )
	{
		Bullet = std::make_unique<ExplosionBullet>(Attribute);
	}
}

ExplosionGun::~ExplosionGun()
{
}

void ExplosionGun::Initialize(const Vector3& Pos,Model* BulletModel,AudioManager* audioManager_,const float& soundVol)
{
	fbxModel_.reset(FbxLoader::GetInstance()->LoadModelFromFile("NormalGunFbx",true));
	fbxObj3d_ = FBXObject3d::Create();
	fbxObj3d_->SetModel(fbxModel_.get());
	fbxObj3d_->Update();

	GunTrans.Initialize();
	GunTrans.translation_ = Pos;
	UpdatePosition();

	audioManager = audioManager_;

	ExSoundSize = soundVol;
	ExNum = audioManager->LoadAudio("Resources/Sound/launcher1.mp3",ExSoundSize);
	ExBaNum = audioManager->LoadAudio("Resources/Sound/small_explosion3.mp3",ExBaSoundSize);
	for ( auto&& Bullet : explosionBullet )
	{
		Bullet->Initialize(BulletModel);
	}
}

void ExplosionGun::Update(const Vector3& Pos,const Vector3& rot)
{
	GunTrans.translation_ = Pos;
	GunTrans.SetRot(rot);
	UpdatePosition();
	fbxObj3d_->Update();
	TimeUpdate();

	if ( CoolTime == static_cast< uint32_t >( Numbers::Zero ) )
	{
		isReload = false;
	}

	for ( auto&& Bullet : explosionBullet )
	{
		Bullet->Update();
	}

	for ( auto&& Bullet : explosionBullet )
	{
		if ( Bullet->GetBulletNotAlive() )
		{
			audioManager->PlayWave(ExBaNum,false);
		}
	}
}

void ExplosionGun::Draw(const ViewProjection& viewProjection_,const ViewProjection& LightViewProjection_)
{
	for ( auto&& Bullet : explosionBullet )
	{
		Bullet->Draw(viewProjection_,LightViewProjection_);
	}
}

void ExplosionGun::FbxDraw(const ViewProjection& viewProjection_,const ViewProjection& lightViewProjection_)
{
	fbxObj3d_->Draw(GunTrans,viewProjection_,lightViewProjection_);
}

void ExplosionGun::FbxShadowDraw(const ViewProjection& lightViewProjection_)
{
	fbxObj3d_->ShadowDraw(GunTrans,lightViewProjection_);
}

void ExplosionGun::ShotBullet(const Vector3& BulletVec)
{
	if ( CoolTime == static_cast< uint32_t >( Numbers::Zero ) )
	{
		for ( auto&& Bullet : explosionBullet )
		{
			if ( !Bullet->GetBulletAlive() )
			{
				CoolTime = MaxCoolTime;
				UseBulletCount++;
				Vector3 shootVec = BulletVec - MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(0) * GunTrans.matWorld_);
				Bullet->MakeBullet(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(0) * GunTrans.matWorld_),shootVec.norm(),BulletSpeed);
				audioManager->PlayWave(ExNum,false);
				if ( UseBulletCount >= BulletMaxCount )
				{
					isReload = true;
					UseBulletCount = 0;
					CoolTime = MaxReloadCoolTime;
				}
				break;
			}
		}
	}
}

void ExplosionGun::UpdatePosition()
{
	GunTrans.TransferMatrix();
}

void ExplosionGun::TimeUpdate()
{
	if ( CoolTime > static_cast< uint32_t >( Numbers::Zero ) )
	{
		CoolTime--;
	}
}

WorldTarnsLook ExplosionGun::GetLook() const
{
	return GunTrans.LookVelocity;
}

void ExplosionGun::CSUpdate(ID3D12GraphicsCommandList* commandList)
{
	for ( auto&& Bullet : explosionBullet )
	{
		Bullet->CSUpadate(commandList);
	}
}

void ExplosionGun::ParticleDraw(const ViewProjection& viewProjection_)
{
	for ( auto&& Bullet : explosionBullet )
	{
		Bullet->ParticleDraw(viewProjection_);
	}
}

bool ExplosionGun::GetIsReload()
{
	return isReload;
}

uint32_t ExplosionGun::GetBulletCount()
{
	uint32_t ExpendedBullets = BulletMaxCount - UseBulletCount;
	return ExpendedBullets;
}

