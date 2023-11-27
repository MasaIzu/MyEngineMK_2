#include "Player.h"
#include "WinApp.h"
#include "MyMath.h"
#include "CollisionManager.h"
#include"ImGuiManager.h"
//#include <FbxLoader.h>
#include <SphereCollider.h>
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

	playerRotWorldTrans.Initialize();

	viewProjection_ = viewProjection;

	playerNormalGun = std::make_unique<NormalGun>(COLLISION_ATTR_ATTACK);
	playerNormalGun->Initialize(Pos,model_.get());

	DebugWorldTrans.Initialize();
	DebugWorldTrans.scale_ = Vector3(PlayerBladeRadius,PlayerBladeRadius,PlayerBladeRadius);

	playerMovement = std::make_unique<PlayerMovement>();

	// コリジョンマネージャに追加
	float sphereF = FloatNumber(fNumbers::fZero);
	PlayerCollider = new SphereCollider(Vector4(sphereF,PlayerRadius,sphereF,sphereF),PlayerRadius);
	CollisionManager::GetInstance()->AddCollider(PlayerCollider);
	PlayerCollider->SetAttribute(COLLISION_ATTR_ALLIES);
	PlayerCollider->Update(playerWorldTrans.matWorld_);

	for ( uint32_t i = 0; i < AttackColSphereCount; i++ )
	{
		BladeColWorldTrans[ i ].scale_ = Vector3(PlayerBladeRadius,PlayerBladeRadius,PlayerBladeRadius);
		BladeColWorldTrans[ i ].Initialize();
		PlayerBladeAttackCollider[ i ] = new SphereCollider(Vector4(sphereF,PlayerBladeRadius,sphereF,sphereF),PlayerBladeRadius);
		CollisionManager::GetInstance()->AddCollider(PlayerBladeAttackCollider[ i ]);
		PlayerBladeAttackCollider[ i ]->SetAttribute(COLLISION_ATTR_NOTATTACK);
		PlayerBladeAttackCollider[ i ]->Update(playerWorldTrans.matWorld_);
	}

	playerUI = std::make_unique<PlayerUI>();
	playerUI->Initialize(playerMovement->GetFuel());

	playerUI->PlayerHpUpdate(PlayerHP,PlayerMaxHP);

	animation = std::make_unique<Animation>();
	animation->Initialize();

	ParticleHanabi = std::make_unique<ParticleHandHanabi>();
	int MaxParticleCountA = 20000;
	ParticleHanabi->Initialize(MaxParticleCountA);
	ParticleHanabi->SetTextureHandle(TextureManager::Load("sprite/effect4.png"));

	ParticleBooster = std::make_unique<ParticleBoost>();
	int MaxParticleCount = 15000;
	ParticleBooster->Initialize(MaxParticleCount);
	ParticleBooster->SetTextureHandle(TextureManager::Load("sprite/effect4.png"));



	DamageUI = std::make_unique<PlayerDamageHitUI>();
	DamageUI->Initialize();
}

void Player::Update()
{
	isAttack = false;
	isPressing = false;
	isBladeAttack = false;

	Moved = playerWorldTrans.translation_;

	//攻撃
	if ( input_->MouseInputing(static_cast< int >( Numbers::Zero )) )
	{
		isAttack = true;
	}
	if ( isBladeAttacking == false )
	{
		if ( input_->MouseInputTrigger(static_cast< int >( Numbers::One )) )
		{
			isBladeAttack = true;
		}
	}

	//当たり判定チェック
	CheckHitCollision();
	//HPのアップデート
	HPUpdate();

	//回転させる
	PlayerRot(isAttack,isBladeAttacking);

	playerWorldTrans.translation_ += playerMovement->Move(playerWorldTrans,onGround,isBladeAttacking);


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
			DeterminationDirection();
		}
	}

	SlideBoostUpdate();

	//移動の値更新
	WorldTransUpdate();

	ImGui::Begin("Player");

	ImGui::Text("%d",LeftBoneNum);
	ImGui::Text("PlayerMoveRotation = %f",playerMovement->GetPlayerAngle());

	ImGui::End();

	//uiのアプデ
	playerUI->Update(playerMovement->GetFuel());

	if ( input_->TriggerKey(DIK_F5) )
	{
		isAlive = true;
		PlayerHP = PlayerMaxHP;
		playerUI->PlayerHpUpdate(PlayerHP,PlayerMaxHP);
	}

	if ( isAlive == false )
	{
		if ( isDieMotion == false )
		{
			isDieMotion = true;
			animation->SetAnimation(static_cast< uint32_t >( PlayerAnimation::LeftDown ),static_cast< uint32_t >( Numbers::Zero ),playerAnimTime.DieMotion,false);
		}
	}

	playerRotWorldTrans.translation_ = playerWorldTrans.translation_;
	playerRotWorldTrans.TransferMatrix();
	animation->Update();
	playerNormalGun->Update(MyMath::GetWorldTransform(animation->GetBonePos(LeftBoneNum) * playerRotWorldTrans.matWorld_),RotKeep);

	ParticleStartPos = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(animation->GetBonePos(RightBoneNum) * playerRotWorldTrans.matWorld_));
	ParticleEndPos = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(animation->GetBonePos(BladeAttackEndPos) * playerRotWorldTrans.matWorld_));

	BoostStartPos = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(animation->GetBonePos(kataLeftBoneNumber) * playerRotWorldTrans.matWorld_));

	BladeColRatio = MyMath::Vec4ToVec3(ParticleEndPos) - MyMath::Vec4ToVec3(ParticleStartPos);
	BladeColRatio = ( BladeColRatio.norm() * MaxBladeColDetection ) / AttackColSphereCount;

	DebugWorldTrans.translation_ = MyMath::GetWorldTransform(animation->GetBonePos(RightBoneNum) * playerRotWorldTrans.matWorld_);
	DebugWorldTrans.TransferMatrix();

	for ( uint32_t i = 0; i < AttackColSphereCount; i++ )
	{
		BladeColWorldTrans[ i ].translation_ = MyMath::Vec4ToVec3(ParticleStartPos) + ( BladeColRatio * static_cast< float >( i ) );
		BladeColWorldTrans[ i ].TransferMatrix();
	}

	bonePos.BoostStartPos[ 0 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(animation->GetBonePos(RightStart) * playerRotWorldTrans.matWorld_));
	bonePos.BoostEndPos[ 0 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(animation->GetBonePos(RightEnd) * playerRotWorldTrans.matWorld_));
	bonePos.BoostStartPos[ 1 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(animation->GetBonePos(CenterRightStart) * playerRotWorldTrans.matWorld_));
	bonePos.BoostEndPos[ 1 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(animation->GetBonePos(CenterRightEnd) * playerRotWorldTrans.matWorld_));
	bonePos.BoostStartPos[ 2 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(animation->GetBonePos(CenterLeftStart) * playerRotWorldTrans.matWorld_));
	bonePos.BoostEndPos[ 2 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(animation->GetBonePos(CenterLeftEnd) * playerRotWorldTrans.matWorld_));
	bonePos.BoostStartPos[ 3 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(animation->GetBonePos(LeftStart) * playerRotWorldTrans.matWorld_));
	bonePos.BoostEndPos[ 3 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(animation->GetBonePos(LeftEnd) * playerRotWorldTrans.matWorld_));

	DamageUI->Update();
}

void Player::Draw()
{
	//for ( uint32_t i = 0; i < AttackColSphereCount; i++ )
	//{
	//	model_->Draw(BladeColWorldTrans[ i ],*viewProjection_);
	//}

	//model_->Draw(DebugWorldTrans,*viewProjection_);

	playerNormalGun->Draw(*viewProjection_);
}

void Player::FbxDraw() {
	playerNormalGun->FbxDraw(*viewProjection_);
	animation->FbxDraw(playerRotWorldTrans,*viewProjection_);
}


void Player::DrawSprite()
{
	playerUI->Draw();
	DamageUI->Draw();
}

void Player::AttackUpdate(const Vector3& EnemyPos,bool& LockOn)
{
	if ( isBladeAttacking == true )
	{
		if ( isPreparation == false )
		{
			if ( BladeAttackTime < BladeMaxAttackTime )
			{
				BladeAttackTime++;
				if ( EnemyPos.y < MyMath::GetWorldTransform(animation->GetBonePos(static_cast< uint32_t >( Numbers::Three )) * playerWorldTrans.matWorld_).y )
				{
					BladeAttackVelocity.y = FloatNumber(fNumbers::fZero);
				}
				playerWorldTrans.translation_ += BladeAttackVelocity;
				animation->SetKeepAnimation(static_cast< uint32_t >( PlayerAnimation::HandAttack ),static_cast< uint32_t >( Numbers::Ten ),playerAnimTime.BladeAttack);
				if ( input_->MouseInputTrigger(static_cast< int >( Numbers::One )) )
				{
					BladeAttackTime = BladeMaxAttackTime;
					animation->SetAnimation(static_cast< uint32_t >( PlayerAnimation::HandAttack ),static_cast< uint32_t >( Numbers::Ten ),playerAnimTime.BladeAttack,false);
				}
			}
			else
			{
				isPreparation = true;
				BladeAttackVelocity.y = FloatNumber(fNumbers::fZero);
				animation->SetAnimation(static_cast< uint32_t >( PlayerAnimation::HandAttack ),static_cast< uint32_t >( Numbers::Ten ),playerAnimTime.BladeAttack,false);
				BladeAttributeSet(COLLISION_ATTR_MELEEATTACK);
			}
		}
		else
		{
			if ( animation->GetAnimAlmostOver(BladeColEndHasten) )
			{
				isBladeAttacking = false;
				BladeAttributeSet(COLLISION_ATTR_NOTATTACK);
				CollisionManager::GetInstance()->ResetMeleeAttack();
			}
			if ( LockOn )
			{
				FixedAngle = MyMath::Get2VecAngle(MyMath::GetWorldTransform(animation->GetBonePos(static_cast< uint32_t >( Numbers::Three )) * playerRotWorldTrans.matWorld_),EnemyPos);
				BladeAttackVelocity = EnemyPos - playerWorldTrans.translation_;
				playerWorldTrans.translation_ += BladeAttackVelocity.norm() * BladeAttackSpeed;
			}
			else
			{
				FixedAngle = MyMath::Get2VecAngle(playerWorldTrans.translation_ + playerWorldTrans.LookVelocity.look,TargetPosition);
				BladeAttackVelocity = playerRotWorldTrans.LookVelocity.look;
				playerWorldTrans.translation_ += BladeAttackVelocity.norm() * BladeAttackSpeed;
			}
		}
	}


	if ( isAttack == true )
	{
		PlayerAttack(EnemyPos,LockOn);
	}
	else if ( isBladeAttack == true )
	{
		PlayerBladeAttack(EnemyPos,LockOn);
	}

	playerUI->AttackReticleUpdate(LockOn);
	WorldTransUpdate();

	//当たり判定チェック
	CheckPlayerCollider();

	ImGui::Begin("PlayerLockOn");

	ImGui::Text("Moved = %f,%f,%f",Moved.x,Moved.y,Moved.z);
	ImGui::Text("playerWorldTrans = %f,%f,%f",playerWorldTrans.translation_.x,playerWorldTrans.translation_.y,playerWorldTrans.translation_.z);
	ImGui::End();

	Moved = playerWorldTrans.translation_ - Moved;

}

void Player::CSUpdate(ID3D12GraphicsCommandList* cmdList)
{
	ParticleHanabi->CSUpdate(cmdList,ParticleStartPos,ParticleEndPos,static_cast< uint32_t >( isBladeAttacking ));
	ParticleBooster->CSUpdate(cmdList,bonePos,playerMovement->GetBoostPower(isBladeAttacking),playerMovement->GetPushBoostKey(isBladeAttacking));
}

void Player::ParticleDraw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = DirectXCore::GetInstance()->GetCommandList();
	ParticleHandHanabi::PreDraw(commandList);
	ParticleHanabi->Draw(*viewProjection_);
	ParticleHandHanabi::PostDraw();

	ParticleBoost::PreDraw(commandList);
	ParticleBooster->Draw(*viewProjection_);
	ParticleBoost::PostDraw();
}

void Player::PlayerRot(const bool& Attack,const bool& BladeAttack)
{
	playerMovement->PlayerAngle(Attack,BladeAttack);
	RotKeep = Vec3Number(fNumbers::fZero);
	if ( !Attack && !BladeAttack )
	{
		RotKeep = Vector3(FloatNumber(fNumbers::fZero),cameraRot.x + MyMath::GetAngle(playerMovement->GetPlayerAngle()),FloatNumber(fNumbers::fZero));
		playerRotWorldTrans.SetRot(RotKeep);
		playerWorldTrans.SetRot(Vector3(FloatNumber(fNumbers::fZero),cameraRot.x + MyMath::GetAngle(playerMovement->GetPlayerAngle()),FloatNumber(fNumbers::fZero)));
	}
	else
	{
		RotKeep = Vector3(FloatNumber(fNumbers::fZero),MyMath::GetAngle(FixedAngle) + MyMath::GetAngle(playerMovement->GetPlayerAngle()),FloatNumber(fNumbers::fZero));
		playerRotWorldTrans.SetRot(RotKeep);
		playerWorldTrans.SetRot(Vector3(FloatNumber(fNumbers::fZero),cameraRot.x,FloatNumber(fNumbers::fZero)));
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
}

void Player::PlayerBladeAttack(const Vector3& EnemyPos,bool& LockOn)
{
	isBladeAttacking = true;
	isPreparation = false;
	BladeAttackTime = static_cast< uint32_t >( Numbers::Zero );
	if ( LockOn )
	{
		FixedAngle = MyMath::Get2VecAngle(MyMath::GetWorldTransform(animation->GetBonePos(static_cast< uint32_t >( Numbers::Three )) * playerRotWorldTrans.matWorld_),EnemyPos);
		BladeAttackVelocity = EnemyPos - playerWorldTrans.translation_;
		BladeAttackVelocity = BladeAttackVelocity.norm() * BladeAttackBoostSpeed;
	}
	else
	{
		FixedAngle = MyMath::Get2VecAngle(playerWorldTrans.translation_ + playerWorldTrans.LookVelocity.look,TargetPosition);
		BladeAttackVelocity = playerRotWorldTrans.LookVelocity.look;
		BladeAttackVelocity = BladeAttackVelocity.norm() * BladeAttackBoostSpeed;
	}
}

void Player::WorldTransUpdate()
{
	playerWorldTrans.TransferMatrix();
}

void Player::CheckPlayerCollider()
{
	PlayerCollider->Update(animation->GetBonePos(static_cast< uint32_t >( Numbers::Three )) * playerWorldTrans.matWorld_);
	for ( uint32_t i = 0; i < AttackColSphereCount; i++ )
	{
		PlayerBladeAttackCollider[ i ]->Update(BladeColWorldTrans[ i ].matWorld_);
	}
	Vector3 playerPos = playerWorldTrans.translation_;
	//地面メッシュコライダー
	{
		// 球の上端から球の下端までのレイキャスト
		Ray Groundray;
		Groundray.start = MyMath::Vec3ToVec4(playerPos);
		Groundray.start.y += FlontRadius * FloatNumber(fNumbers::fTwoPointZero);
		Groundray.dir = { FloatNumber(fNumbers::fZero),-FloatNumber(fNumbers::fOnePointZero),FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fZero) };
		RaycastHit raycastHit;

		if ( !playerMovement->GetIsBoost() && !isBladeAttacking )
		{
			// 接地状態
			if ( onGround )
			{
					// スムーズに坂を下る為の吸着距離
				const float adsDistance = FloatNumber(fNumbers::fOnePointTwo);
				// 接地を維持
				if ( CollisionManager::GetInstance()->Raycast(Groundray,COLLISION_ATTR_LANDSHAPE,&raycastHit,FlontRadius * FloatNumber(fNumbers::fTwoPointZero) + adsDistance) )
				{
					onGround = true;
					playerWorldTrans.translation_.y -= ( raycastHit.distance - FlontRadius * FloatNumber(fNumbers::fTwoPointZero) );
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
				if ( CollisionManager::GetInstance()->Raycast(Groundray,COLLISION_ATTR_LANDSHAPE,&raycastHit,FlontRadius * FloatNumber(fNumbers::fTwoPointZero)) )
				{
					// 着地
					onGround = true;
					playerWorldTrans.translation_.y -= ( raycastHit.distance - FlontRadius * FloatNumber(fNumbers::fTwoPointZero) );
				}
			}
		}
	}
	{
		//横メッシュコライダー
		Ray wall;
		Vector3 startPos = playerPos - Vector3(FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fZero),PlayerRadius);
		wall.start = MyMath::Vec3ToVec4(startPos);
		wall.start.y += FlontRadius * FloatNumber(fNumbers::fTwoPointZero);
		wall.dir = { FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fOnePointOne),FloatNumber(fNumbers::fZero) };
		RaycastHit wallRaycastHit;
		// スムーズに坂を下る為の吸着距離

		// 接地を維持
		if ( CollisionManager::GetInstance()->Raycast(wall,COLLISION_ATTR_LANDSHAPE,&wallRaycastHit,PlayerRadius * FloatNumber(fNumbers::fTwoPointZero)) )
		{
			playerWorldTrans.translation_.z += ( wallRaycastHit.distance - PlayerRadius * FloatNumber(fNumbers::fTwoPointZero) );
		}

	}
	{
		//横メッシュコライダー
		Ray wall;
		Vector3 startPos = playerPos - Vector3(FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fZero),-PlayerRadius);
		wall.start = MyMath::Vec3ToVec4(startPos);
		wall.start.y += FlontRadius * FloatNumber(fNumbers::fTwoPointZero);
		wall.dir = { FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fZero),-FloatNumber(fNumbers::fOnePointOne),FloatNumber(fNumbers::fZero) };
		RaycastHit wallRaycastHit;
		// スムーズに坂を下る為の吸着距離

		// 接地を維持
		if ( CollisionManager::GetInstance()->Raycast(wall,COLLISION_ATTR_LANDSHAPE,&wallRaycastHit,PlayerRadius * FloatNumber(fNumbers::fTwoPointZero)) )
		{
			playerWorldTrans.translation_.z -= ( wallRaycastHit.distance - PlayerRadius * FloatNumber(fNumbers::fTwoPointZero) );
		}
	}
	{
		//横メッシュコライダー
		Ray wall;
		Vector3 startPos = playerPos - Vector3(PlayerRadius,FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fZero));
		wall.start = MyMath::Vec3ToVec4(startPos);
		wall.start.y += FlontRadius * FloatNumber(fNumbers::fTwoPointZero);
		wall.dir = { FloatNumber(fNumbers::fOnePointOne),FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fZero) };
		RaycastHit wallRaycastHit;
		// スムーズに坂を下る為の吸着距離

		// 接地を維持
		if ( CollisionManager::GetInstance()->Raycast(wall,COLLISION_ATTR_LANDSHAPE,&wallRaycastHit,PlayerRadius * FloatNumber(fNumbers::fTwoPointZero)) )
		{
			playerWorldTrans.translation_.x += ( wallRaycastHit.distance - PlayerRadius * FloatNumber(fNumbers::fTwoPointZero) );
		}

	}
	{
		//横メッシュコライダー
		Ray wall;
		Vector3 startPos = playerPos - Vector3(-PlayerRadius,FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fZero));
		wall.start = MyMath::Vec3ToVec4(startPos);
		wall.start.y += FlontRadius * FloatNumber(fNumbers::fTwoPointZero);
		wall.dir = { -FloatNumber(fNumbers::fOnePointOne),FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fZero) };
		RaycastHit wallRaycastHit;
		// スムーズに坂を下る為の吸着距離

		// 接地を維持
		if ( CollisionManager::GetInstance()->Raycast(wall,COLLISION_ATTR_LANDSHAPE,&wallRaycastHit,PlayerRadius * FloatNumber(fNumbers::fTwoPointZero)) )
		{
			playerWorldTrans.translation_.x -= ( wallRaycastHit.distance - PlayerRadius * FloatNumber(fNumbers::fTwoPointZero) );
		}

	}
	WorldTransUpdate();
}


void Player::DeterminationDirection()
{
	if ( playerMovement->SlidingMaterial(playerWorldTrans) )
	{
		isSliding = true;
		SlidingSpeed = MaxSlidingSpeed;
		animation->SetAnimation(static_cast< uint32_t >( PlayerAnimation::Step ),static_cast< uint32_t >( Numbers::Zero ),playerAnimTime.Step,false);
	}
}

void Player::SlideBoostUpdate()
{
	if ( isSliding )
	{
		if ( SlidingSpeed > FloatNumber(fNumbers::fZero) )
		{
			playerWorldTrans.translation_ += playerMovement->SlidingUpdate(SlidingSpeed,MaxSlidingSpeed,playerAnimTime.Step);
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
		DamageUI->MakeNoise();
	}
	if ( PlayerCollider->GetHitSphere() )
	{
		playerWorldTrans.translation_ += PlayerCollider->GetRejectVec();
		PlayerCollider->ResetSphere();
	}

}

void Player::HPUpdate()
{
	if ( isTakeMissileDamages )
	{
		PlayerHP -= Damege.MissileAttack;
		playerUI->PlayerHpUpdate(PlayerHP,PlayerMaxHP);
		isTakeMissileDamages = false;
		if ( PlayerHP > FloatNumber(fNumbers::fZero) )
		{

		}
		else
		{
			PlayerHP = FloatNumber(fNumbers::fZero);//0固定
			playerUI->PlayerHpUpdate(PlayerHP,PlayerMaxHP);
			isAlive = false;
		}
	}
}

void Player::BladeAttributeSet(const unsigned short Attribute_)
{
	for ( auto&& col : PlayerBladeAttackCollider )
	{
		col->SetAttribute(Attribute_);
	}
}

void Player::SetCameraNeedInformation(const Vector2& CameraRot,const Vector3& targetPosition,const float& distance,const float& cameraMaxDistance)
{
	cameraRot = CameraRot;
	TargetPosition = targetPosition;
	PlayerToCameraDistance = distance;
	CameraMaxDistance = cameraMaxDistance;
}

Vector3 Player::GetPlayerPos() const
{
	return MyMath::GetWorldTransform(animation->GetBonePos(static_cast< uint32_t >( Numbers::Three )) * playerRotWorldTrans.matWorld_);
}
