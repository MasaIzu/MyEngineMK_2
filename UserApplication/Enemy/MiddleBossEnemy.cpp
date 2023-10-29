#include "MiddleBossEnemy.h"
#include <CollisionAttribute.h>
#include <SphereCollider.h>
#include <imgui.h>
#include <Easing.h>

MiddleBossEnemy::MiddleBossEnemy()
{
	MovieUpdateTimes = MaxMovieUpdateTimes;
	model_.reset(Model::CreateFromOBJ("sphereNormalEnemy",true));
	BossWorldTrans.scale_ = Vector3(5.0f,5.0f,5.0f);
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
	missileBullet = std::make_unique<MissileBullet>();
	this->player = Player;

	// コリジョンマネージャに追加
	MiddleBossCollider = new SphereCollider(Vector4(0,Radius,0,0),Radius);
	CollisionManager::GetInstance()->AddCollider(MiddleBossCollider);
	MiddleBossCollider->SetAttribute(COLLISION_ATTR_ENEMYS);
	MiddleBossCollider->Update(BossWorldTrans.matWorld_);
}

void MiddleBossEnemy::TitleInitialize()
{
	normalGun = std::make_unique<NormalGun>(COLLISION_ATTR_ENEMY_BULLET_ATTACK);
	missileBullet = std::make_unique<MissileBullet>();
}

void MiddleBossEnemy::Update()
{
	if ( isTurn )
	{

		if ( RotTime > 0 )
		{
			RotTime--;
			Angle += RotSpeed;
		}
		else
		{
			isTurn = false;
			isStart = true;
		}

		BossWorldTrans.SetRot(Vector3(0,MyMath::GetAngle(Angle),0));

	}
	if ( isStart )
	{
		Timer();

		Angle = MyMath::Get2VecAngle(BossWorldTrans.translation_ + BossWorldTrans.LookVelocity.look,player->GetPlayerPos());

		BossWorldTrans.SetRot(Vector3(0,MyMath::GetAngle(Angle),0));
		if ( KeepAttackingTime == 0 )
		{
			isAttack = false;
		}
		if ( isAttack )
		{
			if ( BulletCoolTime == 0 )
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
					MoveingTimer = 0;
					MaxMoveingTimer = 30;
					if ( mveType == 0 )
					{
						MovePos = BossWorldTrans.translation_ + ( BossWorldTrans.LookVelocity.look_lookLeft.norm() * MovePower );
					}
					else if ( mveType == 1 )
					{
						MovePos = BossWorldTrans.translation_ + ( BossWorldTrans.LookVelocity.look_lookRight.norm() * MovePower );
					}
					else if ( mveType == 2 )
					{
						MovePos = BossWorldTrans.translation_ + ( BossWorldTrans.LookVelocity.look_lookLeft.norm() * MovePower );
					}
					else if ( mveType == 3 )
					{
						MovePos = BossWorldTrans.translation_ + ( BossWorldTrans.LookVelocity.look_lookRight.norm() * MovePower );
					}
					MoveStartPos = BossWorldTrans.translation_;

					uint32_t MissileShotCount =	9;
					missileBullet->MakeSelectMissileBullet(BossWorldTrans.translation_,
						BossWorldTrans.LookVelocity.lookUp_lookLeft,BossWorldTrans.LookVelocity.lookUp_lookRight,
						BossWorldTrans.LookVelocity.lookDown_lookLeft,BossWorldTrans.LookVelocity.lookDown_lookRight,MissileShotCount);

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
			Velocity = MyMath::lerp( Velocity, goPos, 0.1f);
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
					uint32_t MissileShotCount = 6;
					BackMissileTimes++;
					missileBullet->MakeSelectMissileBullet(BossWorldTrans.translation_,
						BossWorldTrans.LookVelocity.lookLeft,BossWorldTrans.LookVelocity.lookUp,
						BossWorldTrans.LookVelocity.lookRight,MissileShotCount);
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
			enemyHP2DUI->EnemyHpUpdate(MiddleBossHp,MaxMiddleBossHp);
			enemyHP3DUI->EnemyHpUpdate(MiddleBossHp,MaxMiddleBossHp);
		}

		if ( MiddleBossHp > 0 )
		{

		}
		else
		{
			isDead = true;
		}

		normalGun->Update(BossWorldTrans.translation_);
		missileBullet->Update(player->GetPlayerPos());

		MiddleBossCollider->Update(BossWorldTrans.matWorld_);
	}

	WorldTransUpdate();

	ImGui::Begin("MiddleBoss");
	ImGui::Text("Angle = %f",Angle);
	ImGui::Text("AngleSize = %f",AngleSize);
	ImGui::Text("RotTime = %d",RotTime);

	ImGui::SliderFloat("HpPositionX",&HpPosition.x,0,static_cast< float >( WinApp::GetInstance()->window_width ));
	ImGui::SliderFloat("HpPositionY",&HpPosition.y,0,static_cast< float >( WinApp::GetInstance()->window_height ));
	ImGui::Text("HpPosition:%f,%f",HpPosition.x,HpPosition.y);

	ImGui::End();

	enemyHP2DUI->Update();
	enemyHP3DUI->Update();

}

void MiddleBossEnemy::Draw(const ViewProjection& viewProjection_)
{
	normalGun->Draw(viewProjection_);
	missileBullet->Draw(viewProjection_);
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
		BulletCoolTime = MaxBulletCoolTime;

		Vector3 AttackVelocity = player->GetPlayerPos() - BossWorldTrans.translation_;
		AttackVelocity.normalize();

		normalGun->ShotBullet(AttackVelocity);

	}
	else if ( attackType == AttackType::Missile )
	{
		BulletCoolTime = MaxBulletCoolTime;
		uint32_t MissileShotCount = 6;
		missileBullet->MakeSelectMissileBullet(BossWorldTrans.translation_,
			BossWorldTrans.LookVelocity.lookLeft,BossWorldTrans.LookVelocity.lookUp,
			BossWorldTrans.LookVelocity.lookRight,MissileShotCount);

	}
	else if ( attackType == AttackType::MoveingAttack )
	{
		BulletCoolTime = MaxBulletCoolTime;

		Vector3 AttackVelocity = player->GetPlayerPos() - BossWorldTrans.translation_;
		AttackVelocity.normalize();

		normalGun->ShotBullet(AttackVelocity);
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
	attackType = static_cast< AttackType >( MyMath::Random(0,AllAttackTypeCount) );
	if ( attackType != AttackType::Move )
	{
		attackType = static_cast< AttackType >( MyMath::Random(0,AllAttackTypeCount) );
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
		MaxBulletCoolTime = 5;
		AttackCooltime = 50;
		KeepAttackingTime = 50;
	}
	else if ( attackType == AttackType::Missile )
	{
		isAttack = true;
		MaxBulletCoolTime = 50;
		AttackCooltime = 50;
		KeepAttackingTime = 100;
	}
	else if ( attackType == AttackType::MoveingAttack )
	{
		isAttack = true;
		MaxBulletCoolTime = 1;
		AttackCooltime = 50;
		KeepAttackingTime = 10;
	}
	else if ( attackType == AttackType::Move )
	{
		MoveTimes++;
		isMoveing = true;
		isOneMoreTime = MyMath::Random(0,1);
		MoveingTimer = 0;
		MaxMoveingTimer = 50;
		if ( isOneMoreTime == true )
		{
			isOneMoreTime = MyMath::Random(0,1);
		}
		mveType = MyMath::Random(0,3);
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
				MoveTimes = 0;
				isBackSponePos = true;
				isMoveing = false;
				Velocity = { 0,0,0 };
				BackPosCounter = 1;
				BackMissileTimes = 0;
				BulletCoolTime = BackMissileFirstCoolTime;
				BackLerpPos = ( tmp / 2.0f ) + BossWorldTrans.translation_;
				BackPoints.clear();
				BackPoints.push_back(BossWorldTrans.translation_);
				float LeftOrLight = MyMath::JudgeLeftorRight(EndPos,player->GetPlayerPos(),BossWorldTrans.translation_);
				Vector3 EasingWaypoint;
				if ( LeftOrLight == 1.0f )//左
				{
					EasingWaypoint = BossWorldTrans.translation_ + ( BossWorldTrans.LookVelocity.lookBack_lookLeft.norm() * BackBosPower );
					BackPoints.push_back(EasingWaypoint);
				}
				else if ( LeftOrLight == -1.0f )//右
				{
					EasingWaypoint = BossWorldTrans.translation_ + ( BossWorldTrans.LookVelocity.lookBack_lookRight.norm() * BackBosPower );
					BackPoints.push_back(EasingWaypoint);
				}
				else
				{

				}
				Vector3 waypoint = EndPos - EasingWaypoint;
				waypoint = ( waypoint / 2.0f ) + EasingWaypoint + Vector3(0,jampHeight,0) + BossWorldTrans.LookVelocity.lookBack * BackStrength;
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
	uint32_t newAttackType = MyMath::Random(0,AllAttackTypeCount);
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
