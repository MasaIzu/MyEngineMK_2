#include "TutorialEnemy.h"
#include <imgui.h>
#include <random>
#include <CollisionManager.h>
#include <SphereCollider.h>
#include <CollisionAttribute.h>

TutorialEnemy::TutorialEnemy(const Vector3& BonePos_, Model* model)
{
	BonePos = BonePos_;
	model_ = model;
	for (uint32_t i = 0; i < AttackSphereCount; i++) {
		TutorialEnemyAttackSpereCollider[i] = nullptr;
	}
}

TutorialEnemy::~TutorialEnemy()
{
}

void TutorialEnemy::Initialize()
{

	modelDebug_.reset(Model::CreateFromOBJ("sphere", true));
	enemyWorldTrans.Initialize();
	enemyWorldTrans.translation_ = BonePos;
	WorldTransUpdate();

	input_ = Input::GetInstance();

	DebugWorldTrans.Initialize();
	DebugWorldTrans.translation_ = BonePos;
	DebugWorldTrans.scale_ = { SearchingAreaRadius,SearchingAreaRadius,SearchingAreaRadius };
	DebugWorldTrans.alpha = 0.5f;
	DebugWorldTrans.TransferMatrix();

	TutorialEnemyCollider = new SphereCollider(Vector4(0, EnemyRadius, 0, 0), EnemyRadius);
	CollisionManager::GetInstance()->AddCollider(TutorialEnemyCollider);
	TutorialEnemyCollider->SetAttribute(COLLISION_ATTR_ENEMYS);

	for (uint32_t i = 0; i < AttackSphereCount; i++) {
		splinePosition[i] = std::make_unique<SplinePosition>();
		AttackWorldTrans[i].Initialize();
		if (i != 0) {
			AttackWorldTrans[i].translation_ = AttackWorldTrans[i - 1].LookVelocity.lookUp;
		}
		AttackWorldTrans[i].TransferMatrix();
		TutorialEnemyAttackSpereCollider[i] = new SphereCollider(Vector4(0, EnemyRadius, 0, 0), EnemyRadius);
		CollisionManager::GetInstance()->AddCollider(TutorialEnemyAttackSpereCollider[i]);
		TutorialEnemyAttackSpereCollider[i]->SetAttribute(COLLISION_ATTR_ENEMYATTACK);
	}

	collisionManager = CollisionManager::GetInstance();


}

void TutorialEnemy::Update(const Vector3& PlayerPos)
{
	playerPos = PlayerPos;

	if (IsAlive == false) {
		IsAlive = true;
		enemyWorldTrans.translation_ = BonePos;
	}

	if (IsPlayerSpotted) {
		PlayerSpottedMove();
	}
	else {
		PlayerNotSpottedMove();
	}

	if (input_->TriggerKey(DIK_P)) {
		splinePosition[0]->Reset();
	}


	ImGui::Begin("NotFoundPhase");

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
	ImGui::End();


	WorldTransUpdate();


	TutorialEnemyCollider->Update(enemyWorldTrans.matWorld_);
	TutorialEnemyCollider->SetAttribute(COLLISION_ATTR_ENEMYS);
	for (uint32_t i = 0; i < AttackSphereCount; i++) {
		if (i != 0) {
			//AttackWorldTrans[i].parent_ = &AttackWorldTrans[i - 1];
		}
		AttackWorldTrans[i].TransferMatrix();
		TutorialEnemyAttackSpereCollider[i]->Update(AttackWorldTrans[i].matWorld_);
		TutorialEnemyAttackSpereCollider[i]->SetAttribute(COLLISION_ATTR_ENEMYATTACK);
	}

	if (TutorialEnemyCollider->GetHit()) {
		TutorialEnemyCollider->Reset();
		PlayerBulletHit();
	}

	DebugWorldTrans.translation_ = enemyWorldTrans.translation_;
	DebugWorldTrans.TransferMatrix();
}

void TutorialEnemy::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(enemyWorldTrans, viewProjection_);
	for (uint32_t i = 0; i < AttackSphereCount; i++) {
		modelDebug_->Draw(AttackWorldTrans[i], viewProjection_);
	}
}

void TutorialEnemy::PlayerBulletHit()
{
	IsAlive = false;
}

void TutorialEnemy::PlayerNotSpottedMove()
{
	PlayerNotSpottedMoveTimer();

	switch (NotSpottedPhase_)
	{
	case TutorialEnemy::NotSpottedPhase::Walk:

		if (WalkTime > 0) {
			enemyWorldTrans.translation_ += enemyWorldTrans.LookVelocity.look * EnemySpeed;

			//円を作って出ない処理を作る
			tmp = BonePos - enemyWorldTrans.translation_;
			dist = tmp.dot(tmp);
			radius = TerritoryRadius;
			radius *= radius;

			if (dist >= radius) {
				StopTime = Random(150, 240);
				NotSpottedPhase_ = NotSpottedPhase::Interruption;
			}

			enemyWorldTrans.translation_ += enemyMoveMent;
		}
		else {
			StopTime = Random(150, 240);
			NotSpottedPhase_ = NotSpottedPhase::Stop;
		}

		break;
	case TutorialEnemy::NotSpottedPhase::Stop:

		if (StopTime <= 0) {
			WalkTime = Random(60, 120);
			Rot = static_cast<float>(Random(0, 360));
			enemyWorldTrans.SetRot({ 0,MyMath::GetAngle(Rot),0 });
			NotSpottedPhase_ = NotSpottedPhase::Walk;
		}

		break;
	case TutorialEnemy::NotSpottedPhase::Interruption:

		if (StopTime <= 0) {
			WalkTime = Random(80, 140);
			NotSpottedPhase_ = NotSpottedPhase::ForcedWalking;
		}

		break;
	case TutorialEnemy::NotSpottedPhase::ForcedWalking:

		if (WalkTime > 0) {
			BackBonePos = BonePos - enemyWorldTrans.translation_;
			BackBonePos.normalize();
			enemyWorldTrans.translation_ += BackBonePos * EnemySpeed;
		}
		else {
			StopTime = Random(150, 240);
			NotSpottedPhase_ = NotSpottedPhase::Stop;
		}

		break;
	case TutorialEnemy::NotSpottedPhase::SpottedPlayer:
		IsPlayerSpotted = true;
		NotSpottedPhase_ = NotSpottedPhase::Walk;
		SpottedPhase_ = SpottedPhase::Turn;
		break;
	case TutorialEnemy::NotSpottedPhase::Nothing:
		//何もしない

		break;
	default:
		break;
	}
	SearchingPlayer();
}

void TutorialEnemy::PlayerSpottedMove()
{
	GetPlayerForEnemyAngle();
	PlayerSpottedMoveTimer();
	SearchingPlayer();
	switch (SpottedPhase_)
	{
	case TutorialEnemy::SpottedPhase::Intimidation:
		Rot = EnemyToPlayerAngle;
		if (AttackStopTime > 0) {

		}
		else {
			if (GetIsAttackArea()) {
				SpottedPhase_ = SpottedPhase::Attack;
			}
			else {
				SpottedPhase_ = SpottedPhase::Walk;
			}
		}

		break;
	case TutorialEnemy::SpottedPhase::Walk:
		if (IsNeverSpotted == true) {
			SpottedPhase_ = SpottedPhase::Attack;
			AttackPhase_ = AttackPhase::RunAttack;
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
			EnemyToPlayerVec = playerPos - enemyWorldTrans.translation_;
			EnemyToPlayerVec.normalize();
			enemyWorldTrans.translation_ += EnemyToPlayerVec * SpottedEnemySpeed;
		}
		break;
	case TutorialEnemy::SpottedPhase::Stop:


		break;
	case TutorialEnemy::SpottedPhase::Attack:
		IsAttack = true;
		Attack();

		break;
	case TutorialEnemy::SpottedPhase::Wait:


		break;
	case TutorialEnemy::SpottedPhase::LoseSightofPlayer:
		IsPlayerSpotted = false;
		BackBonePos = BonePos - enemyWorldTrans.translation_;
		HowAboutFarAway = BackBonePos.length();
		WalkTime = static_cast<uint32_t>(HowAboutFarAway / EnemySpeed);

		IsNeverSpotted = true;
		NotSpottedPhase_ = NotSpottedPhase::ForcedWalking;
		break;
	case TutorialEnemy::SpottedPhase::Turn:

		Rot = EnemyToPlayerAngle;
		AttackStopTime = Random(10, 20);
		SpottedPhase_ = SpottedPhase::Intimidation;
		break;
	case TutorialEnemy::SpottedPhase::Nothing:


		break;
	default:
		break;
	}
	enemyWorldTrans.SetRot({ 0,MyMath::GetAngle(Rot),0 });
	for (uint32_t i = 0; i < AttackSphereCount; i++) {
		AttackWorldTrans[i].SetRot({ 0,MyMath::GetAngle(Rot),0 });
	}
}

void TutorialEnemy::Attack()
{
	switch (AttackPhase_)
	{
	case TutorialEnemy::AttackPhase::NormalAttack:

		//回転する手前のボール
		start = enemyWorldTrans.translation_ + enemyWorldTrans.LookVelocity.lookUp;
		p1 = enemyWorldTrans.translation_ + enemyWorldTrans.LookVelocity.lookUp_look;
		p2 = enemyWorldTrans.translation_ + enemyWorldTrans.LookVelocity.look;
		p3 = enemyWorldTrans.translation_ + enemyWorldTrans.LookVelocity.lookDown_look;
		end = enemyWorldTrans.translation_ + enemyWorldTrans.LookVelocity.lookDown;
		splinePosition[0]->Update(start, p1, p2, p3, end, NormalAttackSpeed);
		AttackWorldTrans[0].translation_ = splinePosition[0]->NowPos;

		//ボールに追従するボール
		for (uint32_t i = 0; i < AttackSphereCount; i++) {
			if (i != 0) {
				start = AttackWorldTrans[i - 1].translation_ + AttackWorldTrans[i - 1].LookVelocity.lookUp;
				p1 = AttackWorldTrans[i - 1].translation_ + AttackWorldTrans[i - 1].LookVelocity.lookUp_look;
				p2 = AttackWorldTrans[i - 1].translation_ + AttackWorldTrans[i - 1].LookVelocity.look;
				p3 = AttackWorldTrans[i - 1].translation_ + AttackWorldTrans[i - 1].LookVelocity.lookDown_look;
				end = AttackWorldTrans[i - 1].translation_ + AttackWorldTrans[i - 1].LookVelocity.lookDown;
				splinePosition[i]->Update(start, p1, p2, p3, end, NormalAttackSpeed);
				AttackWorldTrans[i].translation_ = splinePosition[i]->NowPos;
			}
		}

		if (splinePosition[0]->GetFinishSpline()) {
			for (uint32_t i = 0; i < AttackSphereCount; i++) {
				splinePosition[i]->Reset();
			}
			AttackDelayTime = MaxAttackDelayTime;
			AttackPhase_ = AttackPhase::Nothing;
			SpottedPhase_ = SpottedPhase::Walk;
		}

		break;
	case TutorialEnemy::AttackPhase::RunAttack:
		if (IsEnemyHasADestination == false) {
			EnemyToPlayerVec = playerPos - enemyWorldTrans.translation_;
			Distance = EnemyToPlayerVec.length();
			Distance += LittleFar;
			EnemyToPlayerVec.normalize();
			DestinationPos = EnemyToPlayerVec * Distance;
			RunAttackTime = static_cast<uint32_t>(Distance / RunAttackSpeed);
			IsEnemyHasADestination = true;
		}
		else {
			if (RunAttackTime > 0) {
				enemyWorldTrans.translation_ += EnemyToPlayerVec * RunAttackSpeed;
			}
			else {
				IsEnemyHasADestination = false;
				AttackDelayTime = MaxAttackDelayTime;
				AttackPhase_ = AttackPhase::Nothing;
				SpottedPhase_ = SpottedPhase::Walk;
			}
		}
		break;
	case TutorialEnemy::AttackPhase::Nothing:
		break;
	default:
		break;
	}
}

void TutorialEnemy::PlayerNotSpottedMoveTimer()
{
	if (WalkTime > 0) {
		WalkTime--;
	}
	if (StopTime > 0) {
		StopTime--;
	}
}

void TutorialEnemy::PlayerSpottedMoveTimer()
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

void TutorialEnemy::SearchingPlayer()
{
	if (IsPlayerSpotted == false) {
		//円を作ってプレイヤーがいたらフェーズ変えに移行
		tmp = enemyWorldTrans.translation_ - playerPos;
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
		tmp = enemyWorldTrans.translation_ - playerPos;
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

