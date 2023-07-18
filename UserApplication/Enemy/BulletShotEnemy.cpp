#include "BulletShotEnemy.h"
#include <imgui.h>
#include <random>
#include <CollisionManager.h>
#include <SphereCollider.h>
#include <CollisionAttribute.h>

BulletShotEnemy::BulletShotEnemy(const Vector3& BonePos_)
{
	BonePos = BonePos_;

}

BulletShotEnemy::~BulletShotEnemy()
{
}

void BulletShotEnemy::Initialize()
{

	model_.reset(Model::CreateFromOBJ("sphereColor", true));
	modelDebug_.reset(Model::CreateFromOBJ("sphere", true));
	BulletShotEnemyWorldTrans.Initialize();
	BulletShotEnemyWorldTrans.translation_ = BonePos;
	WorldTransUpdate();

	input_ = Input::GetInstance();

	DebugWorldTrans.Initialize();
	DebugWorldTrans.translation_ = BonePos;
	DebugWorldTrans.scale_ = { SearchingAreaRadius,SearchingAreaRadius,SearchingAreaRadius };
	DebugWorldTrans.alpha = 0.5f;
	DebugWorldTrans.TransferMatrix();

	BulletShotEnemyCollider = new SphereCollider(Vector4(0, EnemyRadius, 0, 0), EnemyRadius);
	CollisionManager::GetInstance()->AddCollider(BulletShotEnemyCollider);
	BulletShotEnemyCollider->SetAttribute(COLLISION_ATTR_ENEMYS);
	BulletShotEnemyCollider->Update(BulletShotEnemyWorldTrans.matWorld_);

	enemyBullet = std::make_unique<EnemyBullet>();
	enemyBullet->Initialize();

	collisionManager = CollisionManager::GetInstance();


}

void BulletShotEnemy::Update(const Vector3& PlayerPos)
{
	playerPos = PlayerPos;

	if (IsAlive == false) {
		IsAlive = true;
		BulletShotEnemyWorldTrans.translation_ = BonePos;
	}

	if (IsPlayerSpotted) {
		PlayerSpottedMove();
	}
	else {
		PlayerNotSpottedMove();
	}


	/*ImGui::Begin("NotFoundPhase");

	if (NotSpottedPhase_ == NotSpottedPhase::Walk) {
		ImGui::Text("Walk");
	}
	else if (NotSpottedPhase_ == NotSpottedPhase::Stop) {
		ImGui::Text("Stop");
	}

	ImGui::Text("WalkTime:%d", WalkTime);
	ImGui::Text("StopTime:%d", StopTime);
	ImGui::Text("RunAttackTime:%d", RunAttackTime);


	ImGui::Text("BackAngle:%f", BackAngle);
	ImGui::Text("radius:%f", radius);
	ImGui::Text("Rot:%f", Rot);
	ImGui::Text("translation_:%f,%f,%f", enemyWorldTrans.LookVelocity.look.x, enemyWorldTrans.LookVelocity.look.y, enemyWorldTrans.LookVelocity.look.z);
	ImGui::Text("EnemyToPlayerAngle:%f", EnemyToPlayerAngle);
	ImGui::End();*/


	WorldTransUpdate();
	BulletShotEnemyCollider->Update(BulletShotEnemyWorldTrans.matWorld_);
	BulletShotEnemyCollider->SetAttribute(COLLISION_ATTR_ENEMYS);

	if (collisionManager->GetIsAttackHit()) {
		PlayerBulletHit();
	}

	DebugWorldTrans.translation_ = BulletShotEnemyWorldTrans.translation_;
	DebugWorldTrans.TransferMatrix();

	enemyBullet->Update();
}

void BulletShotEnemy::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(BulletShotEnemyWorldTrans, viewProjection_);
	enemyBullet->Draw(viewProjection_);
	/*for (uint32_t i = 0; i < AttackSphereCount; i++) {
		modelDebug_->Draw(AttackWorldTrans[i], viewProjection_);
	}*/
}

void BulletShotEnemy::CSUpdate(ID3D12GraphicsCommandList* cmdList)
{
	enemyBullet->CSUpdate(cmdList);
}

void BulletShotEnemy::ParticleDraw(ViewProjection& viewProjection_)
{
	enemyBullet->ParticleDraw(viewProjection_);
}

void BulletShotEnemy::CopyParticle()
{
	enemyBullet->CopyParticle();
}

void BulletShotEnemy::PlayerBulletHit()
{
	IsAlive = false;
}

void BulletShotEnemy::PlayerNotSpottedMove()
{
	PlayerNotSpottedMoveTimer();

	switch (NotSpottedPhase_)
	{
	case BulletShotEnemy::NotSpottedPhase::Walk:

		if (WalkTime > 0) {
			BulletShotEnemyWorldTrans.translation_ += BulletShotEnemyWorldTrans.LookVelocity.look * EnemySpeed;

			//円を作って出ない処理を作る
			tmp = BonePos - BulletShotEnemyWorldTrans.translation_;
			dist = tmp.dot(tmp);
			radius = TerritoryRadius;
			radius *= radius;

			if (dist >= radius) {
				StopTime = Random(150, 240);
				NotSpottedPhase_ = NotSpottedPhase::Interruption;
			}

			BulletShotEnemyWorldTrans.translation_ += enemyMoveMent;
		}
		else {
			StopTime = Random(150, 240);
			NotSpottedPhase_ = NotSpottedPhase::Stop;
		}

		break;
	case BulletShotEnemy::NotSpottedPhase::Stop:

		if (StopTime <= 0) {
			WalkTime = Random(60, 120);
			Rot = static_cast<float>(Random(0, 360));
			BulletShotEnemyWorldTrans.SetRot({ 0,MyMath::GetAngle(Rot),0 });
			NotSpottedPhase_ = NotSpottedPhase::Walk;
		}

		break;
	case BulletShotEnemy::NotSpottedPhase::Interruption:

		if (StopTime <= 0) {
			WalkTime = Random(80, 140);
			NotSpottedPhase_ = NotSpottedPhase::ForcedWalking;
		}

		break;
	case BulletShotEnemy::NotSpottedPhase::ForcedWalking:

		if (WalkTime > 0) {
			BackBonePos = BonePos - BulletShotEnemyWorldTrans.translation_;
			BackBonePos.normalize();
			BulletShotEnemyWorldTrans.translation_ += BackBonePos * EnemySpeed;
		}
		else {
			StopTime = Random(150, 240);
			NotSpottedPhase_ = NotSpottedPhase::Stop;
		}

		break;
	case BulletShotEnemy::NotSpottedPhase::SpottedPlayer:
		IsPlayerSpotted = true;
		NotSpottedPhase_ = NotSpottedPhase::Walk;
		SpottedPhase_ = SpottedPhase::Turn;
		break;
	case BulletShotEnemy::NotSpottedPhase::Nothing:
		//何もしない

		break;
	default:
		break;
	}
	SearchingPlayer();
}

void BulletShotEnemy::PlayerSpottedMove()
{
	GetPlayerForEnemyAngle();
	PlayerSpottedMoveTimer();
	SearchingPlayer();
	switch (SpottedPhase_)
	{
	case BulletShotEnemy::SpottedPhase::Intimidation:
		Rot = EnemyToPlayerAngle;
		if (AttackStopTime > 0) {

		}
		else {
			if (GetIsAttackArea()) {
				SpottedPhase_ = SpottedPhase::Attack;
				AttackPhase_ = AttackPhase::NormalAttack;
			}
			else {
				SpottedPhase_ = SpottedPhase::Walk;
			}
		}

		break;
	case BulletShotEnemy::SpottedPhase::Walk:
		if (IsNeverSpotted == true) {
			SpottedPhase_ = SpottedPhase::Attack;
			AttackPhase_ = AttackPhase::NormalAttack;
			IsNeverSpotted = false;
		}
		Rot = EnemyToPlayerAngle;
		if (GetIsAttackArea()) {
			if (AttackDelayTime > 0) {

			}
			else {
				SpottedPhase_ = SpottedPhase::Attack;
				AttackPhase_ = AttackPhase::NormalAttack;
			}
		}
		else {
			EnemyToPlayerVec = playerPos - BulletShotEnemyWorldTrans.translation_;
			EnemyToPlayerVec.normalize();
			BulletShotEnemyWorldTrans.translation_ += EnemyToPlayerVec * SpottedEnemySpeed;
		}
		break;
	case BulletShotEnemy::SpottedPhase::Stop:


		break;
	case BulletShotEnemy::SpottedPhase::Attack:
		IsAttack = true;
		Attack();

		break;
	case BulletShotEnemy::SpottedPhase::Wait:


		break;
	case BulletShotEnemy::SpottedPhase::LoseSightofPlayer:
		IsPlayerSpotted = false;
		BackBonePos = BonePos - BulletShotEnemyWorldTrans.translation_;
		HowAboutFarAway = BackBonePos.length();
		WalkTime = static_cast<uint32_t>(HowAboutFarAway / EnemySpeed);

		IsNeverSpotted = true;
		NotSpottedPhase_ = NotSpottedPhase::ForcedWalking;
		break;
	case BulletShotEnemy::SpottedPhase::Turn:

		Rot = EnemyToPlayerAngle;
		AttackStopTime = Random(10, 20);
		SpottedPhase_ = SpottedPhase::Intimidation;
		break;
	case BulletShotEnemy::SpottedPhase::Nothing:


		break;
	default:
		break;
	}
	BulletShotEnemyWorldTrans.SetRot({ 0,MyMath::GetAngle(Rot),0 });

}

void BulletShotEnemy::Attack()
{
	switch (AttackPhase_)
	{
	case BulletShotEnemy::AttackPhase::NormalAttack:

		DistanceNolm = playerPos - GetBulletShotEnemyPos();
		DistanceNolm.normalize();
		bulletNumber = enemyBullet->MakeEnemyBullet(GetBulletShotEnemyPos(), DistanceNolm, EnemyBulletSpeed, SpottedLookingPlayerRadius / EnemyBulletSpeed);

		AttackDelayTime = MaxAttackDelayTime;
		AttackPhase_ = AttackPhase::Nothing;
		SpottedPhase_ = SpottedPhase::Walk;


		break;
	case BulletShotEnemy::AttackPhase::RunAttack:

		IsEnemyHasADestination = false;
		AttackDelayTime = MaxAttackDelayTime;
		AttackPhase_ = AttackPhase::Nothing;
		SpottedPhase_ = SpottedPhase::Walk;

		break;
	case BulletShotEnemy::AttackPhase::Nothing:
		break;
	default:
		break;
	}
}

void BulletShotEnemy::PlayerNotSpottedMoveTimer()
{
	if (WalkTime > 0) {
		WalkTime--;
	}
	if (StopTime > 0) {
		StopTime--;
	}
}

void BulletShotEnemy::PlayerSpottedMoveTimer()
{
	if (AttackWalkTime > 0) {
		AttackWalkTime--;
	}
	if (AttackStopTime > 0) {
		AttackStopTime--;
	}
	if (RunAttackTime > 0) {
		RunAttackTime--;
	}
	if (AttackDelayTime > 0) {
		AttackDelayTime--;
	}
}

void BulletShotEnemy::SearchingPlayer()
{
	if (IsPlayerSpotted == false) {
		//円を作ってプレイヤーがいたらフェーズ変えに移行
		tmp = BulletShotEnemyWorldTrans.translation_ - playerPos;
		dist = tmp.dot(tmp);
		radius = SearchingAreaRadius;
		radius *= radius;
		GetPlayerForEnemyAngle();
		if (dist <= radius) {
			NotSpottedPhase_ = NotSpottedPhase::SpottedPlayer;
		}
	}
	else {
		//円を作ってプレイヤーがいたらフェーズ変えに移行
		tmp = BulletShotEnemyWorldTrans.translation_ - playerPos;
		dist = tmp.dot(tmp);
		radius = SpottedLookingPlayerRadius;
		radius *= radius;
		if (dist <= radius) {

		}
		else {
			SpottedPhase_ = SpottedPhase::LoseSightofPlayer;
		}
	}
}

void BulletShotEnemy::GetPlayerForEnemyAngle()
{
	EnemyToPlayerAngle = MyMath::Get2VecAngle(BulletShotEnemyWorldTrans.LookVelocity.look + BulletShotEnemyWorldTrans.translation_, playerPos);
}

bool BulletShotEnemy::GetIsAttackArea()
{
	//円を作ってプレイヤーがいたら攻撃移行
	tmp = BulletShotEnemyWorldTrans.translation_ - playerPos;
	dist = tmp.dot(tmp);
	radius = SpottedLookingPlayerRadius;
	radius *= radius;
	if (dist <= radius) {
		return true;
	}
	return false;
}

void BulletShotEnemy::WorldTransUpdate()
{
	BulletShotEnemyWorldTrans.TransferMatrix();
}

uint32_t BulletShotEnemy::Random(const uint32_t& low, const uint32_t& high)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(low, high);
	return dist(gen);
}

