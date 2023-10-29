#include "NormalBullet.h"
#include "SphereCollider.h"

NormalBullet::NormalBullet(const unsigned short Attribute)
{
	BulletWorldTrans.scale_ = Vector3(0.5f,0.5f,0.5f);
	BulletWorldTrans.Initialize();
	BulletCollider = nullptr;
	BulletLifeTime = 0;
	BulletRadius = 0.5f;
	EnemyBulletSpeed = 0;
	isBulletAlive = false;

	// コリジョンマネージャに追加
	BulletCollider = new SphereCollider(Vector4(0,BulletRadius,0,0),BulletRadius);
	CollisionManager::GetInstance()->AddCollider(BulletCollider);
	BulletCollider->SetAttribute(Attribute);
	BulletCollider->Update(BulletWorldTrans.matWorld_);
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
		}
	}

}

void NormalBullet::MakeBullet(const Vector3& pos,const Vector3& BulletVelocity,const float& BulletSpeed)
{

	if ( isBulletAlive == false )
	{
		BulletLifeTime = 100;
		isBulletAlive = true;
		BulletWorldTrans.translation_ = pos;
		EnemyBulletMoveMent = BulletVelocity * BulletSpeed;
	}

}
