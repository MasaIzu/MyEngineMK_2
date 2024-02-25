#include "NormalGun.h"
#include <Numbers.h>
#include <FbxLoader.h>

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

void NormalGun::Initialize(const Vector3& Pos,Model* BulletModel,AudioManager* audioManager_,const float& soundVol)
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
	GunSound = audioManager->LoadAudio("Resources/Sound/se_gun_fire10.mp3",soundVol);
}

void NormalGun::Update(const Vector3& Pos,const Vector3& rot)
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

	for ( auto&& Bullet : normalBullet )
	{
		Bullet->Update();
	}
}

void NormalGun::Draw(const ViewProjection& viewProjection_)
{
	for ( auto&& Bullet : normalBullet )
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
		for ( auto&& Bullet : normalBullet )
		{
			if ( !Bullet->GetBulletAlive() )
			{
				CoolTime = MaxCoolTime;
				UseBulletCount++;
				if ( audioManager->NowPlay(GunSound) )
				{
					audioManager->StopWave(GunSound);
				}
				audioManager->PlayWave(GunSound);
				audioManager->ChangeVolume(GunSound,SoundVol);
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
