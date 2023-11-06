#include "Player.h"
#include "WinApp.h"
#include "MyMath.h"
#include "CollisionManager.h"
#include"ImGuiManager.h"
//#include <FbxLoader.h>
#include <SphereCollider.h>
#include "BulletDamage.h"
#include "CollisionAttribute.h"
#include "Numbers.h"

Player::Player()
{
}

Player::~Player()
{
	CollisionManager::GetInstance()->RemoveCollider(PlayerCollider);
}

void Player::Initialize(const Vector3& Pos,const ViewProjection* viewProjection)
{
	input_ = Input::GetInstance();
	model_.reset(Model::CreateFromOBJ("sphere",true));
	playerWorldTrans.Initialize();
	playerWorldTrans.translation_ = Pos;
	playerWorldTrans.scale_ = Vec3Number(fNumbers::fOnePointZero);
	playerWorldTrans.TransferMatrix();

	viewProjection_ = viewProjection;

	StartingPointOfGrapple.Initialize();

	playerNormalGun = std::make_unique<NormalGun>(COLLISION_ATTR_ATTACK);
	playerNormalGun->Initialize(Pos,model_.get(),model_.get());

	DebugWorldTrans.Initialize();

	// コリジョンマネージャに追加
	float sphereF = FloatNumber(fNumbers::fZero);
	PlayerCollider = new SphereCollider(Vector4(sphereF,Radius,sphereF,sphereF),Radius);
	CollisionManager::GetInstance()->AddCollider(PlayerCollider);
	PlayerCollider->SetAttribute(COLLISION_ATTR_ALLIES);
	PlayerCollider->Update(playerWorldTrans.matWorld_);

	playerUI = std::make_unique<PlayerUI>();
	playerUI->Initialize();

	playerUI->PlayerHpUpdate(static_cast< uint32_t >( PlayerHP ),static_cast< uint32_t >( PlayerMaxHP ));

	animation = std::make_unique<Animation>();
	animation->Initialize();

	playerMovement = std::make_unique<PlayerMovement>();
}

void Player::Update()
{
	isAttack = false;
	isPressing = false;

	//攻撃
	if ( input_->MouseInputing(static_cast<int>(Numbers::Zero)) )
	{
		isAttack = true;
	}

	//当たり判定チェック
	CheckHitCollision();
	//HPのアップデート
	HPUpdate();

	//回転させる
	PlayerRot(isAttack);
	
	playerWorldTrans.translation_ += playerMovement->Move(playerWorldTrans);

	//落下
	Fall();
	//当たり判定チェック
	CheckPlayerCollider();

	if ( isCameraModeNotFree == true )
	{
		//カメラの位置でアルファが決まる
		Alpha = FloatNumber(fNumbers::fOnePointZero) - ( FloatNumber(fNumbers::fOnePointZero) - PlayerToCameraDistance / CameraMaxDistance );
		playerWorldTrans.alpha = Alpha;
	}


	if ( input_->TriggerKey(DIK_LSHIFT) )
	{
		if ( animation->GetNowAnimFinish() )
		{
			animation->SetAnimation(static_cast< uint32_t >( PlayerAnimation::Step ),static_cast< uint32_t >( Numbers::Zero ),playerAnimTime.Step,false);
			DeterminationDirection();
		}
	}

	SlideBoostUpdate();

	//移動の値更新
	WorldTransUpdate();

	isGrapple = false;
	if ( input_->MouseInputing(static_cast< uint32_t >( Numbers::One )) )
	{

	}


	if ( input_->MouseInputTrigger(static_cast< uint32_t >( Numbers::Zero )) )
	{
		/*LeftBoneNum++;*/
	}
	if ( input_->MouseInputTrigger(static_cast< uint32_t >( Numbers::One )) )
	{
		/*LeftBoneNum--;*/
	}

	ImGui::Begin("Player");

	ImGui::Text("%d",LeftBoneNum);
	ImGui::Text("PlayerMoveRotation = %f",playerMovement->GetPlayerAngle());

	ImGui::End();

	//uiのアプデ
	playerUI->Update();

	if ( input_->TriggerKey(DIK_F5) )
	{
		isAlive = true;
		PlayerHP = PlayerMaxHP;
		playerUI->PlayerHpUpdate(static_cast< uint32_t >( PlayerHP ),static_cast< uint32_t >( PlayerMaxHP ));
	}

	if ( isAlive == false )
	{
		if ( isDieMotion == false )
		{
			isDieMotion = true;
			animation->SetAnimation(static_cast< uint32_t >( PlayerAnimation::LeftDown ),static_cast< uint32_t >( Numbers::Zero ),playerAnimTime.DieMotion,false);
		}
	}
	animation->Update();
	playerNormalGun->Update(MyMath::GetWorldTransform(animation->GetBonePos(LeftBoneNum) * playerWorldTrans.matWorld_));
}

void Player::Draw()
{
	model_->Draw(DebugWorldTrans,*viewProjection_);
	if ( isPressing )
	{
		model_->Draw(StartingPointOfGrapple,*viewProjection_);
	}
	playerNormalGun->Draw(*viewProjection_);
	model_->Draw(playerWorldTrans,*viewProjection_);
}

void Player::FbxDraw() {
	animation->FbxDraw(playerWorldTrans,*viewProjection_);
}


void Player::DrawSprite()
{
	playerUI->Draw();
}

void Player::AttackUpdate(const Vector3& EnemyPos,bool& LockOn)
{
	ImGui::Begin("PlayerLockOn");

	ImGui::Text("LockOn = %d",LockOn);

	ImGui::End();
	if ( isAttack == true )
	{
		PlayerAttack(EnemyPos,LockOn);
	}

	playerUI->AttackReticleUpdate(LockOn);
}

void Player::PlayerRot(const bool& Attack)
{
	playerMovement->PlayerAngle(Attack);
	if ( !Attack )
	{
		playerWorldTrans.SetRot(Vector3(FloatNumber(fNumbers::fZero),cameraRot.x + MyMath::GetAngle(playerMovement->GetPlayerAngle()),FloatNumber(fNumbers::fZero)));
	}
	else
	{
		playerWorldTrans.SetRot(Vector3(FloatNumber(fNumbers::fZero),MyMath::GetAngle(FixedAngle) + MyMath::GetAngle(playerMovement->GetPlayerAngle()),FloatNumber(fNumbers::fZero)));
	}
	//値更新
	WorldTransUpdate();
}

void Player::PlayerAttack(const Vector3& EnemyPos,bool& LockOn)
{
	if ( LockOn )
	{
		FixedAngle = MyMath::Get2VecAngle(playerWorldTrans.translation_ + playerWorldTrans.LookVelocity.look,EnemyPos);
		if ( playerMovement->GetIsRotFinish() )
		{
			playerNormalGun->ShotBullet(EnemyPos);
		}
	}
	else
	{
		FixedAngle = MyMath::Get2VecAngle(playerWorldTrans.translation_ + playerWorldTrans.LookVelocity.look,TargetPosition);
		if ( playerMovement->GetIsRotFinish() )
		{
			playerNormalGun->ShotBullet(TargetPosition);
		}
	}

	//if ( AttackPhase_ == AttackPhase::Nothing )
	//{
	//	AttackPhase_ = AttackPhase::AttackCombo1;
	//}
	//switch ( AttackPhase_ )
	//{
	//case Player::AttackPhase::AttackCombo1:
	//	if ( LockOn )
	//	{
	//		playerNormalGun->ShotBullet(EnemyPos);
	//	}
	//	else
	//	{
	//		playerNormalGun->ShotBullet(TargetPosition);
	//	}

	//	break;
	//case Player::AttackPhase::AttackCombo2:

	//	break;
	//case Player::AttackPhase::AttackCombo3:

	//	break;
	//case Player::AttackPhase::AttackCombo4:

	//	break;
	//case Player::AttackPhase::AttackUlt:

	//	break;
	//case Player::AttackPhase::Nothing:

	//	break;
	//default:
	//	break;
	//}
}

void Player::WorldTransUpdate()
{
	playerWorldTrans.TransferMatrix();
}

void Player::CheckPlayerCollider()
{
	//isHitRail = false;
	//isHitFirstRail = false;
	// ワールド行列更新
	playerWorldTrans.TransferMatrix();
	PlayerCollider->Update(playerWorldTrans.matWorld_);

	//地面メッシュコライダー
	{
		// 球の上端から球の下端までのレイキャスト
		Ray Groundray;
		Groundray.start = MyMath::Vec3ToVec4(playerWorldTrans.translation_);
		Groundray.start.y += Radius;
		Groundray.dir = { FloatNumber(fNumbers::fZero),-FloatNumber(fNumbers::fOnePointZero),FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fZero) };
		RaycastHit raycastHit;


		// 接地状態
		if ( onGround )
		{
// スムーズに坂を下る為の吸着距離
			const float adsDistance = FloatNumber(fNumbers::fOnePointTwo);
			// 接地を維持
			if ( CollisionManager::GetInstance()->Raycast(Groundray,COLLISION_ATTR_LANDSHAPE,&raycastHit,Radius * FloatNumber(fNumbers::fTwoPointZero) + adsDistance) )
			{
				onGround = true;
				playerWorldTrans.translation_.y -= ( raycastHit.distance - Radius * FloatNumber(fNumbers::fTwoPointZero) );
			}
			// 地面がないので落下
			else
			{
				onGround = false;
				fallVec = {};
			}
		}
		// 落下状態
		else
		{
			if ( CollisionManager::GetInstance()->Raycast(Groundray,COLLISION_ATTR_LANDSHAPE,&raycastHit,Radius * FloatNumber(fNumbers::fTwoPointZero)) )
			{
// 着地
				onGround = true;
				playerWorldTrans.translation_.y -= ( raycastHit.distance - Radius * FloatNumber(fNumbers::fTwoPointZero) );
			}
		}
	}
	{
		//横メッシュコライダー
		Ray wall;
		wall.start = MyMath::Vec3ToVec4(playerWorldTrans.translation_);
		wall.start.y += Radius;
		wall.dir = { FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fOnePointZero) };
		RaycastHit wallRaycastHit;
		// スムーズに坂を下る為の吸着距離

		// 接地を維持
		if ( CollisionManager::GetInstance()->Raycast(wall,COLLISION_ATTR_LANDSHAPE,&wallRaycastHit,Radius) )
		{
			playerWorldTrans.translation_.z += ( wallRaycastHit.distance - Radius );
		}

	}
	{
		//横メッシュコライダー
		Ray wall;
		wall.start = MyMath::Vec3ToVec4(playerWorldTrans.translation_);
		wall.start.y += Radius;
		wall.dir = { FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fZero),-FloatNumber(fNumbers::fOnePointZero) };
		RaycastHit wallRaycastHit;
		// スムーズに坂を下る為の吸着距離

		// 接地を維持
		if ( CollisionManager::GetInstance()->Raycast(wall,COLLISION_ATTR_LANDSHAPE,&wallRaycastHit,Radius) )
		{
			playerWorldTrans.translation_.z -= ( wallRaycastHit.distance - Radius );
		}
	}
	{
		//横メッシュコライダー
		Ray wall;
		wall.start = MyMath::Vec3ToVec4(playerWorldTrans.translation_);
		wall.start.y += Radius;
		wall.dir = { FloatNumber(fNumbers::fOnePointZero),FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fZero) };
		RaycastHit wallRaycastHit;
		// スムーズに坂を下る為の吸着距離

		// 接地を維持
		if ( CollisionManager::GetInstance()->Raycast(wall,COLLISION_ATTR_LANDSHAPE,&wallRaycastHit,Radius) )
		{
			playerWorldTrans.translation_.x += ( wallRaycastHit.distance - Radius );
		}

	}
	{
		//横メッシュコライダー
		Ray wall;
		wall.start = MyMath::Vec3ToVec4(playerWorldTrans.translation_);
		wall.start.y += Radius;
		wall.dir = { -FloatNumber(fNumbers::fOnePointZero),FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fZero) };
		RaycastHit wallRaycastHit;
		// スムーズに坂を下る為の吸着距離

		// 接地を維持
		if ( CollisionManager::GetInstance()->Raycast(wall,COLLISION_ATTR_LANDSHAPE,&wallRaycastHit,Radius) )
		{
			playerWorldTrans.translation_.x -= ( wallRaycastHit.distance - Radius );
		}

	}

}

void Player::Fall()
{
	if ( isHitRail == false/* || isHitFirstRail == false*/ )
	{
		// 落下処理
		if ( !onGround )
		{
			// 下向き加速度
			// 加速
			fallVec.y = max(fallVec.y + fallAcc,fallVYMin);
			// 移動
			playerWorldTrans.translation_.x += fallVec.x;
			playerWorldTrans.translation_.y += fallVec.y;
			playerWorldTrans.translation_.z += fallVec.z;
		}
	}
}


void Player::DeterminationDirection()
{
	isSliding = true;
	SlidingSpeed = MaxSlidingSpeed;

	if ( playerMovement->GetPushedKey().isPushW == true && playerMovement->GetPushedKey().isPushA == false && playerMovement->GetPushedKey().isPushD == false )
	{
		SlidingNumber = static_cast<uint32_t>(Numbers::One);
	}
	else if ( playerMovement->GetPushedKey().isPushW == false && playerMovement->GetPushedKey().isPushS == false && playerMovement->GetPushedKey().isPushA == true )
	{
		SlidingNumber = static_cast< uint32_t >( Numbers::Two );
	}
	else if ( playerMovement->GetPushedKey().isPushS == true && playerMovement->GetPushedKey().isPushA == false && playerMovement->GetPushedKey().isPushD == false )
	{
		SlidingNumber = static_cast< uint32_t >( Numbers::Three );
	}
	else if ( playerMovement->GetPushedKey().isPushW == false && playerMovement->GetPushedKey().isPushS == false && playerMovement->GetPushedKey().isPushD == true )
	{
		SlidingNumber = static_cast< uint32_t >( Numbers::Four );
	}
	else if ( playerMovement->GetPushedKey().isPushW == true && playerMovement->GetPushedKey().isPushA == true && playerMovement->GetPushedKey().isPushD == false )
	{
		SlidingNumber = static_cast< uint32_t >( Numbers::Five );
	}
	else if ( playerMovement->GetPushedKey().isPushW == true && playerMovement->GetPushedKey().isPushA == false && playerMovement->GetPushedKey().isPushD == true )
	{
		SlidingNumber = static_cast< uint32_t >( Numbers::Six );
	}
	else if ( playerMovement->GetPushedKey().isPushS == true && playerMovement->GetPushedKey().isPushA == true && playerMovement->GetPushedKey().isPushD == false )
	{
		SlidingNumber = static_cast< uint32_t >( Numbers::Seven );
	}
	else if ( playerMovement->GetPushedKey().isPushS == true && playerMovement->GetPushedKey().isPushA == false && playerMovement->GetPushedKey().isPushD == true )
	{
		SlidingNumber = static_cast< uint32_t >( Numbers::Eight );
	}
	else
	{
		SlidingNumber = static_cast< uint32_t >( Numbers::Zero );
	}
}

void Player::SlideBoostUpdate()
{
	if ( isSliding )
	{
		if ( SlidingSpeed > FloatNumber(fNumbers::fZero) )
		{
			if ( SlidingNumber == static_cast< uint32_t >( Numbers::One ) )
			{
				playerWorldTrans.translation_ += playerWorldTrans.LookVelocity.look * SlidingSpeed;
			}
			else if ( SlidingNumber == static_cast< uint32_t >( Numbers::Two ) )
			{
				playerWorldTrans.translation_ += playerWorldTrans.LookVelocity.lookLeft * SlidingSpeed;
			}
			else if ( SlidingNumber == static_cast< uint32_t >( Numbers::Three ) )
			{
				playerWorldTrans.translation_ += playerWorldTrans.LookVelocity.lookBack * SlidingSpeed;
			}
			else if ( SlidingNumber == static_cast< uint32_t >( Numbers::Four ) )
			{
				playerWorldTrans.translation_ += playerWorldTrans.LookVelocity.lookRight * SlidingSpeed;
			}
			else if ( SlidingNumber == static_cast< uint32_t >( Numbers::Five ) )
			{
				playerWorldTrans.translation_ += playerWorldTrans.LookVelocity.look_lookLeft * SlidingSpeed;
			}
			else if ( SlidingNumber == static_cast< uint32_t >( Numbers::Six ) )
			{
				playerWorldTrans.translation_ += playerWorldTrans.LookVelocity.look_lookRight * SlidingSpeed;
			}
			else if ( SlidingNumber == static_cast< uint32_t >( Numbers::Seven ) )
			{
				playerWorldTrans.translation_ += playerWorldTrans.LookVelocity.lookBack_lookLeft * SlidingSpeed;
			}
			else if ( SlidingNumber == static_cast< uint32_t >( Numbers::Eight ) )
			{
				playerWorldTrans.translation_ += playerWorldTrans.LookVelocity.lookBack_lookRight * SlidingSpeed;
			}
			else
			{
				playerWorldTrans.translation_ += playerWorldTrans.LookVelocity.look * SlidingSpeed;
			}
			SlidingSpeed -= MaxSlidingSpeed / playerAnimTime.Step;
		}
		else
		{
			isSliding = false;
		}
	}
}

float Player::AngleSelect(float& angle_,float& selectAngle)
{
	float floatOverAngle = static_cast< float >( angle_ / selectAngle );

	float Angle = angle_ - ( selectAngle * floatOverAngle );

	return Angle;
}

void Player::CheckHitCollision()
{
	if ( PlayerCollider->GetHit() )
	{
		isTakeMissileDamages = true;
		PlayerCollider->Reset();
	}
}

void Player::HPUpdate()
{
	if ( isTakeMissileDamages )
	{
		PlayerHP -= static_cast< float >( BulletDamage::GetInstance()->GetEnemyMissileBulletDamage() );
		playerUI->PlayerHpUpdate(static_cast< uint32_t >( PlayerHP ),static_cast< uint32_t >( PlayerMaxHP ));
		isTakeMissileDamages = false;
		if ( PlayerHP > FloatNumber(fNumbers::fZero) )
		{

		}
		else
		{
			PlayerHP = FloatNumber(fNumbers::fZero);//0固定
			playerUI->PlayerHpUpdate(static_cast< uint32_t >( PlayerHP ),static_cast< uint32_t >( PlayerMaxHP ));
			isAlive = false;
		}
	}
}

void Player::SetCameraNeedInformation(const Vector2& CameraRot,const Vector3& targetPosition,const float& distance,const float& cameraMaxDistance)
{
	cameraRot = CameraRot;
	TargetPosition = targetPosition;
	PlayerToCameraDistance = distance;
	CameraMaxDistance = cameraMaxDistance;
}

