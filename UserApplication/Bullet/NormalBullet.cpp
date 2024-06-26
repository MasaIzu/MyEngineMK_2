#include "NormalBullet.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"
#include <Numbers.h>


NormalBullet::NormalBullet(const unsigned short Attribute_,const std::string& FileName)
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
	particleKisekiParticle->Initialize(makeBulletParticleCount,true,FileName);
	particleKisekiParticle->SetTextureHandle(TextureManager::Load("sprite/effect4.png"));

	trail_ = std::make_unique<Trail>(50);
	trail_->SetFirstColor(MyMath::Vec4ToVec3(particleKisekiParticle->GetFirstColorParticle()));

	trail3D_ = std::make_unique<Trail3D>(50);
	trail3D_->SetFirstColor(MyMath::Vec4ToVec3(particleKisekiParticle->GetFirstColorParticle()));
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

	Vector3 look_ = MyMath::MatVector(BulletWorldTrans.matWorld_,Vector3(1,0,0));
	look_.normalize();
	look_ *= TrailSize;

	Vector3 top = BulletWorldTrans.translation_ + look_;
	Vector3 end = BulletWorldTrans.translation_ - look_;

	trail_->SetPos(top,end);
	trail_->SetIsVisible(true);
	trail_->Update();

	trail3D_->SetPos(BulletWorldTrans.translation_);
	trail3D_->SetIsVisible(true);
	trail3D_->Update();

	BulletOldPos = BulletWorldTrans.translation_;
}

void NormalBullet::Draw(const ViewProjection& viewProjection_,const ViewProjection& LightViewProjection_)
{
	if ( isBulletAlive == true )
	{
		model_->Draw(BulletWorldTrans,viewProjection_,LightViewProjection_);
	}
}

void NormalBullet::TrailDraw(const ViewProjection& viewProjection_)
{
	//trail_->Draw(viewProjection_);
	trail3D_->Draw(viewProjection_);
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
		BulletOldPos = pos;
		trail_->ResetTrail(pos);
		trail3D_->ResetTrail(pos);
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