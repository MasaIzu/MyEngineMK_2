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
		EnemyBulletSpeed[i] = 0.0f;
		BulletCollider[i] = nullptr;
	}
}

EnemyBullet::~EnemyBullet()
{
}

void EnemyBullet::Initialize()
{

	model_.reset(Model::CreateFromOBJ("sphereBulletEnemy", true));
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
	ParticleMan->SetTextureHandle(TextureManager::Load("sprite/effect4.png"));

	//死んでたら格納
	SetNotAlivePosition();
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
	//死んでたら格納
	SetNotAlivePosition();

	ImGui::Begin("enemyBullet");

	ImGui::Text("isBulletAlive:%d", isBulletAlive[0]);
	ImGui::Text("isBulletAlive:%d", isBulletAlive[1]);
	ImGui::Text("isBulletAlive:%d", isBulletAlive[2]);
	ImGui::Text("isBulletAlive:%d", isBulletAlive[3]);

	ImGui::Text("BulletVector:%f,%f,%f", BulletVector[0].x, BulletVector[0].y, BulletVector[0].z);


	ImGui::Text("ParticleSize:%d", ParticleMan->GetParticlesListSize());

	//ImGui::SliderInt("MackPaticleMax", &MackPaticleMax, 0, 20);
	ImGui::SliderFloat("EnemyParticleSpeed", &EnemyParticleSpeed, 0, 1);
	ImGui::SliderFloat("EnemyParticleDieSpeed", &EnemyParticleDieSpeed, 0, 1);
	ImGui::End();

	ParticleMan->Update();

	for (uint32_t i = 0; i < AllBulletCount; i++) {
		BulletCollider[i]->Update(EnemyBulletWorldTrans[i].matWorld_, BulletRadius[i], EnemyBulletSpeed[i], BulletVector[i]);
		BulletCollider[i]->SetAttribute(COLLISION_ATTR_ENEMYBULLETATTACK);
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
			EnemyBulletMoveMent[i] = BulletVector[i] * EnemyBulletSpeed[i];
			if (BulletCollider[i]->GetSphereMeshHit()) {
				isBulletAlive[i] = false;
				BulletCollider[i]->SphereMeshHitReset();
				Vector3 Verocity = { 0,0,0 };
				for (uint32_t j = 0; j < DieMaxParticle; j++) {
					MakeParticle(EnemyBulletWorldTrans[i].translation_, BulletVector[i], EnemyParticleDieSpeed);
				}
			}
			if (isExpanding == false) {
				MakeParticle(EnemyBulletWorldTrans[i].translation_, BulletVector[i], EnemyParticleSpeed);
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

uint32_t EnemyBullet::MakeEnemyBullet(const Vector3& MakeBulletPos, const Vector3& BulletVec, const float& bulletSpeed, const float& bulletLife)
{
	if (BulletCoolTime <= 0) {
		for (uint32_t i = 0; i < AllBulletCount; i++) {
			if (isBulletAlive[i] == false) {
				isBulletAlive[i] = true;
				EnemyBulletWorldTrans[i].translation_ = MakeBulletPos;
				BulletVector[i] = BulletVec;
				BulletLifeTime[i] = static_cast<uint32_t>(bulletLife);
				BulletRadius[i] = 0.4f;
				EnemyBulletSpeed[i] = bulletSpeed;
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

void EnemyBullet::SetNotAlivePosition()
{
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		if (isBulletAlive[i] == false) {
			EnemyBulletWorldTrans[i].translation_ = Vector3(0, -50, 0);
			EnemyBulletWorldTrans[i].TransferMatrix();
		}
	}
}

void EnemyBullet::MakeParticle(Vector3& pos, Vector3& BulletVelocity, const float& BulletSpeed)
{

	Vector3 Verocty = BulletVelocity * BulletSpeed;
	Vector3 Rand = MyMath::RandomCenterVec3Normalize(0, 20);
	Verocty += Rand * BulletSpeed;
	Vector3 AddPos = pos;
	Vector3 colorRand = MyMath::RandomVec3(Uint32Vector2(0, 1), Uint32Vector2(0, 1), Uint32Vector2(0, 4)) / 100;
	Vector4 color = { colorRand.x,colorRand.y,colorRand.z, 3 };
	Vector4 DownColor = color / static_cast<float>(MaxBulletLifeTime);
	float scale = 1.0f;
	float DownScale = 0.02f;
	ParticleMan->Add(AddPos, Verocty, ParticleFile, color, DownColor, scale, DownScale);

}

void EnemyBullet::OldPosUpdate()
{
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		BulletOldPos[i] = EnemyBulletWorldTrans[i].translation_;
	}
}
