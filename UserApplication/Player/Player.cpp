#include "Player.h"
#include "WinApp.h"
#include "MyMath.h"
#include "CollisionManager.h"
#include"ImGuiManager.h"
//#include <FbxLoader.h>
#include <SphereCollider.h>
#include "BulletDamage.h"
#include "CollisionAttribute.h"

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
	playerWorldTrans.scale_ = { 1,1,1 };
	playerWorldTrans.TransferMatrix();

	viewProjection_ = viewProjection;

	playerWorldTransForBullet.Initialize();
	StartingPointOfGrapple.Initialize();

	playerNormalGun = std::make_unique<NormalGun>(COLLISION_ATTR_ATTACK);
	playerNormalGun->Initialize(Pos,model_.get(),model_.get());

	DebugWorldTrans.Initialize();

	AttackSprite = Sprite::Create(TextureManager::Load("sprite/shoujuun.png"));
	AttackSprite->SetAnchorPoint({ 0.5f,0.5f });

	// コリジョンマネージャに追加
	PlayerCollider = new SphereCollider(Vector4(0,Radius,0,0),Radius);
	CollisionManager::GetInstance()->AddCollider(PlayerCollider);
	PlayerCollider->SetAttribute(COLLISION_ATTR_ALLIES);
	PlayerCollider->Update(playerWorldTrans.matWorld_);

	playerMoveSpline = std::make_unique<SplinePosition>();
	FirstMoveSpline = std::make_unique<SplinePosition>();
	FinalMoveSpline = std::make_unique<SplinePosition>();

	playerUI = std::make_unique<PlayerUI>();
	playerUI->Initialize();

	playerUI->PlayerHpUpdate(static_cast< uint32_t >( PlayerHP ),static_cast< uint32_t >( PlayerMaxHP ));

	animation = std::make_unique<Animation>();
	animation->Initialize();
}

void Player::Update()
{
	isAttack = false;
	isPressing = false;
	isPushW = false;
	isPushA = false;
	isPushS = false;
	isPushD = false;

	if ( playerMoveSpline->GetFinishSpline() )
	{
		isHitRail = false;
	}
	if ( FirstMoveSpline->GetFinishSpline() )
	{
		isHitFirstRail = false;
	}

	//当たり判定チェック
	CheckHitCollision();
	//HPのアップデート
	HPUpdate();

	//回転させる
	PlayerRot();

	//どう動くか
	Move();

	//落下
	Fall();
	//当たり判定チェック
	CheckPlayerCollider();

	//スプラインアップデート
	SplineUpdate();

	if ( isCameraModeNotFree == true )
	{
		//カメラの位置でアルファが決まる
		Alpha = 1.0f - ( 1.0f - PlayerToCameraDistance / CameraMaxDistance );
		playerWorldTrans.alpha = Alpha;
	}
	if ( FinalMoveSpline->GetHowReturnIndex(5) )
	{
		isStop = true;
	}

	if ( input_->TriggerKey(DIK_LSHIFT) )
	{
		if ( animation->GetNowAnimFinish() )
		{
			animation->SetAnimation(static_cast< uint32_t >( PlayerAnimation::Step ),0,playerAnimTime.Step,false);
			DeterminationDirection();
		}
	}

	SlideBoostUpdate();

	//移動の値更新
	WorldTransUpdate();

	isGrapple = false;
	if ( input_->MouseInputing(1) )
	{

	}

	if ( input_->MouseInputing(0) )
	{
		isAttack = true;
	}


	if ( input_->MouseInputTrigger(0) )
	{
		/*LeftBoneNum++;*/
	}
	if ( input_->MouseInputTrigger(1) )
	{
		/*LeftBoneNum--;*/
	}

	ImGui::Begin("Player");

	ImGui::Text("%d",LeftBoneNum);

	ImGui::End();

	//uiのアプデ
	playerUI->Update();

	if ( input_->TriggerKey(DIK_F5) )
	{
		isAlive = true;
		PlayerHP = 1000;
		playerUI->PlayerHpUpdate(static_cast< uint32_t >( PlayerHP ),static_cast< uint32_t >( PlayerMaxHP ));
	}

	if ( isAlive == false )
	{
		if ( isDieMotion == false )
		{
			isDieMotion = true;
			animation->SetAnimation(static_cast< uint32_t >( PlayerAnimation::LeftDown ),0,playerAnimTime.DieMotion,false);
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
	AttackSprite->Draw(Vector2(640,360),Vector4(1,1,1,1),2);

	playerUI->Draw();

}

void Player::AttackUpdate(const Vector3& EnemyPos,bool& LockOn)
{
	if ( isAttack == true )
	{
		PlayerAttack(EnemyPos,LockOn);
	}

	playerUI->AttackReticleUpdate(LockOn);
}

void Player::Move()
{
	playerMoveMent = { 0.0f,0.0f,0.0f };

	if ( isHitRail == false || playerMoveSpline->GetFinishSpline() == true )
	{
		if ( input_->PushKey(DIK_W) )
		{
			isPushW = true;
			playerMoveMent += playerWorldTrans.LookVelocity.look.norm() * playerSpeed;
		}
		if ( input_->PushKey(DIK_S) )
		{
			isPushS = true;
			playerMoveMent += playerWorldTrans.LookVelocity.lookBack.norm() * playerSpeed;
			//playerMoveMent.y -= 0.02f;
		}
		if ( input_->PushKey(DIK_A) )
		{
			isPushA = true;
			playerMoveMent += playerWorldTrans.LookVelocity.lookLeft.norm() * playerSpeed;
		}
		if ( input_->PushKey(DIK_D) )
		{
			isPushD = true;
			playerMoveMent += playerWorldTrans.LookVelocity.lookRight.norm() * playerSpeed;
		}

		if ( input_->PushKey(DIK_W) == 1 && input_->PushKey(DIK_A) == 1 )
		{
			playerMoveMent = { 0.0f,0.0f,0.0f };
			playerMoveMent += playerWorldTrans.LookVelocity.look_lookLeft.norm() * playerSpeed;
		}
		if ( input_->PushKey(DIK_W) == 1 && input_->PushKey(DIK_D) == 1 )
		{
			playerMoveMent = { 0.0f,0.0f,0.0f };
			playerMoveMent += playerWorldTrans.LookVelocity.look_lookRight.norm() * playerSpeed;
		}
		if ( input_->PushKey(DIK_S) == 1 && input_->PushKey(DIK_A) == 1 )
		{
			playerMoveMent = { 0.0f,0.0f,0.0f };
			playerMoveMent += playerWorldTrans.LookVelocity.lookBack_lookLeft.norm() * playerSpeed;
		}
		if ( input_->PushKey(DIK_S) == 1 && input_->PushKey(DIK_D) == 1 )
		{
			playerMoveMent = { 0.0f,0.0f,0.0f };
			playerMoveMent += playerWorldTrans.LookVelocity.lookBack_lookRight.norm() * playerSpeed;
		}
		//if (onGround) {
		if ( input_->TriggerKey(DIK_SPACE) )
		{
			playerMoveSpline->ResetNearSplineReset();
			FirstMoveSpline->ResetNearSplineReset();
			onGround = false;
			const float jumpVYFist = 0.6f;
			float SlidingJump = 0.0f;

			fallVec = { 0, jumpVYFist + SlidingJump, 0, 0 };

		}
		//}

	}



	playerWorldTrans.translation_ += playerMoveMent;

}

void Player::PlayerRot()
{

	playerWorldTrans.SetRot(Vector3(0.0f,cameraRot.x,0.0f));
	playerWorldTransForBullet.SetRot(Vector3(cameraRot.y,cameraRot.x,0.0f));
	//値更新
	WorldTransUpdate();
}

void Player::PlayerAttack(const Vector3& EnemyPos,bool& LockOn)
{

	if ( LockOn )
	{
		playerNormalGun->ShotBullet(EnemyPos);
	}
	else
	{
		playerNormalGun->ShotBullet(TargetPosition);
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
	playerWorldTransForBullet.TransferMatrix();
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
		Groundray.dir = { 0,-1.0f,0,0 };
		RaycastHit raycastHit;


		// 接地状態
		if ( onGround )
		{
// スムーズに坂を下る為の吸着距離
			const float adsDistance = 0.2f;
			// 接地を維持
			if ( CollisionManager::GetInstance()->Raycast(Groundray,COLLISION_ATTR_LANDSHAPE,&raycastHit,Radius * 2.0f + adsDistance) )
			{
				onGround = true;
				playerWorldTrans.translation_.y -= ( raycastHit.distance - Radius * 2.0f );
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
			if ( CollisionManager::GetInstance()->Raycast(Groundray,COLLISION_ATTR_LANDSHAPE,&raycastHit,Radius * 2.0f) )
			{
// 着地
				onGround = true;
				playerWorldTrans.translation_.y -= ( raycastHit.distance - Radius * 2.0f );
			}
		}
	}
	{
		//横メッシュコライダー
		Ray wall;
		wall.start = MyMath::Vec3ToVec4(playerWorldTrans.translation_);
		wall.start.y += Radius;
		wall.dir = { 0,0,1,0 };
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
		wall.dir = { 0,0,-1,0 };
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
		wall.dir = { 1,0,0,0 };
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
		wall.dir = { -1,0,0,0 };
		RaycastHit wallRaycastHit;
		// スムーズに坂を下る為の吸着距離

		// 接地を維持
		if ( CollisionManager::GetInstance()->Raycast(wall,COLLISION_ATTR_LANDSHAPE,&wallRaycastHit,Radius) )
		{
			playerWorldTrans.translation_.x -= ( wallRaycastHit.distance - Radius );
		}

	}

	//レールコライダー
	if ( PlayerCollider->GetFirstSplineHit() )
	{
		PlayerCollider->FirstSplineHitReset();
		Vector3 splinePos = playerWorldTrans.translation_ - Vector3(0,Radius,0);
		FirstMoveSpline->ResetNearSpline(splinePos);
		isHitFirstRail = true;
	}
	//レールコライダー
	if ( isHitRail == false )
	{
		if ( PlayerCollider->GetSphereMeshHit() )
		{
			PlayerCollider->SphereMeshHitReset();
			Vector3 splinePos = playerWorldTrans.translation_ - Vector3(0,Radius,0);
			playerMoveSpline->ResetNearSpline(splinePos);
			isHitRail = true;
		}
	}
	//レールコライダー
	if ( PlayerCollider->GetFinalSplineHit() )
	{
		PlayerCollider->FinalSplineHitReset();
		Vector3 splinePos = playerWorldTrans.translation_ - Vector3(0,Radius,0);
		FirstMoveSpline->ResetNearSpline(splinePos);
		isHitFinalRail = true;
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
			const float fallAcc = -0.035f;
			const float fallVYMin = -1.3f;
			// 加速
			fallVec.y = max(fallVec.y + fallAcc,fallVYMin);
			// 移動
			playerWorldTrans.translation_.x += fallVec.x;
			playerWorldTrans.translation_.y += fallVec.y;
			playerWorldTrans.translation_.z += fallVec.z;
		}
	}
}

void Player::SplineUpdate()
{
	if ( isHitRail == true && playerMoveSpline->GetFinishSpline() == false )
	{
		float speed = 0.02f;
		playerMoveSpline->Update(speed);
		playerWorldTrans.translation_ = playerMoveSpline->NowPos + Vector3(0,Radius,0);
	}
	else if ( playerMoveSpline->GetFinishSpline() )
	{
		isHitRail = false;
	}
	if ( isHitFirstRail == true && FirstMoveSpline->GetFinishSpline() == false )
	{
		float speed = 0.02f;
		FirstMoveSpline->Update(speed);
		playerWorldTrans.translation_ = FirstMoveSpline->NowPos + Vector3(0,Radius,0);
	}
	else if ( FirstMoveSpline->GetFinishSpline() )
	{
		isHitFirstRail = false;
	}
	if ( isHitFinalRail == true && FinalMoveSpline->GetFinishSpline() == false )
	{
		if ( isStop == false )
		{
			float speed = 0.02f;
			FinalMoveSpline->Update(speed);
			playerWorldTrans.translation_ = FinalMoveSpline->NowPos + Vector3(0,Radius,0);
		}
	}
	else if ( FinalMoveSpline->GetFinishSpline() )
	{
		isHitFinalRail = false;
	}
}

void Player::DeterminationDirection()
{
	isSliding = true;
	SlidingSpeed = MaxSlidingSpeed;

	if ( isPushW == 1 && isPushA == 0 && isPushD == 0 )
	{
		SlidingNumber = 1;
	}
	else if ( isPushW == 0 && isPushS == 0 && isPushA == 1 )
	{
		SlidingNumber = 2;
	}
	else if ( isPushS == 1 && isPushA == 0 && isPushD == 0 )
	{
		SlidingNumber = 3;
	}
	else if ( isPushW == 0 && isPushS == 0 && isPushD == 1 )
	{
		SlidingNumber = 4;
	}
	else if ( isPushW == 1 && isPushA == 1 && isPushD == 0 )
	{
		SlidingNumber = 5;
	}
	else if ( isPushW == 1 && isPushA == 0 && isPushD == 1 )
	{
		SlidingNumber = 6;
	}
	else if ( isPushS == 1 && isPushA == 1 && isPushD == 0 )
	{
		SlidingNumber = 7;
	}
	else if ( isPushS == 1 && isPushA == 0 && isPushD == 1 )
	{
		SlidingNumber = 8;
	}
	else
	{
		SlidingNumber = 0;
	}
}

void Player::SlideBoostUpdate()
{
	if ( isSliding )
	{
		if ( SlidingSpeed > 0.0f )
		{
			if ( SlidingNumber == 1 )
			{
				playerWorldTrans.translation_ += playerWorldTrans.LookVelocity.look * SlidingSpeed;
			}
			else if ( SlidingNumber == 2 )
			{
				playerWorldTrans.translation_ += playerWorldTrans.LookVelocity.lookLeft * SlidingSpeed;
			}
			else if ( SlidingNumber == 3 )
			{
				playerWorldTrans.translation_ += playerWorldTrans.LookVelocity.lookBack * SlidingSpeed;
			}
			else if ( SlidingNumber == 4 )
			{
				playerWorldTrans.translation_ += playerWorldTrans.LookVelocity.lookRight * SlidingSpeed;
			}
			else if ( SlidingNumber == 5 )
			{
				playerWorldTrans.translation_ += playerWorldTrans.LookVelocity.look_lookLeft * SlidingSpeed;
			}
			else if ( SlidingNumber == 6 )
			{
				playerWorldTrans.translation_ += playerWorldTrans.LookVelocity.look_lookRight * SlidingSpeed;
			}
			else if ( SlidingNumber == 7 )
			{
				playerWorldTrans.translation_ += playerWorldTrans.LookVelocity.lookBack_lookLeft * SlidingSpeed;
			}
			else if ( SlidingNumber == 8 )
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
	//float sprt = std::sqrt(angle * angle);
	int intOverAngle = static_cast< int >( angle_ / selectAngle );

	float floatOverAngle = static_cast< float >( intOverAngle );

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
		if ( PlayerHP > 0.0f )
		{

		}
		else
		{
			PlayerHP = 0.0f;//0固定
			playerUI->PlayerHpUpdate(static_cast< uint32_t >(PlayerHP),static_cast< uint32_t >(PlayerMaxHP));
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

