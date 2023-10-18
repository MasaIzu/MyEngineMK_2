#include "NormalBullet.h"
#include <imgui.h>
#include <CollisionManager.h>
#include <SphereCollider.h>
#include <CollisionAttribute.h>

NormalBullet::NormalBullet()
{
	isBulletAlive = 0;
	BulletLifeTime = MaxBulletLifeTime;
	BulletRadius = 0.01f;
	BulletCollider = nullptr;
	playerBulletSpeed = 3.0f;
	isBulletDownSpeed = false;
	BulletRadius = 0.4f;
	playerBulletWorldTrans.scale_ = Vector3(BulletRadius,BulletRadius,BulletRadius);
}


NormalBullet::~NormalBullet()
{
}

void NormalBullet::Initialize(Model* model)
{

	model_ = model;


	playerBulletWorldTrans.Initialize();
	playerBulletWorldTrans.scale_ = { BulletRadius,BulletRadius,BulletRadius };
	playerBulletWorldTrans.TransferMatrix();

	isBulletAlive = 0;

	// コリジョンマネージャに追加
	BulletCollider = new SphereCollider(Vector4(0,BulletRadius,0,0),BulletRadius);
	CollisionManager::GetInstance()->AddCollider(BulletCollider);
	BulletCollider->SetAttribute(COLLISION_ATTR_ATTACK);

	collisionManager = CollisionManager::GetInstance();

}

void NormalBullet::Update()
{
	//前回の位置を記録
	OldPosUpdate();
	//時間が切れているかどうか
	CheckBulletAlive();
	//生きていたらアプデ
	BulletUpdate();
	//死んでたら格納
	SetNotAlivePosition();

	//ImGui::Begin("NormalBullet");

	//ImGui::Text("isBulletAlive:%d", isBulletAlive[0]);
	//ImGui::Text("isBulletAlive:%d", isBulletAlive[1]);
	//ImGui::Text("isBulletAlive:%d", isBulletAlive[2]);
	//ImGui::Text("isBulletAlive:%d", isBulletAlive[3]);

	//ImGui::Text("BulletVector:%f,%f,%f", BulletVector[0].x, BulletVector[0].y, BulletVector[0].z);


	//ImGui::Text("ParticleSize:%d", ParticleMan->GetParticlesListSize());

	////ImGui::SliderInt("MackPaticleMax", &MackPaticleMax, 0, 20);
	//ImGui::SliderFloat("PlayerParticleSpeed", &PlayerParticleSpeed, 0, 1);
	//ImGui::SliderInt("x", &x, 0, 100);
	//ImGui::SliderFloat("PlayerBulletMakeParticleSpeed", &PlayerBulletMakeParticleSpeed, 0, 2);
	//ImGui::End();


	if ( BulletCollider->GetHit() )
	{
		BulletCollider->Reset();
		isBulletAlive = false;
		BulletCollider->SphereMeshHitReset();
	}

	BulletCollider->Update(playerBulletWorldTrans.matWorld_,BulletRadius,playerBulletSpeed,BulletVector);
	BulletCollider->SetAttribute(COLLISION_ATTR_ATTACK);
}

void NormalBullet::Draw(const ViewProjection& viewProjection_)
{
	if ( isBulletAlive == true )
	{
		model_->Draw(playerBulletWorldTrans,viewProjection_);
	}
}


void NormalBullet::BulletUpdate()
{
	if ( isBulletAlive == true )
	{
		playerBulletMoveMent = BulletVector * playerBulletSpeed;

		if ( BulletCollider->GetSphereMeshHit() )
		{
			isBulletAlive = false;
			BulletCollider->SphereMeshHitReset();
		}
	}

	WorldTransUpdate();
}

void NormalBullet::MakePlayerBullet(const Vector3& MakeBulletPos,const Vector3& BulletVec)
{
	if ( isBulletAlive == false )
	{
		isBulletAlive = true;
		BulletLifeTime = MaxBulletLifeTime;
		playerBulletWorldTrans.translation_ = MakeBulletPos;
		BulletVector = BulletVec;
		BulletCollider->Reset();
		BulletCollider->Update(playerBulletWorldTrans.matWorld_,BulletRadius,playerBulletSpeed,BulletVector);
	}
}

void NormalBullet::WorldTransUpdate()
{
	playerBulletWorldTrans.translation_ += playerBulletMoveMent;
	playerBulletWorldTrans.TransferMatrix();
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

void NormalBullet::SetNotAlivePosition()
{
	if ( isBulletAlive == false )
	{
		playerBulletWorldTrans.translation_ = Vector3(0,-50,0);
		playerBulletWorldTrans.TransferMatrix();
	}
}

void NormalBullet::OldPosUpdate()
{
	BulletOldPos = playerBulletWorldTrans.translation_;
}
