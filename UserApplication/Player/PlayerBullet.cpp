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
		BulletRadius[i] = 0.4f;
		BulletCollider[i] = nullptr;
	}
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Initialize()
{

	model_.reset(Model::CreateFromOBJ("sphere", true));
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
	//タイマー更新
	BulletAliveTimerUpdate();
	//時間が切れているかどうか
	CheckBulletAlive();
	//生きていたらアプデ
	BulletUpdate();

	ImGui::Begin("PlayerBullet");

	ImGui::Text("isBulletAlive:%d", isBulletAlive[0]);
	ImGui::Text("isBulletAlive:%d", isBulletAlive[1]);
	ImGui::Text("isBulletAlive:%d", isBulletAlive[2]);
	ImGui::Text("isBulletAlive:%d", isBulletAlive[3]);

	ImGui::Text("BulletVector:%f,%f,%f", BulletVector[0].x, BulletVector[0].y, BulletVector[0].z);

	ImGui::End();



	for (uint32_t i = 0; i < AllBulletCount; i++) {
		BulletCollider[i]->Update(playerBulletWorldTrans[i].matWorld_, BulletRadius[i], playerBulletSpeed, BulletVector[i]);
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

void PlayerBullet::BulletUpdate()
{

	for (uint32_t i = 0; i < AllBulletCount; i++) {
		if (isBulletAlive[i] == true) {
			playerBulletMoveMent[i] = BulletVector[i] * playerBulletSpeed;
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

uint32_t PlayerBullet::MakePlayerBullet(const Vector3& MakeBulletPos,const Vector3& BulletVec)
{
	if (BulletCoolTime <= 0) {
		for (uint32_t i = 0; i < AllBulletCount; i++) {
			if (isBulletAlive[i] == false) {
				isBulletAlive[i] = true;
				playerBulletWorldTrans[i].translation_ = MakeBulletPos;
				BulletVector[i] = BulletVec;
				BulletLifeTime[i] = MaxBulletLifeTime;
				BulletRadius[i] = 0.4f;
				playerBulletWorldTrans[i].scale_ = Vector3(BulletRadius[i], BulletRadius[i], BulletRadius[i]);
				BulletCoolTime = MaxBulletCoolTime;
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
				BulletRadius[i] = 0.4f;
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
			if (BulletLifeTime[i] > 0) {
				BulletLifeTime[i]--;
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
		if (BulletLifeTime[i] <= 0) {
			isBulletAlive[i] = false;
			BulletVector[i] = { 0,0,0 };
		}
	}
}
