#include "ExplosionBullet.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"
#include <Numbers.h>

ExplosionBullet::ExplosionBullet(const unsigned short Attribute_)
{
	BulletWorldTrans.scale_ = Vector3(BulletRadius,BulletRadius,BulletRadius);
	BulletWorldTrans.Initialize();
	BulletCollider = nullptr;
	isBulletAlive = false;

	// コリジョンマネージャに追加
	float sphereF = FloatNumber(fNumbers::fZero);
	BulletCollider = new SphereCollider(Vector4(sphereF,BulletRadius,sphereF,sphereF),BulletRadius);
	CollisionManager::GetInstance()->AddCollider(BulletCollider);
	BulletCollider->SetAttribute(COLLISION_ATTR_NOTATTACK);
	Attribute = Attribute_;
	BulletCollider->Update(BulletWorldTrans.matWorld_);

	ParticleExplosion = std::make_unique<BulletExplosionParticle>();
	ParticleExplosion->Initialize(particleCount);
	ParticleExplosion->SetTextureHandle(TextureManager::Load("sprite/effect4.png"));

	HanabiExplosion = std::make_unique<ParticleHanabiExplosion>();
	HanabiExplosion->Initialize(particleExCount);
	HanabiExplosion->SetTextureHandle(TextureManager::Load("sprite/smoke1.png"));
}

ExplosionBullet::~ExplosionBullet()
{
}

void ExplosionBullet::Initialize(Model* BulletModel)
{
	model_ = BulletModel;
}

void ExplosionBullet::Update()
{
	isBulletNotAlive = false;
	CheckBulletAlive();

	if ( BulletCollider->GetHit() )
	{
		BulletCollider->Reset();
		isBulletAlive = false;
		isBulletNotAlive = true;
		BulletCollider->SphereMeshHitReset();
		BulletCollider->SetAttribute(COLLISION_ATTR_NOTATTACK);
	}
	if ( BulletCollider->GetSphereMeshHit() )
	{
		isBulletAlive = false;
		isBulletNotAlive = true;
		BulletCollider->SetAttribute(COLLISION_ATTR_NOTATTACK);
		BulletCollider->SphereMeshHitReset();
	}
	if ( isBulletAlive == true )
	{
		BulletWorldTrans.translation_ += EnemyBulletMoveMent;
		particlePermissionCount += particleCount / MaxBulletLifeTime;
	}

	WorldTransUpdate();
	BulletCollider->Update(BulletWorldTrans.matWorld_);
}

void ExplosionBullet::Draw(const ViewProjection& viewProjection_)
{
	if ( isBulletAlive == true )
	{
		model_->Draw(BulletWorldTrans,viewProjection_);
	}
}

void ExplosionBullet::CSUpadate(ID3D12GraphicsCommandList* commandList)
{
	ParticleExplosion->CSUpdate(commandList,static_cast< uint32_t >( isBulletAlive ),MyMath::Vec3ToVec4(BulletWorldTrans.translation_),particlePermissionCount);
	HanabiExplosion->CSUpdate(commandList,static_cast< uint32_t >( isBulletNotAlive ),MyMath::Vec3ToVec4(BulletWorldTrans.translation_));
}

void ExplosionBullet::ParticleDraw(const ViewProjection& viewProjection_)
{
	ID3D12GraphicsCommandList* commandList = DirectXCore::GetInstance()->GetCommandList();
	BulletExplosionParticle::PreDraw(commandList);
	ParticleExplosion->Draw(viewProjection_);
	BulletExplosionParticle::PostDraw();
	ParticleHanabiExplosion::PreDraw(commandList);
	HanabiExplosion->Draw(viewProjection_);
	ParticleHanabiExplosion::PostDraw();
}

void ExplosionBullet::WorldTransUpdate()
{
	BulletWorldTrans.TransferMatrix();
}

void ExplosionBullet::CheckBulletAlive()
{

	if ( isBulletAlive == true )
	{
		if ( BulletLifeTime > 0 )
		{
			BulletLifeTime--;
		}
		else
		{
			isBulletAlive = false;
			isBulletNotAlive = true;
			BulletCollider->SetAttribute(COLLISION_ATTR_NOTATTACK);
		}
	}

}

void ExplosionBullet::MakeBullet(const Vector3& pos,const Vector3& BulletVelocity,const float& BulletSpeed)
{
	if ( isBulletAlive == false )
	{
		BulletLifeTime = MaxBulletLifeTime;
		isBulletAlive = true;
		BulletWorldTrans.translation_ = pos;
		EnemyBulletMoveMent = BulletVelocity * BulletSpeed;
		WorldTransUpdate();
		BulletCollider->SetAttribute(Attribute);
		BulletCollider->Update(BulletWorldTrans.matWorld_);
		BulletCollider->Reset();
		BulletCollider->SphereMeshHitReset();
		particlePermissionCount = 0;
		particlePermissionCount += particleCount / MaxBulletLifeTime;
	}
}

bool ExplosionBullet::GetBulletAlive() const {
	return isBulletAlive;
}