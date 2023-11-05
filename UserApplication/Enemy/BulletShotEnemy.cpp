#include "BulletShotEnemy.h"
#include <imgui.h>
#include <random>
#include <CollisionManager.h>
#include <SphereCollider.h>
#include <CollisionAttribute.h>
#include <Numbers.h>

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
	float sphereF = FloatNumber(fNumbers::fZero);
	//model_.reset(Model::CreateFromOBJ("sphereColor", true));
	modelDebug_.reset(Model::CreateFromOBJ("sphere", true));
	BulletShotEnemyWorldTrans.Initialize();
	BulletShotEnemyWorldTrans.translation_ = BonePos;
	BulletShotEnemyWorldTransHed.Initialize();
	BulletShotEnemyWorldTransHed.translation_ = { sphereF,EnemyRadius * ColliderSphereCount,sphereF };
	WorldTransUpdate();

	input_ = Input::GetInstance();

	DebugWorldTrans.Initialize();
	DebugWorldTrans.translation_ = BonePos;
	DebugWorldTrans.scale_ = { SearchingAreaRadius,SearchingAreaRadius,SearchingAreaRadius };
	DebugWorldTrans.alpha = FloatNumber(fNumbers::fPointFive);
	DebugWorldTrans.TransferMatrix();

	for (uint32_t i = 0; i < ColliderSphereCount; i++) {
		BulletShotEnemyCollider[i] = new SphereCollider(Vector4(sphereF, EnemyRadius,sphereF,sphereF), EnemyRadius);
		CollisionManager::GetInstance()->AddCollider(BulletShotEnemyCollider[i]);
		BulletShotEnemyCollider[i]->SetAttribute(COLLISION_ATTR_ENEMYS);
		BulletShotEnemyCollider[i]->Update(BulletShotEnemyWorldTrans.matWorld_);
	}
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
		// 加速
		fallVec.y = max(fallVec.y + fallAcc, fallVYMin);
		// 移動
		BulletShotEnemyWorldTrans.translation_.x += fallVec.x;
		BulletShotEnemyWorldTrans.translation_.y += fallVec.y;
		BulletShotEnemyWorldTrans.translation_.z += fallVec.z;
	}

	WorldTransUpdate();

	BulletShotEnemyCollider[0]->Update(BulletShotEnemyWorldTrans.matWorld_);
	BulletShotEnemyCollider[1]->Update(BulletShotEnemyWorldTransHed.matWorld_);
	for (uint32_t i = 0; i < ColliderSphereCount; i++) {
		BulletShotEnemyCollider[i]->SetAttribute(COLLISION_ATTR_ENEMYS);
	}
	CheckCollider();

	for (uint32_t i = 0; i < ColliderSphereCount; i++) {
		if (BulletShotEnemyCollider[i]->GetHit()) {
			BulletShotEnemyCollider[i]->Reset();
			//PlayerBulletHit();
			isDead = true;
		}
	}
	if (BulletShotEnemyCollider[0]->GetHitEnemyEachOtherHit()) {
		BulletShotEnemyWorldTrans.translation_ += BulletShotEnemyCollider[0]->GetRejectVec();
		BulletShotEnemyCollider[0]->EnemyHittingEachOtherReset();
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

		if (WalkTime > static_cast< uint32_t >( Numbers::Zero ) ) {
			BulletShotEnemyWorldTrans.translation_ += BulletShotEnemyWorldTrans.LookVelocity.look * EnemySpeed;

			//円を作って出ない処理を作る
			tmp = BonePos - BulletShotEnemyWorldTrans.translation_;
			dist = tmp.dot(tmp);
			radius = TerritoryRadius;
			radius *= radius;

			if (dist >= radius) {
				StopTime = Random(RandomMin,RandomMax);
				NotSpottedPhase_ = NotSpottedPhase::Interruption;
			}

			BulletShotEnemyWorldTrans.translation_ += enemyMoveMent;
		}
		else {
			StopTime = Random(RandomMin,RandomMax);
			NotSpottedPhase_ = NotSpottedPhase::Stop;
		}

		break;
	case BulletShotEnemy::NotSpottedPhase::Stop:

		if (StopTime <= static_cast< uint32_t >( Numbers::Zero ) ) {
			WalkTime = Random(WalkTimeMin,WalkTimeMax);
			Rot = static_cast<float>(Random(static_cast<uint32_t>(Numbers::Zero),RandomRotMax) );
			BulletShotEnemyWorldTrans.SetRot({ static_cast< float >( Numbers::Zero ),MyMath::GetAngle(Rot),static_cast< float >( Numbers::Zero ) });
			NotSpottedPhase_ = NotSpottedPhase::Walk;
		}

		break;
	case BulletShotEnemy::NotSpottedPhase::Interruption:

		if (StopTime <= static_cast< uint32_t >( Numbers::Zero ) ) {
			WalkTime = Random(WalkTimeMin,WalkTimeMax);
			NotSpottedPhase_ = NotSpottedPhase::ForcedWalking;
		}

		break;
	case BulletShotEnemy::NotSpottedPhase::ForcedWalking:

		if (WalkTime > static_cast< uint32_t >( Numbers::Zero ) ) {
			BackBonePos = BonePos - BulletShotEnemyWorldTrans.translation_;
			BackBonePos.normalize();
			BulletShotEnemyWorldTrans.translation_ += BackBonePos * EnemySpeed;
		}
		else {
			StopTime = Random(RandomMin,RandomMax);
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
		if (AttackStopTime > static_cast< uint32_t >( Numbers::Zero ) ) {

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
			if (AttackDelayTime > static_cast< uint32_t >( Numbers::Zero ) ) {

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
		AttackStopTime = Random(AttackStopTimeMin,AttackStopTimeMax);
		SpottedPhase_ = SpottedPhase::Intimidation;
		break;
	case BulletShotEnemy::SpottedPhase::Nothing:


		break;
	default:
		break;
	}
	BulletShotEnemyWorldTrans.SetRot({ static_cast< float >( Numbers::Zero ),MyMath::GetAngle(Rot),static_cast< float >( Numbers::Zero ) });

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
	if (WalkTime > static_cast< uint32_t >( Numbers::Zero ) ) {
		WalkTime--;
	}
	if (StopTime > static_cast< uint32_t >( Numbers::Zero ) ) {
		StopTime--;
	}
}

void BulletShotEnemy::PlayerSpottedMoveTimer()
{
	if (AttackWalkTime > static_cast< uint32_t >( Numbers::Zero ) ) {
		AttackWalkTime--;
	}
	if (AttackStopTime > static_cast< uint32_t >( Numbers::Zero ) ) {
		AttackStopTime--;
	}
	if (RunAttackTime > static_cast< uint32_t >( Numbers::Zero ) ) {
		RunAttackTime--;
	}
	if (AttackDelayTime > static_cast< uint32_t >( Numbers::Zero ) ) {
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
	float sphereF = FloatNumber(fNumbers::fZero);
	Groundray.dir = { sphereF,-FloatNumber(fNumbers::fOnePointZero),sphereF,sphereF };
	RaycastHit raycastHit;


	// 接地状態
	if (onGround) {
		// スムーズに坂を下る為の吸着距離
		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(Groundray, COLLISION_ATTR_LANDSHAPE, &raycastHit, EnemyRadius * HedRadius + adsDistance)) {
			onGround = true;
			BulletShotEnemyWorldTrans.translation_.y -= (raycastHit.distance - EnemyRadius * HedRadius);
		}
		// 地面がないので落下
		else {
			//onGround = false;
			fallVec = {};
			BulletShotEnemyWorldTrans.translation_ = OldTansPos;
			StopTime = Random(RandomMin,RandomMax);
			NotSpottedPhase_ = NotSpottedPhase::Interruption;
		}
	}
	// 落下状態
	else {
		if (CollisionManager::GetInstance()->Raycast(Groundray, COLLISION_ATTR_LANDSHAPE, &raycastHit, EnemyRadius * HedRadius)) {
			// 着地
			onGround = true;
			BulletShotEnemyWorldTrans.translation_.y -= (raycastHit.distance - EnemyRadius * HedRadius);
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
	float sphereF = FloatNumber(fNumbers::fZero);
	BulletShotEnemyWorldTrans.TransferMatrix();
	BulletShotEnemyWorldTransHed.translation_ = BulletShotEnemyWorldTrans.translation_ + Vector3(sphereF, EnemyRadius * HedRadius,sphereF);
	BulletShotEnemyWorldTransHed.TransferMatrix();
}

uint32_t BulletShotEnemy::Random(const uint32_t& low, const uint32_t& high)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> Randomist(low, high);
	return Randomist(gen);
}

