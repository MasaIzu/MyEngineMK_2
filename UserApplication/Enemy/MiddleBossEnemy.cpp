#include "MiddleBossEnemy.h"
#include <CollisionAttribute.h>
#include <SphereCollider.h>

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
	if (isStart) {
		if (BulletCoolTime > 0) {
			BulletCoolTime--;
		}
		else {
			BulletCoolTime = 5;

			Vector3 Velocity = player->GetPlayerPos() - BossWorldTrans.translation_;
			Velocity.normalize();
			float BulletSpeed = 6.0f;

			multiBullet->MakeBullet(BossWorldTrans.translation_, Velocity, BulletSpeed);
			missileBullet->MakeBullet(BossWorldTrans.translation_);
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
		}
		else {
			isStart = true;
			return isStart;
		}
	}

	WorldTransUpdate();

	return false;
}

void MiddleBossEnemy::ThinkingTime()
{
	if (isAtack) {

	}
	else {
		if (AttackCooltime > 0) {
			AttackCooltime--;
		}
		else {

		}
	}
}

void MiddleBossEnemy::WorldTransUpdate()
{
	BossWorldTrans.TransferMatrix();
}

void MiddleBossEnemy::CheckAttackType(AttackType& attackType)
{
	if (attackType == oldAttackType[0]) {

	}

}
