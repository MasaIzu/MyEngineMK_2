#include "MiddleBossEnemy.h"
#include <CollisionAttribute.h>
#include <SphereCollider.h>
#include <imgui.h>
#include <Easing.h>
#include <Numbers.h>

MiddleBossEnemy::MiddleBossEnemy()
{
	MovieUpdateTimes = MaxMovieUpdateTimes;
	model_.reset(Model::CreateFromOBJ("sphereNormalEnemy",true));
	BossWorldTrans.scale_ = Vector3(Scale,Scale,Scale);
	BossWorldTrans.Initialize();

	enemyHP2DUI = std::make_unique<EnemyHP2DUI>();
	enemyHP2DUI->Initialize();

	enemyHP3DUI = std::make_unique<EnemyHP3DUI>();
	enemyHP3DUI->Initialize();

	for ( auto&& old : oldAttackType )
	{
		old = AttackType::NotAttack;
	}
}

MiddleBossEnemy::~MiddleBossEnemy()
{

}

void MiddleBossEnemy::Initialize(Player* Player)
{
	normalGun = std::make_unique<NormalGun>(COLLISION_ATTR_ENEMY_BULLET_ATTACK);
	normalGun->Initialize(BossWorldTrans.translation_,model_.get(),model_.get());
	missileGun = std::make_unique<MissileGun>(COLLISION_ATTR_ENEMY_BULLET_ATTACK);
	missileGun->Initialize(BossWorldTrans.translation_,model_.get(),model_.get());
	this->player = Player;

	// コリジョンマネージャに追加
	MiddleBossCollider = new SphereCollider(Vector4(FloatNumber(fNumbers::fZero),Radius,FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fZero)),Radius);
	CollisionManager::GetInstance()->AddCollider(MiddleBossCollider);
	MiddleBossCollider->SetAttribute(COLLISION_ATTR_ENEMYS);
	MiddleBossCollider->Update(BossWorldTrans.matWorld_);
}

void MiddleBossEnemy::TitleInitialize()
{
	normalGun = std::make_unique<NormalGun>(COLLISION_ATTR_ENEMY_BULLET_ATTACK);
	normalGun->Initialize(BossWorldTrans.translation_,model_.get(),model_.get());
	missileGun = std::make_unique<MissileGun>(COLLISION_ATTR_ENEMY_BULLET_ATTACK);
	missileGun->Initialize(BossWorldTrans.translation_,model_.get(),model_.get());
}

void MiddleBossEnemy::Update()
{
	if ( isTurn )
	{

		if ( RotTime > static_cast< uint32_t >( Numbers::Zero ) )
		{
			RotTime--;
			Angle += RotSpeed;
		}
		else
		{
			isTurn = false;
			isStart = true;
		}

		BossWorldTrans.SetRot(Vector3(FloatNumber(fNumbers::fZero),MyMath::GetAngle(Angle),FloatNumber(fNumbers::fZero)));

	}
	if ( !isDead )
	{
		if ( isStart )
		{
			Timer();

			Angle = MyMath::Get2VecAngle(BossWorldTrans.translation_ + BossWorldTrans.LookVelocity.look,player->GetPlayerPos());

			BossWorldTrans.SetRot(Vector3(FloatNumber(fNumbers::fZero),MyMath::GetAngle(Angle),FloatNumber(fNumbers::fZero)));
			if ( KeepAttackingTime == FloatNumber(fNumbers::fZero) )
			{
				isAttack = false;
			}
			if ( isAttack )
			{
				if ( BulletCoolTime == FloatNumber(fNumbers::fZero) )
				{
					Attack();
				}
			}
			else
			{
				ThinkingTime();
			}

			if ( isMoveing )
			{
				BossWorldTrans.translation_ = Easing::EaseOutQuintVec3(MoveStartPos,MovePos,MoveingTimer,MaxMoveingTimer);

				if ( MoveingTimer == MaxMoveingTimer )
				{
					if ( isOneMoreTime == false )
					{
						isOneMoreTime = true;
						isMoveing = true;
						MoveingTimer = static_cast< uint32_t >( Numbers::Zero );
						MaxMoveingTimer = MoveOneMoreTime;
						if ( mveType == static_cast< uint32_t >( Numbers::Zero ) )
						{
							MovePos = BossWorldTrans.translation_ + ( BossWorldTrans.LookVelocity.look_lookLeft.norm() * MovePower );
						}
						else if ( mveType == static_cast< uint32_t >( Numbers::One ) )
						{
							MovePos = BossWorldTrans.translation_ + ( BossWorldTrans.LookVelocity.look_lookRight.norm() * MovePower );
						}
						else if ( mveType == static_cast< uint32_t >( Numbers::Two ) )
						{
							MovePos = BossWorldTrans.translation_ + ( BossWorldTrans.LookVelocity.look_lookLeft.norm() * MovePower );
						}
						else if ( mveType == static_cast< uint32_t >( Numbers::Three ) )
						{
							MovePos = BossWorldTrans.translation_ + ( BossWorldTrans.LookVelocity.look_lookRight.norm() * MovePower );
						}
						MoveStartPos = BossWorldTrans.translation_;

						missileGun->ShotBullet();
					}
					else
					{
						isMoveing = false;
					}
				}
			}

			if ( isBackSponePos == true )
			{
				Vector3 goPos = BackPoints[ BackPosCounter ] - BossWorldTrans.translation_;
				goPos.normalize();
				Velocity = MyMath::lerp(Velocity,goPos,0.1f);
				BossWorldTrans.translation_ += Velocity * BackSpeed;
				// 中心点の距離の２乗 <= 半径の和の２乗　なら交差
				Vector3 tmp;
				tmp = BackPoints[ BackPosCounter ] - BossWorldTrans.translation_;
				float dist = tmp.dot(tmp);
				float radius2 = 5.0f;
				radius2 *= radius2;

				if ( BackMissileTimes < BackMissileMaxTimes )
				{
					if ( BulletCoolTime == 0 )
					{
						BulletCoolTime = BackMissileCoolTime;
						missileGun->ShotBullet();
					}
				}

				if ( dist <= radius2 )
				{
					if ( BackPosCounter < BackPoints.size() - 1 )
					{
						BackPosCounter++;
					}
					else
					{
						isBackSponePos = false;
					}
				}
			}


			if ( MiddleBossCollider->GetHit() )
			{
				MiddleBossCollider->Reset();
				MiddleBossHp--;
				if ( MiddleBossHp > 0 )
				{

				}
				else
				{
					isDead = true;
					MiddleBossHp = static_cast< uint32_t >( Numbers::Zero );//0固定;
				}
				enemyHP2DUI->EnemyHpUpdate(MiddleBossHp,MaxMiddleBossHp);
				enemyHP3DUI->EnemyHpUpdate(MiddleBossHp,MaxMiddleBossHp);
			}

			MiddleBossCollider->Update(BossWorldTrans.matWorld_);
		}
	}

	WorldTransUpdate();

	normalGun->Update(BossWorldTrans.translation_);
	missileGun->Update(BossWorldTrans.translation_,player->GetPlayerPos());

	enemyHP2DUI->Update();
	enemyHP3DUI->Update();

}

void MiddleBossEnemy::Draw(const ViewProjection& viewProjection_)
{
	normalGun->Draw(viewProjection_);
	missileGun->Draw(viewProjection_);
	if ( isSporn )
	{
		model_->Draw(BossWorldTrans,viewProjection_);
	}
}

void MiddleBossEnemy::DrawSprite(const ViewProjection& viewProjection_)
{
	Vector3 HPposition = BossWorldTrans.translation_ + Vector3(0,10,0);

	enemyHP2DUI->Draw(HpPosition);
	enemyHP3DUI->Draw(HPposition,viewProjection_);
}

bool MiddleBossEnemy::MovieUpdate(const Vector3& startPos,Vector3& endPos)
{
	if ( isSporn == false )
	{
		isSporn = true;
		BossWorldTrans.translation_ = startPos;
		EndPos = endPos;
		Vector3 startToEnd = endPos - startPos;
		Velocity = startToEnd / MaxMovieUpdateTimes;
	}
	else
	{
		if ( MovieUpdateTimes > 0.0f )
		{

			MovieUpdateTimes -= 1.0f;
			BossWorldTrans.translation_ += Velocity;
			AngleSize = MyMath::Get2VecAngle(BossWorldTrans.translation_ + BossWorldTrans.LookVelocity.look,Vector3(0,0,0));

			RotTime = static_cast< uint32_t >( AngleSize / RotSpeed );
		}
		else
		{
			isTurn = true;
			return isTurn;
		}
	}

	WorldTransUpdate();
	MiddleBossCollider->Update(BossWorldTrans.matWorld_);
	missileGun->Update(BossWorldTrans.translation_,Vector3(0,0,0));

	return false;
}

void MiddleBossEnemy::TitleUpdate(const Vector3& TrackingLocation)
{
	missileGun->Update(BossWorldTrans.translation_,TrackingLocation);
}

void MiddleBossEnemy::MakeMissileBullet()
{

}

void MiddleBossEnemy::MakeTitleMissileBullet()
{
	if ( isTitleShot == false )
	{
		isTitleShot = true;
		missileGun->ShotBullet();
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
	if ( BulletCoolTime > 0 )
	{
		BulletCoolTime--;
	}
	if ( KeepAttackingTime > 0 )
	{
		KeepAttackingTime--;
	}
	if ( MoveingTimer < MaxMoveingTimer )
	{
		MoveingTimer++;
	}
}

void MiddleBossEnemy::Attack()
{
	if ( attackType == AttackType::Nomal )
	{
		normalGun->ShotBullet(player->GetPlayerPos());
	}
	else if ( attackType == AttackType::Missile )
	{
		BulletCoolTime = MaxBulletCoolTime;
		missileGun->ShotBullet();

	}
	else if ( attackType == AttackType::MoveingAttack )
	{
		normalGun->ShotBullet(player->GetPlayerPos());
	}
}

void MiddleBossEnemy::ThinkingTime()
{
	if ( isAttack == false && isMoveing == false && isBackSponePos == false )
	{
		if ( AttackCooltime > 0 )
		{
			AttackCooltime--;
		}
		else
		{
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
	attackType = static_cast< AttackType >( MyMath::Random(static_cast< uint32_t >( Numbers::Zero ),AllAttackTypeCount) );
	if ( attackType != AttackType::Move )
	{
		attackType = static_cast< AttackType >( MyMath::Random(static_cast< uint32_t >( Numbers::Zero ),AllAttackTypeCount) );
	}

	if ( attackType == oldAttackType[ 0 ] )
	{
		//同じ攻撃はしない
		uint32_t AttackCount = static_cast< uint32_t >( attackType );
		attackType = static_cast< AttackType >( RandomType(AttackCount) );
	}

	if ( attackType == AttackType::Nomal )
	{
		isAttack = true;
		MaxBulletCoolTime = MaxNomalBulletCoolTime;
		AttackCooltime = MaxAttackCoolTime;
		KeepAttackingTime = MaxNomalTime;
	}
	else if ( attackType == AttackType::Missile )
	{
		isAttack = true;
		MaxBulletCoolTime = MaxMissileBulletCoolTime;
		AttackCooltime = MaxAttackCoolTime;
		KeepAttackingTime = MaxMissileTime;
	}
	else if ( attackType == AttackType::MoveingAttack )
	{
		isAttack = true;
		MaxBulletCoolTime = MaxMoveingAttackBulletTime;
		AttackCooltime = MaxAttackCoolTime;
		KeepAttackingTime = MaxMoveingAttackTime;
	}
	else if ( attackType == AttackType::Move )
	{
		MoveTimes++;
		isMoveing = true;
		isOneMoreTime = MyMath::Random(static_cast< uint32_t >( Numbers::Zero ),static_cast< uint32_t >( Numbers::One ));
		MoveingTimer = static_cast< uint32_t >( Numbers::Zero );
		MaxMoveingTimer = MoveFirstTime;
		if ( isOneMoreTime == true )
		{
			isOneMoreTime = MyMath::Random(static_cast< uint32_t >( Numbers::Zero ),static_cast< uint32_t >( Numbers::One ));
		}
		mveType = MyMath::Random(static_cast< uint32_t >( Numbers::Zero ),static_cast< uint32_t >( Numbers::Three ));
		/*mveType = 0;*/
		if ( mveType == 0 )
		{
			MovePos = BossWorldTrans.translation_ + ( BossWorldTrans.LookVelocity.lookLeft.norm() * MovePower );
		}
		else if ( mveType == 1 )
		{
			MovePos = BossWorldTrans.translation_ + ( BossWorldTrans.LookVelocity.lookRight.norm() * MovePower );
		}
		else if ( mveType == 2 )
		{
			MovePos = BossWorldTrans.translation_ + ( BossWorldTrans.LookVelocity.look.norm() * MovePower );
		}
		else if ( mveType == 3 )
		{
			MovePos = BossWorldTrans.translation_ + ( BossWorldTrans.LookVelocity.lookBack.norm() * MovePower );
		}

		MoveStartPos = BossWorldTrans.translation_;

		if ( MoveTimes >= MaxMoveTimes )
		{
			// 中心点の距離の２乗 <= 半径の和の２乗　なら交差
			Vector3 tmp;
			tmp = EndPos - BossWorldTrans.translation_;
			float dist = tmp.dot(tmp);
			float radius2 = MoveSafeRadius;
			radius2 *= radius2;

			if ( dist <= radius2 )
			{

			}
			else
			{
				MoveTimes = static_cast< uint32_t >( Numbers::Zero );
				isBackSponePos = true;
				isMoveing = false;
				Velocity = { Vec3Number(fNumbers::fZero) };
				BackPosCounter = static_cast< uint32_t >( Numbers::One );
				BackMissileTimes = static_cast< uint32_t >( Numbers::Zero );
				BulletCoolTime = BackMissileFirstCoolTime;
				BackLerpPos = ( tmp / static_cast< float >( fNumbers::fTwoPointZero ) ) + BossWorldTrans.translation_;
				BackPoints.clear();
				BackPoints.push_back(BossWorldTrans.translation_);
				float LeftOrLight = MyMath::JudgeLeftorRight(EndPos,player->GetPlayerPos(),BossWorldTrans.translation_);
				Vector3 EasingWaypoint;
				if ( LeftOrLight == static_cast< float >( fNumbers::fOnePointZero ) )//左
				{
					EasingWaypoint = BossWorldTrans.translation_ + ( BossWorldTrans.LookVelocity.lookBack_lookLeft.norm() * BackBosPower );
					BackPoints.push_back(EasingWaypoint);
				}
				else if ( LeftOrLight == -static_cast< float >( fNumbers::fOnePointZero ) )//右
				{
					EasingWaypoint = BossWorldTrans.translation_ + ( BossWorldTrans.LookVelocity.lookBack_lookRight.norm() * BackBosPower );
					BackPoints.push_back(EasingWaypoint);
				}
				else
				{

				}
				Vector3 waypoint = EndPos - EasingWaypoint;
				waypoint = ( waypoint / static_cast< float >( fNumbers::fTwoPointZero ) ) + EasingWaypoint + Vector3(static_cast< float >( fNumbers::fOnePointZero ),jampHeight,static_cast< float >( fNumbers::fOnePointZero )) + BossWorldTrans.LookVelocity.lookBack * BackStrength;
				BackPoints.push_back(waypoint);
				BackPoints.push_back(EndPos);
			}
		}

	}
	for ( uint32_t i = AttackedKeepCount - 1; i > 0; i-- )
	{
		oldAttackType[ i ] = oldAttackType[ i - 1 ];
	}
	oldAttackType[ 0 ] = attackType;
}

uint32_t MiddleBossEnemy::RandomType(uint32_t& NoUseType)
{
	uint32_t newAttackType = MyMath::Random(static_cast< uint32_t >( Numbers::Zero ),AllAttackTypeCount);
	if ( NoUseType == newAttackType )
	{
		return RandomType(NoUseType);
	}
	else
	{
		return newAttackType;
	}
}

Vector3 MiddleBossEnemy::GetPosition() const
{
	return MyMath::GetWorldTransform(BossWorldTrans.matWorld_);
}
