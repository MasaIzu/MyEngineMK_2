#include "EnemyBullet.h"
#include <imgui.h>
#include <CollisionManager.h>
#include <SphereCollider.h>
#include <CollisionAttribute.h>

EnemyBullet::EnemyBullet()
{
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		isBulletAlive[i] = 0;
		BulletLifeTime[i] = MaxBulletLifeTime;
		BulletRadius[i] = 0.4f;
		BulletCollider[i] = nullptr;
	}
}

EnemyBullet::~EnemyBullet()
{
}

void EnemyBullet::Initialize()
{

	model_.reset(Model::CreateFromOBJ("sphereColor", true));
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		EnemyBulletWorldTrans[i].Initialize();
		EnemyBulletWorldTrans[i].scale_ = { BulletRadius[i],BulletRadius[i],BulletRadius[i] };
		EnemyBulletWorldTrans[i].TransferMatrix();

		isBulletAlive[i] = 0;
	}


	for (uint32_t i = 0; i < AllBulletCount; i++) {
		// コリジョンマネージャに追加
		BulletCollider[i] = new SphereCollider(Vector4(0, BulletRadius[i], 0, 0), BulletRadius[i]);
		CollisionManager::GetInstance()->AddCollider(BulletCollider[i]);
		BulletCollider[i]->SetAttribute(COLLISION_ATTR_ATTACK);
	}
	collisionManager = CollisionManager::GetInstance();

	ParticleMan = std::make_unique<ParticleManager>();
	ParticleMan->Initialize();
	ParticleMan->SetTextureHandle(TextureManager::Load("effect4.png"));

}

void EnemyBullet::Update()
{
	//前回の位置を記録
	OldPosUpdate();
	//タイマー更新
	BulletAliveTimerUpdate();
	//時間が切れているかどうか
	CheckBulletAlive();
	//生きていたらアプデ
	BulletUpdate();



	ParticleMan->Update();

	for (uint32_t i = 0; i < AllBulletCount; i++) {
		BulletCollider[i]->Update(EnemyBulletWorldTrans[i].matWorld_, BulletRadius[i], EnemyBulletSpeed, BulletVector[i]);
		BulletCollider[i]->SetAttribute(COLLISION_ATTR_ATTACK);
	}
}

void EnemyBullet::Draw(ViewProjection& viewProjection_)
{
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		if (isBulletAlive[i] == true) {
			model_->Draw(EnemyBulletWorldTrans[i], viewProjection_);
		}
	}
}

void EnemyBullet::CSUpdate(ID3D12GraphicsCommandList* cmdList)
{
	ParticleMan->CSUpdate(cmdList);
}

void EnemyBullet::ParticleDraw(ViewProjection& viewProjection_)
{
	ParticleMan->Draw(viewProjection_);
}

void EnemyBullet::CopyParticle()
{
	ParticleMan->CopyData();
}

void EnemyBullet::BulletUpdate()
{

	for (uint32_t i = 0; i < AllBulletCount; i++) {
		if (isBulletAlive[i] == true) {
			EnemyBulletMoveMent[i] = BulletVector[i] * EnemyBulletSpeed;
			if (isExpanding == false) {
				MakeParticle(EnemyBulletWorldTrans[i].translation_, BulletVector[i], EnemyBulletSpeed);
			}
		}
	}

	if (isExpanding) {
		if (BulletRadius[BulletNum_] < EnemyBulletMaxRadius) {
			BulletRadius[BulletNum_] += 0.1f;
			EnemyBulletMoveMent[BulletNum_] = Vector3(0, 0, 0);
			EnemyBulletWorldTrans[BulletNum_].scale_ = Vector3(BulletRadius[BulletNum_], BulletRadius[BulletNum_], BulletRadius[BulletNum_]);
		}
		else {
			isExpanding = false;
		}
	}

	WorldTransUpdate();
}

uint32_t EnemyBullet::MakeEnemyBullet(const Vector3& MakeBulletPos, const Vector3& BulletVec, const float& Distance)
{
	if (BulletCoolTime <= 0) {
		for (uint32_t i = 0; i < AllBulletCount; i++) {
			if (isBulletAlive[i] == false) {
				isBulletAlive[i] = true;
				EnemyBulletWorldTrans[i].translation_ = MakeBulletPos;
				BulletVector[i] = BulletVec;
				BulletLifeTime[i] = MaxBulletLifeTime;
				BulletRadius[i] = 0.4f;
				EnemyBulletSpeed = Distance / static_cast<float>(MaxBulletLifeTime);
				EnemyBulletWorldTrans[i].scale_ = Vector3(BulletRadius[i], BulletRadius[i], BulletRadius[i]);
				BulletCoolTime = MaxBulletCoolTime;
				return i;
			}
		}
	}
	return 0;
}

void EnemyBullet::BulletControl(const uint32_t& BulletNum, const Vector3& BulletVec)
{
	BulletVector[BulletNum] = BulletVec;
}

void EnemyBullet::MakeExpandingStunBullet()
{
	if (isExpanding == false) {
		for (uint32_t i = 0; i < AllBulletCount; i++) {
			if (isBulletAlive[i] == false) {
				isBulletAlive[i] = true;
				BulletLifeTime[i] = MaxBulletLifeTime;
				BulletRadius[i] = 0.4f;
				isExpanding = true;
				BulletNum_ = i;
				break;
			}
		}
	}
}

void EnemyBullet::UpdateWhileExpanding(const Vector3& MakeBulletPos, const Vector3& BulletVec)
{
	if (isExpanding) {
		EnemyBulletWorldTrans[BulletNum_].translation_ = MakeBulletPos;
		BulletVector[BulletNum_] = BulletVec;
	}
}

void EnemyBullet::WorldTransUpdate()
{
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		EnemyBulletWorldTrans[i].translation_ += EnemyBulletMoveMent[i];
		EnemyBulletWorldTrans[i].TransferMatrix();
	}
}

void EnemyBullet::BulletAliveTimerUpdate()
{
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		if (isBulletAlive[i] == true) {
			if (BulletLifeTime[i] > 0) {
				BulletLifeTime[i]--;
			}
		}
	}
	if (BulletCoolTime > 0) {
		BulletCoolTime--;
	}
}

void EnemyBullet::CheckBulletAlive()
{
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		if (BulletLifeTime[i] <= 0) {
			isBulletAlive[i] = false;
			BulletVector[i] = { 0,0,0 };
		}
	}
}

void EnemyBullet::MakeParticle(Vector3& pos, Vector3& BulletVelocity, const float& BulletSpeed)
{
	//for (uint32_t i = 0; i < MackPaticleMax; i++) {
	//	Vector3 Verocty = BulletVelocity;
	//	Vector3 Rand = MyMath::RandomVec3(0, 20);
	//	Verocty += Rand * PlayerParticleSpeed;
	//	Vector4 color = { 0.5f,1.0f,0.3f,1 };
	//	ParticleMan->Add(pos, Verocty, ParticleFile, color);
	//}
	for (float i = 0; i < BulletSpeed; i++) {
		Vector3 Verocty = BulletVelocity;
		Vector3 Rand = MyMath::RandomCenterVec3Normalize(0, 20);
		Verocty += Rand * EnemyParticleSpeed;
		Vector3 AddPos = pos + (BulletVelocity * i);
		Vector3 colorRand = MyMath::RandomVec3(Uint32Vector2(5, 20), Uint32Vector2(0, 3), Uint32Vector2(0, 6)) / 10;
		Vector4 color = { colorRand.x,colorRand.y,colorRand.z, 3 };
		Vector4 DownColor = color / static_cast<float>(MaxBulletLifeTime);
		float scale = (1.0f / BulletSpeed) * i;
		ParticleMan->Add(AddPos, Verocty, ParticleFile, color, DownColor, scale);
	}
}

void EnemyBullet::OldPosUpdate()
{
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		BulletOldPos[i] = EnemyBulletWorldTrans[i].translation_;
	}
}
