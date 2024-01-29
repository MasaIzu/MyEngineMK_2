#include "NormalBullet.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"
#include <Numbers.h>

NormalBullet::NormalBullet(const unsigned short Attribute_)
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

	particleKisekiParticle = std::make_unique<ParticleEditor>();
	particleKisekiParticle->Initialize(makeBulletParticleCount,true,"Kiseki");
	particleKisekiParticle->SetTextureHandle(TextureManager::Load("sprite/effect4.png"));
}

NormalBullet::~NormalBullet()
{
}

void NormalBullet::Initialize(Model* BulletModel)
{
	model_ = BulletModel;
}

void NormalBullet::Update()
{
	CheckBulletAlive();

	if ( BulletCollider->GetHit() )
	{
		BulletCollider->Reset();
		isBulletAlive = false;
		BulletCollider->SphereMeshHitReset();
		BulletCollider->SetAttribute(COLLISION_ATTR_NOTATTACK);
	}
	if ( BulletCollider->GetSphereMeshHit() )
	{
		isBulletAlive = false;
		BulletCollider->SetAttribute(COLLISION_ATTR_NOTATTACK);
		BulletCollider->SphereMeshHitReset();
	}
	if ( isBulletAlive == true )
	{
		BulletWorldTrans.translation_ += EnemyBulletMoveMent;
	}

	WorldTransUpdate();
	BulletCollider->Update(BulletWorldTrans.matWorld_);
}

void NormalBullet::Draw(const ViewProjection& viewProjection_)
{
	if ( isBulletAlive == true )
	{
		model_->Draw(BulletWorldTrans,viewProjection_);
	}
}

void NormalBullet::CSUpadate(ID3D12GraphicsCommandList* commandList)
{
	particleKisekiParticle->CSUpdate(commandList,MyMath::Vec3ToVec4(BulletWorldTrans.translation_),isBulletAlive);
}

void NormalBullet::ParticleDraw(const ViewProjection& viewProjection_)
{
	particleKisekiParticle->Draw(viewProjection_);
}

void NormalBullet::WorldTransUpdate()
{
	BulletWorldTrans.TransferMatrix();
}

void NormalBullet::CheckBulletAlive()
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
			BulletCollider->SetAttribute(COLLISION_ATTR_NOTATTACK);
		}
	}

}

void NormalBullet::MakeBullet(const Vector3& pos,const Vector3& BulletVelocity,const float& BulletSpeed)
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
	}
}

bool NormalBullet::GetBulletAlive() const {
	return isBulletAlive;
}