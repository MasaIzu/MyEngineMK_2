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
	DebugWorldTrans.scale_ = { 1,1,1 };
	DebugWorldTrans.alpha = 0.5f;
	DebugWorldTrans.TransferMatrix();

	TutorialEnemyCollider = new SphereCollider(Vector4(0, EnemyRadius, 0, 0), EnemyRadius);
	CollisionManager::GetInstance()->AddCollider(TutorialEnemyCollider);
	TutorialEnemyCollider->SetAttribute(COLLISION_ATTR_ENEMYS);

	collisionManager = CollisionManager::GetInstance();

}

void TutorialEnemy::Update(const Vector3& PlayerPos)
{
	playerPos = PlayerPos;

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

	ImGui::Text("BackAngle:%f", BackAngle);
	ImGui::Text("radius:%f", radius);
	ImGui::Text("Rot:%f", Rot);
	ImGui::Text("translation_:%f,%f,%f", enemyWorldTrans.LookVelocity.look.x, enemyWorldTrans.LookVelocity.look.y, enemyWorldTrans.LookVelocity.look.z);
	ImGui::Text("EnemyToPlayerAngle:%f", EnemyToPlayerAngle);
	ImGui::End();


	WorldTransUpdate();


	TutorialEnemyCollider->Update(enemyWorldTrans.matWorld_);
	TutorialEnemyCollider->SetAttribute(COLLISION_ATTR_ENEMYS);

	if (collisionManager->GetIsAttackHit()) {
		PlayerBulletHit();
	}

	DebugWorldTrans.translation_ = enemyWorldTrans.LookVelocity.look + enemyWorldTrans.translation_;
	DebugWorldTrans.TransferMatrix();
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

void TutorialEnemy::PlayerNotFoundMove()
{
	PlayerNotFoundMoveTimer();

	switch (NotFoundPhase_)
	{
	case TutorialEnemy::NotFoundPhase::Walk:

		if (WalkTime > 0) {
			enemyWorldTrans.translation_ += enemyWorldTrans.LookVelocity.look * EnemySpeed;

			//円を作って出ない処理を作る
			tmp = BonePos - enemyWorldTrans.translation_;
			dist = tmp.dot(tmp);
			radius = TerritoryRadius;
			radius *= radius;

			if (dist >= radius) {
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
			BackBonePos = BonePos - enemyWorldTrans.translation_;
			BackBonePos.normalize();
			enemyWorldTrans.translation_ += BackBonePos * EnemySpeed;
		}
		else {
			StopTime = Random(150, 240);
			NotFoundPhase_ = NotFoundPhase::Stop;
		}

		break;
	case TutorialEnemy::NotFoundPhase::FoundPlayer:
		isPlayerFound = true;
		NotFoundPhase_ = NotFoundPhase::Walk;
		FoundPhase_ = FoundPhase::Turn;
		break;
	case TutorialEnemy::NotFoundPhase::Nothing:
		//何もしない

		break;
	default:
		break;
	}
	SearchingPlayer();
}

void TutorialEnemy::PlayerFoundMove()
{
	PlayerFoundMoveTimer();
	GetPlayerForEnemyAngle();
	SearchingPlayer();
	switch (FoundPhase_)
	{
	case TutorialEnemy::FoundPhase::Intimidation:
		Rot = EnemyToPlayerAngle;
		if (AttackStopTime > 0) {

		}
		else {
			if (GetIsAttackArea()) {
				FoundPhase_ = FoundPhase::Attack;
			}
			else {
				FoundPhase_ = FoundPhase::Walk;
			}
		}

		break;
	case TutorialEnemy::FoundPhase::Walk:
		Rot = EnemyToPlayerAngle;
		if (GetIsAttackArea()) {
			FoundPhase_ = FoundPhase::Attack;
		}
		else {
			EnemyToPlayerVec = playerPos - enemyWorldTrans.translation_;
			EnemyToPlayerVec.normalize();
			enemyWorldTrans.translation_ += EnemyToPlayerVec * FoundEnemySpeed;
		}
		break;
	case TutorialEnemy::FoundPhase::Stop:


		break;
	case TutorialEnemy::FoundPhase::Attack:
		isAttack = true;

		FoundPhase_ = FoundPhase::Walk;

		break;
	case TutorialEnemy::FoundPhase::Wait:


		break;
	case TutorialEnemy::FoundPhase::LoseSightofPlayer:
		isPlayerFound = false;
		BackBonePos = BonePos - enemyWorldTrans.translation_;
		HowAboutFarAway = BackBonePos.length();
		WalkTime = static_cast<uint32_t>(HowAboutFarAway / EnemySpeed);

		NotFoundPhase_ = NotFoundPhase::ForcedWalking;
		break;
	case TutorialEnemy::FoundPhase::Turn:

		Rot = EnemyToPlayerAngle;
		AttackStopTime = Random(10, 20);
		FoundPhase_ = FoundPhase::Intimidation;
		break;
	case TutorialEnemy::FoundPhase::Nothing:


		break;
	default:
		break;
	}
	enemyWorldTrans.SetRot({ 0,MyMath::GetAngle(Rot),0 });
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

void TutorialEnemy::PlayerFoundMoveTimer()
{
	if (AttackWalkTime > 0) {
		AttackWalkTime--;
	}
	if (AttackStopTime > 0) {
		AttackStopTime--;
	}
}

void TutorialEnemy::SearchingPlayer()
{
	if (isPlayerFound == false) {
		//円を作ってプレイヤーがいたらフェーズ変えに移行
		tmp = enemyWorldTrans.translation_ - playerPos;
		dist = tmp.dot(tmp);
		radius = SearchingAreaRadius;
		radius *= radius;
		GetPlayerForEnemyAngle();
		if (dist <= radius) {
			NotFoundPhase_ = NotFoundPhase::FoundPlayer;
		}
	}
	else {
		//円を作ってプレイヤーがいたらフェーズ変えに移行
		tmp = enemyWorldTrans.translation_ - playerPos;
		dist = tmp.dot(tmp);
		radius = FoundLookingPlayerRadius;
		radius *= radius;
		if (dist <= radius) {
			
		}
		else {
			FoundPhase_ = FoundPhase::LoseSightofPlayer;
		}
	}
}

void TutorialEnemy::GetPlayerForEnemyAngle()
{
	EnemyToPlayerAngle = MyMath::Get2VecAngle(enemyWorldTrans.LookVelocity.look + enemyWorldTrans.translation_, playerPos);
}

bool TutorialEnemy::GetIsAttackArea()
{
	//円を作ってプレイヤーがいたら攻撃移行
	tmp = enemyWorldTrans.translation_ - playerPos;
	dist = tmp.dot(tmp);
	radius = AttackAreaRadius;
	radius *= radius;
	if (dist <= radius) {
		return true;
	}
	return false;
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
