#include "MiddleBossEnemy.h"
#include <CollisionAttribute.h>
#include <SphereCollider.h>
#include <imgui.h>
#include <Easing.h>

MiddleBossEnemy::MiddleBossEnemy()
{
	MovieUpdateTimes = MaxMovieUpdateTimes;
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

void MiddleBossEnemy::Initialize(Player* Player)
{
	multiBullet = std::make_unique<MultiBullet>();
	missileBullet = std::make_unique<MissileBullet>();
	this->player = Player;

	// コリジョンマネージャに追加
	MiddleBossCollider = new SphereCollider(Vector4(0, Radius, 0, 0), Radius);
	CollisionManager::GetInstance()->AddCollider(MiddleBossCollider);
	MiddleBossCollider->SetAttribute(COLLISION_ATTR_ENEMYS);
	MiddleBossCollider->Update(BossWorldTrans.matWorld_);
}

void MiddleBossEnemy::TitleInitialize()
{
	multiBullet = std::make_unique<MultiBullet>();
	missileBullet = std::make_unique<MissileBullet>();
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
					if (mveType == 0) {
						MovePos = BossWorldTrans.translation_ + (BossWorldTrans.LookVelocity.lookLeft.norm() * 30.0f);
					}
					else if (mveType == 1) {
						MovePos = BossWorldTrans.translation_ + (BossWorldTrans.LookVelocity.lookRight.norm() * 30.0f);
					}
					MoveStartPos = BossWorldTrans.translation_;

					uint32_t MissileShotCount = 6;
					missileBullet->MakeSelectMissileBullet(BossWorldTrans.translation_,
						BossWorldTrans.LookVelocity.lookLeft, BossWorldTrans.LookVelocity.lookUp,
						BossWorldTrans.LookVelocity.lookRight, MissileShotCount);

				}
				else {
					isMoveing = false;
				}
			}
		}

		if (isBackSponePos == true) {
			if (BackLengthUint32 > BackLengthHalfUint32) {
				BackLengthUint32--;
				BossWorldTrans.translation_ += JampBackVelocity * JumpMoveSpeed;
			}
			else if (BackLengthUint32 <= BackLengthHalfUint32) {
				BackLengthUint32--;
				BossWorldTrans.translation_ += JampBackVelocity * JumpMoveSpeed;

				if (BackLengthUint32 <= 0) {
					isBackSponePos = false;
				}
			}
			else {
		
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

bool MiddleBossEnemy::MovieUpdate(const Vector3& StartPos, Vector3& endPos)
{
	if (isSporn == false) {
		isSporn = true;
		BossWorldTrans.translation_ = StartPos;
		this->EndPos = endPos;
		Vector3 startToEnd = endPos - StartPos;
		Velocity = startToEnd / MovieUpdateTimes;
	}
	else {
		if (MovieUpdateTimes > 0.0f) {

			MovieUpdateTimes -= 1.0f;
			BossWorldTrans.translation_ += Velocity;
			AngleSize = MyMath::Get2VecAngle(BossWorldTrans.translation_ + BossWorldTrans.LookVelocity.look, Vector3(0, 0, 0));

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

void MiddleBossEnemy::TitleUpdate(const Vector3& TrackingLocation)
{
	missileBullet->Update(TrackingLocation);
}

void MiddleBossEnemy::MakeMissileBullet()
{
	if ( isTitleShot == false )
	{
		isTitleShot = true;
		uint32_t MissileShotCount = 6;
		missileBullet->MakeSelectMissileBullet(BossWorldTrans.translation_,
			BossWorldTrans.LookVelocity.lookLeft,BossWorldTrans.LookVelocity.lookUp,
			BossWorldTrans.LookVelocity.lookRight,MissileShotCount);
	}
}

void MiddleBossEnemy::ResetTitleMove()
{
	isTitleShot = false;
	isSporn = false;
	MovieUpdateTimes = MaxMovieUpdateTimes;
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

		Vector3 AttackVelocity = player->GetPlayerPos() - BossWorldTrans.translation_;
		AttackVelocity.normalize();

		multiBullet->MakeBullet(BossWorldTrans.translation_, AttackVelocity, BulletSpeed);

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

		Vector3 AttackVelocity = player->GetPlayerPos() - BossWorldTrans.translation_;
		AttackVelocity.normalize();

		multiBullet->MakeBullet(BossWorldTrans.translation_, AttackVelocity, BulletSpeed);
	}
}

void MiddleBossEnemy::ThinkingTime()
{
	if (isAttack == false && isMoveing == false && isBackSponePos == false) {
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
	attackType = AttackType::Move;

	//for (uint32_t i = 0; i < AttackedKeepCount; i++) {
	//	if (attackType == AttackType::Move) {//移動は一回まで
	//		if (attackType == oldAttackType[i]) {
	//			uint32_t AttackCount = static_cast<uint32_t>(attackType);
	//			attackType = static_cast<AttackType>(RandomType(AttackCount));
	//			break;
	//		}
	//	}
	//}

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
		MoveTimes++;
		isMoveing = true;
		isOneMoreTime = false;
		MoveingTimer = 0;
		MaxMoveingTimer = 50;
		mveType = 1;
		if (mveType == 0) {
			MovePos = BossWorldTrans.translation_ + (BossWorldTrans.LookVelocity.lookLeft.norm() * 30.0f);
		}
		else if (mveType == 1) {
			MovePos = BossWorldTrans.translation_ + (BossWorldTrans.LookVelocity.lookRight.norm() * 30.0f);
		}
		MoveStartPos = BossWorldTrans.translation_;

		if (MoveTimes >= MaxMoveTimes) {

			// 中心点の距離の２乗 <= 半径の和の２乗　なら交差
			Vector3 tmp;
			tmp = EndPos - BossWorldTrans.translation_;
			float dist = tmp.dot(tmp);
			float radius2 = MoveSafeRadius;
			radius2 *= radius2;

			if (dist <= radius2)
			{
				
			}
			else {
				MoveTimes = 0;
				isBackSponePos = true;
				BackLength = tmp.length();
				Vector3 Case;
				Case = tmp;
				Case.y = 0.0f;
				JampBackVelocity = Case.normalize();

				BackLengthUint32 = static_cast<uint32_t>(BackLength / 2);
				BackLengthHalfUint32 = BackLengthUint32;
				BackLengthUint32 *= 2;
				BackLength = static_cast<float>(BackLengthUint32);
				JumpHeight = MaxJumpHeight / ((BackLength / 2.0f) * JumpMoveSpeed);

			}


		}

	}
	for (uint32_t i = AttackedKeepCount - 1; i > 0; i--) {
		oldAttackType[i] = oldAttackType[i - 1];
	}
	oldAttackType[0] = attackType;
}

uint32_t MiddleBossEnemy::RandomType(uint32_t& NoUseType)
{
	uint32_t newAttackType = MyMath::Random(0, AllAttackTypeCount);
	if (NoUseType == newAttackType) {
		return RandomType(NoUseType);
	}
	else {
		return newAttackType;
	}
}

Vector3 MiddleBossEnemy::GetPosition() const
{
	return MyMath::GetWorldTransform(BossWorldTrans.matWorld_);
}
