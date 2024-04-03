#include "NormalGun.h"
#include <Numbers.h>
#include <FbxLoader.h>

NormalGun::NormalGun(const unsigned short Attribute,const std::string& FileName)
{
	for ( auto&& Bullet : normalBullet )
	{
		Bullet = std::make_unique<NormalBullet>(Attribute,FileName);
	}
}

NormalGun::~NormalGun()
{
}

void NormalGun::Initialize(const Vector3& Pos,Model* BulletModel,AudioManager* audioManager_,const float& soundVol,const bool& AnotherSound)
{
	fbxModel_.reset(FbxLoader::GetInstance()->LoadModelFromFile("NormalGunFbx",true));
	fbxObj3d_ = FBXObject3d::Create();
	fbxObj3d_->SetModel(fbxModel_.get());
	fbxObj3d_->Update();

	GunTrans.Initialize();
	GunTrans.translation_ = Pos;
	UpdatePosition();

	for ( auto&& Bullet : normalBullet )
	{
		Bullet->Initialize(BulletModel);
	}
	audioManager = audioManager_;

	SoundVol = soundVol;

	AnotherSound_ = AnotherSound;
	
	GunSound = audioManager->LoadAudio("Resources/Sound/fireS.mp3",soundVol,AnotherSound);
	
}

void NormalGun::Update(const Vector3& Pos,const Vector3& rot,const float& vol)
{
	GunTrans.translation_ = Pos;
	GunTrans.SetRot(rot);
	UpdatePosition();
	fbxObj3d_->Update();
	TimeUpdate();

	SoundVol = vol;

	if ( CoolTime == static_cast< uint32_t >( Numbers::Zero ) )
	{
		isReload = false;
	}

	for ( auto&& Bullet : normalBullet )
	{
		Bullet->Update();
	}
}

void NormalGun::Draw(const ViewProjection& viewProjection_,const ViewProjection& LightViewProjection_)
{
	for ( auto&& Bullet : normalBullet )
	{
		Bullet->Draw(viewProjection_,LightViewProjection_);
	}
}

void NormalGun::TrailDraw(const ViewProjection& viewProjection_)
{
	for ( auto&& Bullet : normalBullet )
	{
		Bullet->TrailDraw(viewProjection_);
	}
}

void NormalGun::FbxDraw(const ViewProjection& viewProjection_,const ViewProjection& lightViewProjection_)
{
	fbxObj3d_->Draw(GunTrans,viewProjection_,lightViewProjection_);
}

void NormalGun::FbxShadowDraw(const ViewProjection& viewProjection_)
{
	fbxObj3d_->ShadowDraw(GunTrans,viewProjection_);
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
				UseBulletCount++;

				audioManager->ChangeVolume(GunSound,SoundVol);
				audioManager->PlayWave(GunSound,false);

				Vector3 shootVec = BulletVec - MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(0) * GunTrans.matWorld_);
				Bullet->MakeBullet(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(0) * GunTrans.matWorld_),shootVec.norm(),BulletSpeed);
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

bool NormalGun::GetIsReload()
{
	return isReload;
}

uint32_t NormalGun::GetBulletCount()
{
	uint32_t ExpendedBullets = BulletMaxCount - UseBulletCount;
	return ExpendedBullets;
}

void NormalGun::CSUpdate(ID3D12GraphicsCommandList* commandList)
{
	for (auto&& Bullet : normalBullet)
	{
		Bullet->CSUpadate(commandList);
	}
}

void NormalGun::ParticleDraw(const ViewProjection& viewProjection_)
{
	for (auto&& Bullet : normalBullet)
	{
		Bullet->ParticleDraw(viewProjection_);
	}
}
