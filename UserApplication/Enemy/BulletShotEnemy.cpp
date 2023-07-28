#include "BulletShotEnemy.h"
#include <imgui.h>
#include <random>
#include <CollisionManager.h>
#include <SphereCollider.h>
#include <CollisionAttribute.h>

BulletShotEnemy::BulletShotEnemy(const Vector3& BonePos_, Model* model)
{
	BonePos = BonePos_;
	model_ = model;
}

BulletShotEnemy::~BulletShotEnemy()
{
}

void BulletShotEnemy::Initialize()
{

	//model_.reset(Model::CreateFromOBJ("sphereColor", true));
	modelDebug_.reset(Model::CreateFromOBJ("sphere", true));
	BulletShotEnemyWorldTrans.Initialize();
	BulletShotEnemyWorldTrans.translation_ = BonePos;
	BulletShotEnemyWorldTransHed.Initialize();
	BulletShotEnemyWorldTransHed.translation_ = { 0.0f,EnemyRadius * 2,0.0f };
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
	OldTansPos = BulletShotEnemyWorldTrans.translation_;
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


	// 落下処理
	if (!onGround) {
		// 下向き加速度
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		// 加速
		fallVec.y = max(fallVec.y + fallAcc, fallVYMin);
		// 移動
		BulletShotEnemyWorldTrans.translation_.x += fallVec.x;
		BulletShotEnemyWorldTrans.translation_.y += fallVec.y;
		BulletShotEnemyWorldTrans.translation_.z += fallVec.z;
	}

	WorldTransUpdate();
	BulletShotEnemyCollider->Update(BulletShotEnemyWorldTrans.matWorld_);
	BulletShotEnemyCollider->SetAttribute(COLLISION_ATTR_ENEMYS);

	CheckCollider();

	if (BulletShotEnemyCollider->GetHit()) {
		BulletShotEnemyCollider->Reset();
		PlayerBulletHit();
	}
	if (BulletShotEnemyCollider->GetHitEnemyEachOtherHit()) {
		BulletShotEnemyWorldTrans.translation_ += BulletShotEnemyCollider->GetRejectVec();
		BulletShotEnemyCollider->EnemyHittingEachOtherReset();
	}

	DebugWorldTrans.translation_ = BulletShotEnemyWorldTrans.translation_;
	DebugWorldTrans.TransferMatrix();

	enemyBullet->Update();
}

void BulletShotEnemy::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(BulletShotEnemyWorldTrans, viewProjection_);
	model_->Draw(BulletShotEnemyWorldTransHed, viewProjection_);
	enemyBullet->Draw(viewProjection_);
	/*for (uint32_t i = 0; i < AttackSphereCount; i++) {
		
	}*/
	//modelDebug_->Draw(DebugWorldTrans, viewProjection_);
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

		if (CheckBetweenToPlayerCollider()) {

		}
		else {
			DistanceNolm = playerPos - MyMath::GetWorldTransform(BulletShotEnemyWorldTransHed.matWorld_);
			DistanceNolm.normalize();
			bulletNumber = enemyBullet->MakeEnemyBullet(MyMath::GetWorldTransform(BulletShotEnemyWorldTransHed.matWorld_), DistanceNolm, EnemyBulletSpeed, SpottedLookingPlayerRadius / EnemyBulletSpeed);

			AttackDelayTime = MaxAttackDelayTime;
			AttackPhase_ = AttackPhase::Nothing;
			SpottedPhase_ = SpottedPhase::Walk;
		}

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
			if (CheckBetweenToPlayerCollider() == false) {
				NotSpottedPhase_ = NotSpottedPhase::SpottedPlayer;
			}
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

void BulletShotEnemy::CheckCollider()
{

	//オブジェクトメッシュコライダー
	// 球の上端から球の下端までのレイキャスト
	Ray Groundray;
	Groundray.start = MyMath::Vec3ToVec4(BulletShotEnemyWorldTrans.translation_);
	Groundray.start.y += EnemyRadius;
	Groundray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;


	// 接地状態
	if (onGround) {
		// スムーズに坂を下る為の吸着距離
		const float adsDistance = 0.2f;
		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(Groundray, COLLISION_ATTR_LANDSHAPE, &raycastHit, EnemyRadius * 2.0f + adsDistance)) {
			onGround = true;
			BulletShotEnemyWorldTrans.translation_.y -= (raycastHit.distance - EnemyRadius * 2.0f);
		}
		// 地面がないので落下
		else {
			//onGround = false;
			fallVec = {};
			BulletShotEnemyWorldTrans.translation_ = OldTansPos;
			StopTime = Random(150, 240);
			NotSpottedPhase_ = NotSpottedPhase::Interruption;
		}
	}
	// 落下状態
	else {
		if (CollisionManager::GetInstance()->Raycast(Groundray, COLLISION_ATTR_LANDSHAPE, &raycastHit, EnemyRadius * 2.0f)) {
			// 着地
			onGround = true;
			BulletShotEnemyWorldTrans.translation_.y -= (raycastHit.distance - EnemyRadius * 2.0f);
		}
	}

}

bool BulletShotEnemy::CheckBetweenToPlayerCollider()
{
	// 球の上端から球の下端までのレイキャスト
	Ray LookRay;
	LookRay.start = MyMath::Vec3ToVec4(BulletShotEnemyWorldTrans.translation_);
	LookRay.start.y += EnemyRadius;
	EnemyToPlayerVec = playerPos - BulletShotEnemyWorldTrans.translation_;
	Distance = EnemyToPlayerVec.length();
	LookRay.dir = MyMath::Vec3ToVec4(EnemyToPlayerVec.norm());
	RaycastHit raycastHit;

	//プレーヤーとの間にオブジェクトがあれば見失う
	if (CollisionManager::GetInstance()->Raycast(LookRay, COLLISION_ATTR_LANDSHAPE, &raycastHit, Distance)) {
		return true;
	}
	else {
		return false;
	}
	return false;
}

bool BulletShotEnemy::GetIsAttackArea()
{
	//円を作ってプレイヤーがいたら攻撃移行
	tmp = BulletShotEnemyWorldTrans.translation_ - playerPos;
	dist = tmp.dot(tmp);
	radius = SpottedLookingPlayerRadius;
	radius *= radius;
	if (dist <= radius) {
		if (CheckBetweenToPlayerCollider() == false) {
			return true;
		}
		else {
			SpottedPhase_ = SpottedPhase::LoseSightofPlayer;
		}
	}
	return false;
}

void BulletShotEnemy::WorldTransUpdate()
{
	BulletShotEnemyWorldTrans.TransferMatrix();
	BulletShotEnemyWorldTransHed.parent_ = &BulletShotEnemyWorldTrans;
	BulletShotEnemyWorldTransHed.TransferMatrix();
}

uint32_t BulletShotEnemy::Random(const uint32_t& low, const uint32_t& high)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(low, high);
	return dist(gen);
}

