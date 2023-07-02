#include "PlayerBullet.h"
#include <imgui.h>
#include <CollisionManager.h>
#include <SphereCollider.h>
#include <CollisionAttribute.h>

PlayerBullet::PlayerBullet()
{
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		isBulletAlive[i] = 0;
		BulletLifeTime[i] = 120;
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


	for (int i = 0; i < AllBulletCount; i++) {
		// コリジョンマネージャに追加
		BulletCollider[i] = new SphereCollider(Vector4(0, BulletRadius[i], 0, 0), BulletRadius[i]);
		CollisionManager::GetInstance()->AddCollider(BulletCollider[i]);
		BulletCollider[i]->SetAttribute(COLLISION_ATTR_NOTATTACK);
	}
	collisionManager = CollisionManager::GetInstance();
}

void PlayerBullet::Update()
{
	//タイマー更新
	BulletAliveTimerUpdate();
	//時間が切れているかどうか
	CheckBulletAlive();
	//生きていたら動く
	BulletMove();

	ImGui::Begin("PlayerBullet");

	ImGui::Text("isBulletAlive:%d", isBulletAlive[0]);
	ImGui::Text("isBulletAlive:%d", isBulletAlive[1]);
	ImGui::Text("isBulletAlive:%d", isBulletAlive[2]);
	ImGui::Text("isBulletAlive:%d", isBulletAlive[3]);

	ImGui::Text("BulletVector:%f,%f,%f", BulletVector[0].x, BulletVector[0].y, BulletVector[0].z);

	ImGui::End();



	for (int i = 0; i < AllBulletCount; i++) {
		BulletCollider[i]->Update(playerBulletWorldTrans[i].matWorld_, BulletRadius[i]);
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

void PlayerBullet::BulletMove()
{

	for (uint32_t i = 0; i < AllBulletCount; i++) {
		if (isBulletAlive[i] == true) {
			playerBulletMoveMent[i] = BulletVector[i] * playerBulletSpeed;
		}
	}

	WorldTransUpdate();
}

uint32_t PlayerBullet::MakePlayerBullet(const Vector3& MakeBulletPos,const Vector3& BulletVec)
{
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		if (isBulletAlive[i] == false) {
			isBulletAlive[i] = true;
			playerBulletWorldTrans[i].translation_ = MakeBulletPos;
			BulletVector[i] = BulletVec;
			BulletLifeTime[i] = 120;
			BulletRadius[i] = 0.4f;
			return i;
		}
	}
	return 0;
}

void PlayerBullet::InputingBulletMove(const uint32_t& bulletNum, const Vector3& BulletVec)
{
	BulletRadius[bulletNum] += 0.1f;
	BulletVector[bulletNum] = BulletVec;
	playerBulletWorldTrans[bulletNum].scale_ = Vector3(BulletRadius[bulletNum], BulletRadius[bulletNum], BulletRadius[bulletNum]);
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
