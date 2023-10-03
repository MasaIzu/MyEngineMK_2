#include "MiddleBossEnemy.h"
#include <CollisionAttribute.h>
#include <SphereCollider.h>
#include <imgui.h>
#include <Easing.h>

MiddleBossEnemy::MiddleBossEnemy()
{
	MovieUpdateTimes = 120.0f;
	model_.reset(Model::CreateFromOBJ("sphereNormalEnemy", true));
	BossWorldTrans.scale_ = Vector3(5.0f, 5.0f, 5.0f);
	BossWorldTrans.Initialize();

	for (uint32_t i = 0; i < AttackedKeepCount; i++) {
		oldAttackType[i] = AttackType::NotAttack;
	}
}

MiddleBossEnemy::~MiddleBossEnemy()
{
}

void MiddleBossEnemy::Initialize(Player* player)
{
	multiBullet = std::make_unique<MultiBullet>();
	missileBullet = std::make_unique<MissileBullet>();
	this->player = player;

	// コリジョンマネージャに追加
	MiddleBossCollider = new SphereCollider(Vector4(0, Radius, 0, 0), Radius);
	CollisionManager::GetInstance()->AddCollider(MiddleBossCollider);
	MiddleBossCollider->SetAttribute(COLLISION_ATTR_ENEMYS);
	MiddleBossCollider->Update(BossWorldTrans.matWorld_);
}

void MiddleBossEnemy::Update()
{
	if (isTurn) {

		if (RotTime > 0) {
			RotTime--;
			Angle += RotSpeed;
		}
		else {
			isTurn = false;
			isStart = true;
		}

		BossWorldTrans.SetRot(Vector3(0, MyMath::GetAngle(Angle), 0));

	}
	if (isStart) {
		Timer();

		Angle = MyMath::Get2VecAngle(BossWorldTrans.translation_ + BossWorldTrans.LookVelocity.look, player->GetPlayerPos());

		BossWorldTrans.SetRot(Vector3(0, MyMath::GetAngle(Angle), 0));
		if (KeepAttackingTime == 0) {
			isAttack = false;
		}
		if (isAttack) {
			if (BulletCoolTime == 0) {
				Attack();
			}
		}
		else {
			ThinkingTime();
		}

		if (isMoveing) {
			BossWorldTrans.translation_ = Easing::EaseOutQuintVec3(MoveStartPos, MovePos, MoveingTimer, MaxMoveingTimer);

			if (MoveingTimer == MaxMoveingTimer) {
				if (isOneMoreTime == false) {
					isOneMoreTime = true;
					isMoveing = true;
					MoveingTimer = 0;
					MaxMoveingTimer = 30;
					MovePos = BossWorldTrans.translation_ + (BossWorldTrans.LookVelocity.lookLeft.norm() * 20.0f);
					MoveStartPos = BossWorldTrans.translation_;
				}
				else {
					isMoveing = false;
				}
			}
		}

		if (MiddleBossCollider->GetHit()) {
			MiddleBossCollider->Reset();
			MiddleBossHp--;
		}

		if (MiddleBossHp > 0) {

		}
		else {
			isDead = true;
		}

		multiBullet->Update();
		missileBullet->Update(player->GetPlayerPos());

		MiddleBossCollider->Update(BossWorldTrans.matWorld_);
	}

	WorldTransUpdate();

	ImGui::Begin("MiddleBoss");
	ImGui::Text("Angle = %f", Angle);
	ImGui::Text("AngleSize = %f", AngleSize);
	ImGui::Text("RotTime = %d", RotTime);
	ImGui::End();

}

void MiddleBossEnemy::Draw(ViewProjection& viewProjection_)
{
	multiBullet->Draw(viewProjection_);
	missileBullet->Draw(viewProjection_);
	if (isSporn) {
		model_->Draw(BossWorldTrans, viewProjection_);
	}
}

bool MiddleBossEnemy::MovieUpdate(const Vector3& StartPos, Vector3& EndPos)
{
	if (isSporn == false) {
		isSporn = true;
		BossWorldTrans.translation_ = StartPos;
		this->EndPos = EndPos;
		Vector3 startToEnd = EndPos - StartPos;
		Velocity = startToEnd / MovieUpdateTimes;
	}
	else {
		if (MovieUpdateTimes > 0.0f) {

			MovieUpdateTimes -= 1.0f;
			BossWorldTrans.translation_ += Velocity;
			AngleSize = MyMath::Get2VecAngle(BossWorldTrans.translation_ + BossWorldTrans.LookVelocity.look, player->GetPlayerPos());

			RotTime = static_cast<uint32_t>(AngleSize / RotSpeed);
		}
		else {
			isTurn = true;
			return isTurn;
		}
	}

	WorldTransUpdate();

	return false;
}

void MiddleBossEnemy::Timer()
{
	if (BulletCoolTime > 0) {
		BulletCoolTime--;
	}
	if (KeepAttackingTime > 0) {
		KeepAttackingTime--;
	}
	if (MoveingTimer < MaxMoveingTimer) {
		MoveingTimer++;
	}
}

void MiddleBossEnemy::Attack()
{
	if (attackType == AttackType::Nomal) {
		BulletCoolTime = MaxBulletCoolTime;

		Vector3 Velocity = player->GetPlayerPos() - BossWorldTrans.translation_;
		Velocity.normalize();

		multiBullet->MakeBullet(BossWorldTrans.translation_, Velocity, BulletSpeed);

	}
	else if (attackType == AttackType::Missile) {
		BulletCoolTime = MaxBulletCoolTime;
		uint32_t MissileShotCount = 6;
		missileBullet->MakeSelectMissileBullet(BossWorldTrans.translation_,
			BossWorldTrans.LookVelocity.lookLeft, BossWorldTrans.LookVelocity.lookUp,
			BossWorldTrans.LookVelocity.lookRight, MissileShotCount);

	}
	else if (attackType == AttackType::MoveingAttack) {
		BulletCoolTime = MaxBulletCoolTime;

		Vector3 Velocity = player->GetPlayerPos() - BossWorldTrans.translation_;
		Velocity.normalize();

		multiBullet->MakeBullet(BossWorldTrans.translation_, Velocity, BulletSpeed);
	}
}

void MiddleBossEnemy::ThinkingTime()
{
	if (isAttack == false && isMoveing == false) {
		if (AttackCooltime > 0) {
			AttackCooltime--;
		}
		else {
			CheckAttackType();
		}
	}
}

void MiddleBossEnemy::WorldTransUpdate()
{
	BossWorldTrans.TransferMatrix();
}

void MiddleBossEnemy::CheckAttackType()
{
	attackType = static_cast<AttackType>(MyMath::Random(0, AllAttackTypeCount));

	for (uint32_t i = 0; i < AttackedKeepCount; i++) {
		if (attackType == AttackType::Move) {//移動は一回まで
			if (attackType == oldAttackType[i]) {
				uint32_t AttackCount = static_cast<uint32_t>(attackType);
				attackType = static_cast<AttackType>(RandomType(AttackCount));
				break;
			}
		}
	}

	if (attackType == oldAttackType[0]) {
		if (attackType == oldAttackType[1]) {//同じ攻撃は2回まで
			uint32_t AttackCount = static_cast<uint32_t>(attackType);
			attackType = static_cast<AttackType>(RandomType(AttackCount));
		}
	}

	if (attackType == AttackType::Nomal) {
		isAttack = true;
		MaxBulletCoolTime = 5;
		AttackCooltime = 50;
		KeepAttackingTime = 50;
	}
	else if (attackType == AttackType::Missile) {
		isAttack = true;
		MaxBulletCoolTime = 50;
		AttackCooltime = 50;
		KeepAttackingTime = 100;
	}
	else if (attackType == AttackType::MoveingAttack) {
		isAttack = true;
		MaxBulletCoolTime = 1;
		AttackCooltime = 50;
		KeepAttackingTime = 10;
	}
	else if (attackType == AttackType::Move) {
		isMoveing = true;
		isOneMoreTime = false;
		MoveingTimer = 0;
		MaxMoveingTimer = 50;
		MovePos = BossWorldTrans.translation_ + (BossWorldTrans.LookVelocity.lookLeft.norm() * 30.0f);
		MoveStartPos = BossWorldTrans.translation_;
	}
	for (uint32_t i = AttackedKeepCount - 1; i > 0; i--) {
		oldAttackType[i] = oldAttackType[i - 1];
	}
	oldAttackType[0] = attackType;
}

uint32_t MiddleBossEnemy::RandomType(uint32_t& NoUseType)
{
	uint32_t attackType = MyMath::Random(0, AllAttackTypeCount);
	if (NoUseType == attackType) {
		return RandomType(NoUseType);
	}
	else {
		return attackType;
	}

	return attackType;
}
