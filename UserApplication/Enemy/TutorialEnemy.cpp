#include "TutorialEnemy.h"
#include <imgui.h>
#include <random>
#include <CollisionManager.h>
#include <SphereCollider.h>
#include <CollisionAttribute.h>

TutorialEnemy::TutorialEnemy(const Vector3& BonePos_)
{
	BonePos = BonePos_;
}

TutorialEnemy::~TutorialEnemy()
{
}

void TutorialEnemy::Initialize()
{

	model_.reset(Model::CreateFromOBJ("sphereColor", true));
	modelDebug_.reset(Model::CreateFromOBJ("sphereColor", true));
	enemyWorldTrans.Initialize();
	enemyWorldTrans.translation_ = BonePos;
	WorldTransUpdate();

	input_ = Input::GetInstance();

	DebugWorldTrans.Initialize();
	DebugWorldTrans.translation_ = BonePos;
	DebugWorldTrans.scale_ = { TerritoryRadius,TerritoryRadius,TerritoryRadius };
	DebugWorldTrans.alpha = 0.5f;
	DebugWorldTrans.TransferMatrix();

	TutorialEnemyCollider = new SphereCollider(Vector4(0, EnemyRadius, 0, 0), EnemyRadius);
	CollisionManager::GetInstance()->AddCollider(TutorialEnemyCollider);
	TutorialEnemyCollider->SetAttribute(COLLISION_ATTR_ENEMYS);

	collisionManager = CollisionManager::GetInstance();

}

void TutorialEnemy::Update()
{

	if (isAlive == false) {
		isAlive = true;
		enemyWorldTrans.translation_ = BonePos;
	}

	if (isPlayerFound) {
		PlayerFoundMove();
	}
	else {
		PlayerNotFoundMove();
	}




	ImGui::Begin("NotFoundPhase");

	if (NotFoundPhase_ == NotFoundPhase::Walk) {
		ImGui::Text("Walk");
	}
	else if (NotFoundPhase_ == NotFoundPhase::Stop) {
		ImGui::Text("Stop");
	}

	ImGui::Text("WalkTime:%d", WalkTime);
	ImGui::Text("StopTime:%d", StopTime);

	ImGui::Text("BonePos:%f,%f,%f", BonePos.x, BonePos.y, BonePos.z);
	ImGui::Text("translation_:%f,%f,%f", enemyWorldTrans.translation_.x, enemyWorldTrans.translation_.y, enemyWorldTrans.translation_.z);

	ImGui::End();


	WorldTransUpdate();


	TutorialEnemyCollider->Update(enemyWorldTrans.matWorld_);
	TutorialEnemyCollider->SetAttribute(COLLISION_ATTR_ENEMYS);

	if (collisionManager->GetIsAttackHit()) {
		PlayerBulletHit();
	}
}

void TutorialEnemy::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(enemyWorldTrans, viewProjection_);
	modelDebug_->Draw(DebugWorldTrans, viewProjection_);
}

void TutorialEnemy::PlayerBulletHit()
{
	isAlive = false;
}

void TutorialEnemy::PlayerFoundMove()
{
	PlayerFoundMoveTimer();

	switch (FoundPhase_)
	{
	case TutorialEnemy::FoundPhase::Walk:



		break;
	case TutorialEnemy::FoundPhase::Stop:



		break;
	case TutorialEnemy::FoundPhase::Attack:



		break;
	default:
		break;
	}

}

void TutorialEnemy::PlayerNotFoundMove()
{
	PlayerNotFoundMoveTimer();

	switch (NotFoundPhase_)
	{
	case TutorialEnemy::NotFoundPhase::Walk:

		if (WalkTime > 0) {
			enemyWorldTrans.translation_ += enemyWorldTrans.LookVelocity.look * EnemySpeed;

			//‰~‚ðì‚Á‚Äo‚È‚¢ˆ—‚ðì‚é
			tmp = BonePos - enemyWorldTrans.translation_;
			dist = tmp.dot(tmp);
			radius2 = TerritoryRadius;
			radius2 *= radius2;

			if (dist >= radius2) {
				StopTime = Random(150, 240);
				NotFoundPhase_ = NotFoundPhase::Interruption;
			}

			enemyWorldTrans.translation_ += enemyMoveMent;
		}
		else {
			StopTime = Random(150, 240);
			NotFoundPhase_ = NotFoundPhase::Stop;
		}

		break;
	case TutorialEnemy::NotFoundPhase::Stop:

		if (StopTime <= 0) {
			WalkTime = Random(60, 120);
			Rot = static_cast<float>(Random(0, 360));
			enemyWorldTrans.SetRot({ 0,MyMath::GetAngle(Rot),0 });
			NotFoundPhase_ = NotFoundPhase::Walk;
		}

		break;
	case TutorialEnemy::NotFoundPhase::Interruption:

		if (StopTime <= 0) {
			WalkTime = Random(80, 140);
			NotFoundPhase_ = NotFoundPhase::ForcedWalking;
		}

		break;
	case TutorialEnemy::NotFoundPhase::ForcedWalking:

		if (WalkTime > 0) {
			Vector3 BackBonePos = BonePos - enemyWorldTrans.translation_;
			BackBonePos.normalize();
			enemyWorldTrans.translation_ += BackBonePos * EnemySpeed;
		}
		else {
			StopTime = Random(150, 240);
			NotFoundPhase_ = NotFoundPhase::Stop;
		}

		break;
	default:
		break;
	}

}

void TutorialEnemy::PlayerFoundMoveTimer()
{
	if (AttackWalkTime > 0) {
		AttackWalkTime--;
	}
	if (AttackStopTime > 0) {
		AttackStopTime--;
	}
}

void TutorialEnemy::PlayerNotFoundMoveTimer()
{
	if (WalkTime > 0) {
		WalkTime--;
	}
	if (StopTime > 0) {
		StopTime--;
	}
}

void TutorialEnemy::WorldTransUpdate()
{
	enemyWorldTrans.TransferMatrix();
}

uint32_t TutorialEnemy::Random(const uint32_t& low, const uint32_t& high)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(low, high);
	return dist(gen);
}
