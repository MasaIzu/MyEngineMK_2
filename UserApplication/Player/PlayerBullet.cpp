#include "PlayerBullet.h"
#include <imgui.h>
#include <CollisionManager.h>
#include <SphereCollider.h>
#include <CollisionAttribute.h>

PlayerBullet::PlayerBullet()
{
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		isBulletAlive[i] = 0;
		BulletLifeTime[i] = MaxBulletLifeTime;
		BulletRadius[i] = 0.01f;
		BulletCollider[i] = nullptr;
		playerBulletSpeed[i] = 3.0f;
		isBulletDownSpeed[i] = false;
		flame[i] = 0.0f;
	}
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Initialize()
{

	model_.reset(Model::CreateFromOBJ("sphereBulletEnemy", true));
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		playerBulletWorldTrans[i].Initialize();
		playerBulletWorldTrans[i].scale_ = { BulletRadius[i],BulletRadius[i],BulletRadius[i] };
		playerBulletWorldTrans[i].TransferMatrix();

		isBulletAlive[i] = 0;
	}


	for (uint32_t i = 0; i < AllBulletCount; i++) {
		// コリジョンマネージャに追加
		BulletCollider[i] = new SphereCollider(Vector4(0, BulletRadius[i], 0, 0), BulletRadius[i]);
		CollisionManager::GetInstance()->AddCollider(BulletCollider[i]);
		BulletCollider[i]->SetAttribute(COLLISION_ATTR_ATTACK);
	}
	collisionManager = CollisionManager::GetInstance();


}

void PlayerBullet::Update()
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

	//ImGui::Begin("PlayerBullet");

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

	//MakeBulletMaxParticle = static_cast<uint32_t>(x);
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		if (BulletCollider[i]->GetHit()) {
			BulletCollider[i]->Reset();
			isBulletAlive[i] = false;
			BulletCollider[i]->SphereMeshHitReset();
			Vector3 Verocity = { 0,0,0 };
			for (uint32_t j = 0; j < DieMaxParticle; j++) {
				MakeParticle(playerBulletWorldTrans[i].translation_, BulletVector[i], PlayerParticleDieSpeed, 0.04f);
			}
		}
	}

	for (uint32_t i = 0; i < AllBulletCount; i++) {
		BulletCollider[i]->Update(playerBulletWorldTrans[i].matWorld_, BulletRadius[i], playerBulletSpeed[i], BulletVector[i]);
		BulletCollider[i]->SetAttribute(COLLISION_ATTR_ATTACK);
	}

}

void PlayerBullet::Draw(ViewProjection& viewProjection_)
{
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		if (isBulletAlive[i] == true) {
			model_->Draw(playerBulletWorldTrans[i], viewProjection_);
		}
	}
}

void PlayerBullet::CSUpdate(ID3D12GraphicsCommandList* cmdList)
{
	
}

void PlayerBullet::ParticleDraw(ViewProjection& viewProjection_)
{
	
}

void PlayerBullet::CopyParticle()
{
	
}

void PlayerBullet::BulletUpdate()
{
	//MyMath::HorizontalProjection(BulletVector[i] * playerBulletSpeed, G, flame)
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		if (isBulletAlive[i] == true) {
			if (isBulletDownSpeed[i] == false) {
				playerBulletMoveMent[i] = /*MyMath::HorizontalProjection(*/BulletVector[i] * playerBulletSpeed[i]/*, G, flame[i])*/;
			}
			else {
				playerBulletMoveMent[i] = MyMath::HorizontalProjection(BulletVector[i] * playerBulletSpeed[i], G, flame[i]);
			}
			if (BulletCollider[i]->GetSphereMeshHit()) {
				isBulletAlive[i] = false;
				BulletCollider[i]->SphereMeshHitReset();
				Vector3 Verocity = { 0,0,0 };
				for (uint32_t j = 0; j < DieMaxParticle; j++) {
					MakeParticle(playerBulletWorldTrans[i].translation_, BulletVector[i], PlayerParticleDieSpeed, 0.04f);
				}
			}
			if (isExpanding == false) {
				for (uint32_t j = 0; j < AttackMaxParticle; j++) {
					MakeParticle(playerBulletWorldTrans[i].translation_, BulletVector[i], PlayerParticleSpeed, 0.04f);
				}
			}
		}
	}

	if (isExpanding) {
		if (BulletRadius[BulletNum_] < playerBulletMaxRadius) {
			BulletRadius[BulletNum_] += 0.1f;
			playerBulletMoveMent[BulletNum_] = Vector3(0, 0, 0);
			playerBulletWorldTrans[BulletNum_].scale_ = Vector3(BulletRadius[BulletNum_], BulletRadius[BulletNum_], BulletRadius[BulletNum_]);
		}
		else {
			isExpanding = false;
		}
	}

	WorldTransUpdate();
}

uint32_t PlayerBullet::MakePlayerBullet(const Vector3& MakeBulletPos, const Vector3& BulletVec, const float& Distance)
{
	if (BulletCoolTime <= 0) {
		for (uint32_t i = 0; i < AllBulletCount; i++) {
			if (isBulletAlive[i] == false) {
				isBulletAlive[i] = true;
				playerBulletSpeed[i] = 10.0f;
				playerBulletWorldTrans[i].translation_ = MakeBulletPos;
				BulletVector[i] = BulletVec;
				BulletSpeedDownTime[i] = static_cast<uint32_t>(Distance / playerBulletSpeed[i]);
				BulletRadius[i] = 0.4f;
				playerBulletWorldTrans[i].scale_ = Vector3(BulletRadius[i], BulletRadius[i], BulletRadius[i]);
				BulletCoolTime = MaxBulletCoolTime;
				flame[i] = 10.0f;
				isBulletDownSpeed[i] = false;
				BulletCollider[i]->Reset();
				BulletCollider[i]->Update(playerBulletWorldTrans[i].matWorld_, BulletRadius[i], playerBulletSpeed[i], BulletVector[i]);
				for (uint32_t j = 0; j < MakeBulletMaxParticle; j++) {
					MakeParticle(playerBulletWorldTrans[i].translation_, BulletVector[i], PlayerBulletMakeParticleSpeed, 0.04f);
				}
				return i;
			}
		}
	}
	return 0;
}

void PlayerBullet::BulletControl(const uint32_t& BulletNum, const Vector3& BulletVec)
{
	BulletVector[BulletNum] = BulletVec;
}

void PlayerBullet::MakeExpandingStunBullet()
{
	if (isExpanding == false) {
		for (uint32_t i = 0; i < AllBulletCount; i++) {
			if (isBulletAlive[i] == false) {
				isBulletAlive[i] = true;
				BulletLifeTime[i] = MaxBulletLifeTime;
				BulletRadius[i] = 0.01f;
				isExpanding = true;
				BulletNum_ = i;
				break;
			}
		}
	}
}

void PlayerBullet::UpdateWhileExpanding(const Vector3& MakeBulletPos, const Vector3& BulletVec)
{
	if (isExpanding) {
		playerBulletWorldTrans[BulletNum_].translation_ = MakeBulletPos;
		BulletVector[BulletNum_] = BulletVec;
	}
}

void PlayerBullet::WorldTransUpdate()
{
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		playerBulletWorldTrans[i].translation_ += playerBulletMoveMent[i];
		playerBulletWorldTrans[i].TransferMatrix();
	}
}

void PlayerBullet::BulletAliveTimerUpdate()
{
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		if (isBulletAlive[i] == true) {
			if (BulletSpeedDownTime[i] > 0) {
				BulletSpeedDownTime[i]--;
			}
		}
	}
	if (BulletCoolTime > 0) {
		BulletCoolTime--;
	}
}

void PlayerBullet::CheckBulletAlive()
{
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		if (isBulletAlive[i] == true) {
			if (BulletSpeedDownTime[i] <= 0) {
				isBulletDownSpeed[i] = true;
				if (playerBulletSpeed[i] > 2.0f) {
					playerBulletSpeed[i] -= 0.5f;
				}

				//isBulletAlive[i] = false;
				//BulletVector[i] = { 0,0,0 };
			}
		}
	}
}

void PlayerBullet::SetNotAlivePosition()
{
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		if (isBulletAlive[i] == false) {
			playerBulletWorldTrans[i].translation_ = Vector3(0, -50, 0);
			playerBulletWorldTrans[i].TransferMatrix();
		}
	}
}

void PlayerBullet::MakeParticle(Vector3& pos, Vector3& BulletVelocity, const float& BulletSpeed)
{
	for (float i = 0; i < BulletSpeed; i++) {
		Vector3 Verocty = BulletVelocity;
		Vector3 Rand = MyMath::RandomCenterVec3Normalize(0, 20);
		Verocty += Rand * PlayerParticleSpeed;
		Vector3 AddPos = pos + (BulletVelocity * i);
		Vector3 colorRand = MyMath::RandomVec3(Uint32Vector2(5, 20), Uint32Vector2(0, 3), Uint32Vector2(0, 6)) / 10;
		Vector4 color = { colorRand.x,colorRand.y,colorRand.z, 3 };
		Vector4 DownColor = color / static_cast<float>(MaxBulletLifeTime);
		float scale = (1.0f / BulletSpeed) * i;
		
	}
}

void PlayerBullet::MakeParticle(Vector3& pos, Vector3& BulletVelocity, const float& BulletSpeed, const float& DownScale)
{

	Vector3 Verocty = BulletVelocity * BulletSpeed;
	Vector3 Rand = MyMath::RandomCenterVec3Normalize(0, 20);
	Verocty += Rand * BulletSpeed;
	Vector3 AddPos = pos;
	Vector3 colorRand = MyMath::RandomVec3(Uint32Vector2(5, 20), Uint32Vector2(0, 3), Uint32Vector2(0, 6)) / 10;
	Vector4 color = { colorRand.x,colorRand.y,colorRand.z, 3 };
	Vector4 DownColor = color / static_cast<float>(DieMaxParticleLife);
	float scale = 1.0f;
	

}

void PlayerBullet::OldPosUpdate()
{
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		BulletOldPos[i] = playerBulletWorldTrans[i].translation_;
	}
}
