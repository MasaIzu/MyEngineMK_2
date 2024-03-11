#include "MiddleBossEnemy.h"
#include <CollisionAttribute.h>
#include <SphereCollider.h>
#include <imgui.h>
#include <Easing.h>
#include <Numbers.h>
#include "LightData.h"

MiddleBossEnemy::MiddleBossEnemy(AudioManager* audioManager_)
{
	MovieUpdateTimes = MaxMovieUpdateTimes;
	model_.reset(Model::CreateFromOBJ("sphereBulletEnemy",true));
	HeriHaneModel_.reset(Model::CreateFromOBJ("HeriHane",true));
	//fbx
	fbxModel_.reset(FbxLoader::GetInstance()->LoadModelFromFile("HeriFbx",true));
	fbxObj3d_ = FBXObject3d::Create();
	fbxObj3d_->SetModel(fbxModel_.get());
	fbxObj3d_->Update();
	BossWorldTrans.scale_ = Vector3(Scale,Scale,Scale);
	BossWorldTrans.Initialize();

	normalGunLeft = std::make_unique<NormalGun>(COLLISION_ATTR_ENEMY_BULLET_ATTACK);
	normalGunLeft->Initialize(BossWorldTrans.translation_,model_.get(),audioManager_,0.0f,false);
	normalGunRight = std::make_unique<NormalGun>(COLLISION_ATTR_ENEMY_BULLET_ATTACK);
	normalGunRight->Initialize(BossWorldTrans.translation_,model_.get(),audioManager_,0.0f,false);

	missileGunLeft = std::make_unique<MissileGun>(COLLISION_ATTR_ENEMY_BULLET_ATTACK);
	missileGunLeft->Initialize(BossWorldTrans.translation_,model_.get(),model_.get());

	missileGunRight = std::make_unique<MissileGun>(COLLISION_ATTR_ENEMY_BULLET_ATTACK);
	missileGunRight->Initialize(BossWorldTrans.translation_,model_.get(),model_.get());

	HeriHaneLeftTrans.scale_ = Vector3(Scale,Scale,Scale);
	HeriHaneLeftTrans.Initialize();
	HeriHaneRightTrans.scale_ = Vector3(Scale,Scale,Scale);
	HeriHaneRightTrans.Initialize();

	enemyHP2DUI = std::make_unique<EnemyHP2DUI>();
	enemyHP2DUI->Initialize();

	enemyHP3DUI = std::make_unique<EnemyHP3DUI>();
	enemyHP3DUI->Initialize();

	//当たり判定用
	EnemyCenterWorldTrans.Initialize();

	EnemyLeftHaneWorldTrans.scale_ = Vector3(BoneColRadius,BoneColRadius,BoneColRadius);
	EnemyLeftHaneWorldTrans.Initialize();
	EnemyRightHaneWorldTrans.scale_ = Vector3(BoneColRadius,BoneColRadius,BoneColRadius);
	EnemyRightHaneWorldTrans.Initialize();

	EnemyLeftNormalWorldTrans.scale_ = Vector3(BoneColRadius,BoneColRadius,BoneColRadius);
	EnemyLeftNormalWorldTrans.Initialize();
	EnemyRightNormalWorldTrans.scale_ = Vector3(BoneColRadius,BoneColRadius,BoneColRadius);
	EnemyRightNormalWorldTrans.Initialize();

	EnemyLeftMissileWorldTrans.scale_ = Vector3(BoneColRadius,BoneColRadius,BoneColRadius);
	EnemyLeftMissileWorldTrans.Initialize();
	EnemyRightMissileWorldTrans.scale_ = Vector3(BoneColRadius,BoneColRadius,BoneColRadius);
	EnemyRightMissileWorldTrans.Initialize();

	EnemyNecWorldTrans.scale_ = Vector3(BoneColRadius,BoneColRadius,BoneColRadius);
	EnemyNecWorldTrans.Initialize();

	EnemyHedWorldTrans.scale_ = Vector3(BoneColRadius,BoneColRadius,BoneColRadius);
	EnemyHedWorldTrans.Initialize();

	enemyBoostParticleLeft = std::make_unique<EnemyBoostParticle>();
	int MaxParticleCountA = 50000;
	enemyBoostParticleLeft->Initialize(MaxParticleCountA);
	enemyBoostParticleLeft->SetTextureHandle(TextureManager::Load("sprite/effect4.png"));

	enemyBoostParticleRight = std::make_unique<EnemyBoostParticle>();
	enemyBoostParticleRight->Initialize(MaxParticleCountA);
	enemyBoostParticleRight->SetTextureHandle(TextureManager::Load("sprite/effect4.png"));

	explosion = std::make_unique<Explosion>();
	explosion->Initialize(MaxParticleCountC);
	explosion->SetTextureHandle(TextureManager::Load("sprite/effect4.png"));

	LightLeftOne = LightData::GetInstance()->AddPointLight(BossWorldTrans.translation_,Vector3(1,1,1),Vector3(0.1f,0.1f,0.1f),false);
	LightLeftTwo = LightData::GetInstance()->AddPointLight(BossWorldTrans.translation_,Vector3(1,1,1),Vector3(0.1f,0.1f,0.1f),false);
	LightRightOne = LightData::GetInstance()->AddPointLight(BossWorldTrans.translation_,Vector3(1,1,1),Vector3(0.1f,0.1f,0.1f),false);
	LightRightTwo = LightData::GetInstance()->AddPointLight(BossWorldTrans.translation_,Vector3(1,1,1),Vector3(0.1f,0.1f,0.1f),false);

	LightBoostLeft = LightData::GetInstance()->AddPointLight(BossWorldTrans.translation_,Vector3(1,1,1),Vector3(0.1f,0.1f,0.1f),false);
	LightBoostRight = LightData::GetInstance()->AddPointLight(BossWorldTrans.translation_,Vector3(1,1,1),Vector3(0.1f,0.1f,0.1f),false);

	particleEditorLeft = std::make_unique<ParticleEditor>();
	particleEditorLeft->Initialize(MaxParticleCountC,false,"EnemyBoost");
	particleEditorLeft->SetTextureHandle(TextureManager::Load("sprite/effect4.png"));

	particleEditorRight = std::make_unique<ParticleEditor>();
	particleEditorRight->Initialize(MaxParticleCountC,false,"EnemyBoost");
	particleEditorRight->SetTextureHandle(TextureManager::Load("sprite/effect4.png"));

	UltDustParticle = std::make_unique<ParticleEditor>();
	UltDustParticle->Initialize("EnemyUltCharge");
	UltDustParticle->SetTextureHandle(TextureManager::Load("sprite/effect4.png"));

	EnemyUltPreparationDownParticle = std::make_unique<ParticleEditor>();
	EnemyUltPreparationDownParticle->Initialize("EnemyUltPreparation");
	EnemyUltPreparationDownParticle->SetTextureHandle(TextureManager::Load("sprite/effect4.png"));

	EnemyUltDownExplosion = std::make_unique<ParticleEditor>();
	EnemyUltDownExplosion->Initialize("EnemyUltDownPreparation");
	EnemyUltDownExplosion->SetTextureHandle(TextureManager::Load("sprite/effect4.png"));

	audioManager = audioManager_;

	HeriSound = audioManager->LoadAudio("Resources/Sound/Heri.mp3",soundVol,false);
	audioManager->PlayWave(HeriSound,true);

	ChargeSound = audioManager->LoadAudio("Resources/Sound/chargeSound.mp3",soundVol,false);
	ChargeFinSound = audioManager->LoadAudio("Resources/Sound/ChargeFin.mp3",soundVol,false);
	ExplosionSound = audioManager->LoadAudio("Resources/Sound/explosion.mp3",soundVol,false);
	UltResoundSound = audioManager->LoadAudio("Resources/Sound/Resound.mp3",soundVol,false);

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
	this->player = Player;

	UltWorldTrans.scale_ = Vector3(UltRadius,UltRadius,UltRadius);
	UltWorldTrans.Initialize();
	UltWorldTrans.TransferMatrix();

	MiddleBossCollider[ 0 ] = new SphereCollider(Vector4(FloatNumber(fNumbers::fZero),Radius,FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fZero)),Radius);
	CollisionManager::GetInstance()->AddCollider(MiddleBossCollider[ 0 ]);
	MiddleBossCollider[ 0 ]->SetAttribute(COLLISION_ATTR_ENEMYS);
	MiddleBossCollider[ 0 ]->Update(BossWorldTrans.matWorld_);
	// コリジョンマネージャに追加
	for ( uint32_t i = 1; i < ColCount; i++ )
	{
		MiddleBossCollider[ i ] = new SphereCollider(Vector4(FloatNumber(fNumbers::fZero),BoneColRadius,FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fZero)),BoneColRadius);
		CollisionManager::GetInstance()->AddCollider(MiddleBossCollider[ i ]);
		MiddleBossCollider[ i ]->SetAttribute(COLLISION_ATTR_ENEMYS);
		MiddleBossCollider[ i ]->Update(BossWorldTrans.matWorld_);
	}

	MiddleBossUltCollider = new SphereCollider(Vector4(FloatNumber(fNumbers::fZero),UltRadius,FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fZero)),UltRadius);
	CollisionManager::GetInstance()->AddCollider(MiddleBossUltCollider);
	MiddleBossUltCollider->SetAttribute(COLLISION_ATTR_NOTATTACK);
	MiddleBossUltCollider->Update(BossWorldTrans.matWorld_);

}

void MiddleBossEnemy::Update()
{
	isExplosion = false;


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
	if ( !isDeadMotion )
	{
		AliveUpdate();
	}
	else
	{
		DieMotionUpdate();
	}


	WorldTransUpdate();
	HeriHaneRotYLeft += HeriHaneRotSpeed;
	HeriHaneLeftTrans.translation_ = MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(static_cast< uint32_t >( Numbers::One )) * BossWorldTrans.matWorld_);
	HeriHaneLeftTrans.SetRot(Vector3(0,HeriHaneRotYLeft + MyMath::GetAngle(Angle),0));
	HeriHaneLeftTrans.TransferMatrix();

	HeriHaneRotYRight -= HeriHaneRotSpeed;
	HeriHaneRightTrans.translation_ = MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(static_cast< uint32_t >( Numbers::Zero )) * BossWorldTrans.matWorld_);
	HeriHaneRightTrans.SetRot(Vector3(0,HeriHaneRotYRight + MyMath::GetAngle(Angle),0));
	HeriHaneRightTrans.TransferMatrix();

	normalGunLeftPos = MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(static_cast< uint32_t >( Numbers::Two )) * BossWorldTrans.matWorld_);
	normalGunRightPos = MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(static_cast< uint32_t >( Numbers::Four )) * BossWorldTrans.matWorld_);
	missileGunLeftPos = MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(static_cast< uint32_t >( Numbers::Three )) * BossWorldTrans.matWorld_);
	missileGunRightPos = MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(static_cast< uint32_t >( Numbers::Five )) * BossWorldTrans.matWorld_);

	normalGunLeft->Update(normalGunLeftPos,Vector3(0,MyMath::GetAngle(Angle),0));
	normalGunRight->Update(missileGunRightPos,Vector3(0,MyMath::GetAngle(Angle),0));
	missileGunLeft->Update(missileGunLeftPos,player->GetPlayerPos(),Vector3(0,MyMath::GetAngle(Angle),0));
	missileGunRight->Update(missileGunRightPos,player->GetPlayerPos(),Vector3(0,MyMath::GetAngle(Angle),0));

	EnemyBoostLeftPos.BoostStartPos[ 0 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(BoostPosLeftStart) * BossWorldTrans.matWorld_));
	EnemyBoostLeftPos.BoostEndPos[ 0 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(BoostPosLeftEnd) * BossWorldTrans.matWorld_)) - Vector4(0,5,0,0);
	EnemyBoostLeftPos.BoostStartPos[ 1 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(BoostPosLeftBackStart) * BossWorldTrans.matWorld_));
	EnemyBoostLeftPos.BoostEndPos[ 1 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(BoostPosLeftBackEnd) * BossWorldTrans.matWorld_)) - Vector4(0,5,0,0);

	EnemyBoostRightPos.BoostStartPos[ 0 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(BoostPosRightStart) * BossWorldTrans.matWorld_));
	EnemyBoostRightPos.BoostEndPos[ 0 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(BoostPosRightEnd) * BossWorldTrans.matWorld_)) - Vector4(0,5,0,0);
	EnemyBoostRightPos.BoostStartPos[ 1 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(BoostPosRightBackStart) * BossWorldTrans.matWorld_));
	EnemyBoostRightPos.BoostEndPos[ 1 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(BoostPosRightBackEnd) * BossWorldTrans.matWorld_)) - Vector4(0,5,0,0);

	enemyHP2DUI->Update();
	enemyHP3DUI->Update();

	LightData::GetInstance()->UpdatePointLight(LightLeftOne,normalGunLeftPos,Vector3(1,1,1),Vector3(0.1f,0.1f,0.1f),isLightActive);
	LightData::GetInstance()->UpdatePointLight(LightLeftTwo,missileGunLeftPos,Vector3(1,0.2f,0.2f),Vector3(0.1f,0.1f,0.1f),isLightActive);
	LightData::GetInstance()->UpdatePointLight(LightRightOne,normalGunRightPos,Vector3(1,1,1),Vector3(0.1f,0.1f,0.1f),isLightActive);
	LightData::GetInstance()->UpdatePointLight(LightRightTwo,missileGunRightPos,Vector3(1,0.2f,0.2f),Vector3(0.1f,0.1f,0.1f),isLightActive);
	LightData::GetInstance()->UpdatePointLight(LightBoostLeft,MyMath::Vec4ToVec3(EnemyBoostLeftPos.BoostEndPos[ 0 ]),Vector3(1,1,1),Vector3(0.1f,0.1f,0.1f),isLightActive);
	LightData::GetInstance()->UpdatePointLight(LightBoostRight,MyMath::Vec4ToVec3(EnemyBoostRightPos.BoostEndPos[ 0 ]),Vector3(1,1,1),Vector3(0.1f,0.1f,0.1f),isLightActive);

	ColTransUpdate();//当たり判定の場所アップデート
	ColUpdate();//当たり判定のアップデート

	UltWorldTrans.translation_ = BossWorldTrans.translation_;
	UltWorldTrans.translation_.y = UltDownYPos;
	UltWorldTrans.scale_ = Vector3(UltRadius,UltRadius,UltRadius);
	UltWorldTrans.TransferMatrix();

	
	audioManager->ChangeVolume(HeriSound,soundVol);


	particleEditorLeft->EditUpdate();
	particleEditorRight->EditUpdate();
}

void MiddleBossEnemy::Draw(const ViewProjection& viewProjection_)
{
	//model_->Draw(UltWorldTrans,viewProjection_);
	//model_->Draw(EnemyNecWorldTrans,viewProjection_);
	//model_->Draw(EnemyHedWorldTrans,viewProjection_);

	if ( !isDead )
	{
		normalGunLeft->Draw(viewProjection_);
		normalGunRight->Draw(viewProjection_);
		missileGunLeft->Draw(viewProjection_);
		missileGunRight->Draw(viewProjection_);
		HeriHaneModel_->Draw(HeriHaneLeftTrans,viewProjection_);
		HeriHaneModel_->Draw(HeriHaneRightTrans,viewProjection_);
	}
}

void MiddleBossEnemy::FbxDraw(const ViewProjection& viewProjection_)
{
	if ( isSporn )
	{
		if ( !isDead )
		{
			fbxObj3d_->Draw(BossWorldTrans,viewProjection_);
		}
	}
}

void MiddleBossEnemy::ParticleDraw(const ViewProjection& viewProjection_)
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = DirectXCore::GetInstance()->GetCommandList();
	if ( !isDead )
	{
		EnemyBoostParticle::PreDraw(commandList);
		//enemyBoostParticleLeft->Draw(viewProjection_);
		//enemyBoostParticleRight->Draw(viewProjection_);
		EnemyBoostParticle::PostDraw();

		particleEditorLeft->Draw(viewProjection_);
		particleEditorRight->Draw(viewProjection_);
		UltDustParticle->Draw(viewProjection_);
		EnemyUltPreparationDownParticle->Draw(viewProjection_);
		EnemyUltDownExplosion->Draw(viewProjection_);
	}

	Explosion::PreDraw(commandList);
	explosion->Draw(viewProjection_);
	Explosion::PostDraw();
}

void MiddleBossEnemy::DrawSprite(const ViewProjection& viewProjection_)
{
	Vector3 HPposition = BossWorldTrans.translation_ + Vector3(0,10,0);

	if ( !isDead )
	{
		enemyHP2DUI->Draw(HpPosition);
		enemyHP3DUI->Draw(HPposition,viewProjection_);
	}
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
	HeriHaneRotYLeft += HeriHaneRotSpeed;
	HeriHaneLeftTrans.translation_ = MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(static_cast< uint32_t >( Numbers::One )) * BossWorldTrans.matWorld_);
	HeriHaneLeftTrans.SetRot(Vector3(0,HeriHaneRotYLeft + MyMath::GetAngle(Angle),0));
	HeriHaneLeftTrans.TransferMatrix();

	HeriHaneRotYRight -= HeriHaneRotSpeed;
	HeriHaneRightTrans.translation_ = MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(static_cast< uint32_t >( Numbers::Zero )) * BossWorldTrans.matWorld_);
	HeriHaneRightTrans.SetRot(Vector3(0,HeriHaneRotYRight + MyMath::GetAngle(Angle),0));
	HeriHaneRightTrans.TransferMatrix();

	normalGunLeftPos = MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(static_cast< uint32_t >( Numbers::Two )) * BossWorldTrans.matWorld_);
	normalGunRightPos = MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(static_cast< uint32_t >( Numbers::Four )) * BossWorldTrans.matWorld_);
	missileGunLeftPos = MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(static_cast< uint32_t >( Numbers::Three )) * BossWorldTrans.matWorld_);
	missileGunRightPos = MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(static_cast< uint32_t >( Numbers::Five )) * BossWorldTrans.matWorld_);

	normalGunLeft->Update(normalGunLeftPos,Vector3(0,MyMath::GetAngle(Angle),0),NormalGunSoundVol);
	normalGunRight->Update(normalGunRightPos,Vector3(0,MyMath::GetAngle(Angle),0));
	missileGunLeft->Update(missileGunLeftPos,Vector3(0,0,0),Vector3(0,MyMath::GetAngle(Angle),0));
	missileGunRight->Update(missileGunRightPos,Vector3(0,0,0),Vector3(0,MyMath::GetAngle(Angle),0));

	EnemyBoostLeftPos.BoostStartPos[ 0 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(BoostPosLeftStart) * BossWorldTrans.matWorld_));
	EnemyBoostLeftPos.BoostEndPos[ 0 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(BoostPosLeftEnd) * BossWorldTrans.matWorld_));
	EnemyBoostLeftPos.BoostStartPos[ 1 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(BoostPosLeftBackStart) * BossWorldTrans.matWorld_));
	EnemyBoostLeftPos.BoostEndPos[ 1 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(BoostPosLeftBackEnd) * BossWorldTrans.matWorld_));

	EnemyBoostRightPos.BoostStartPos[ 0 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(BoostPosRightStart) * BossWorldTrans.matWorld_));
	EnemyBoostRightPos.BoostEndPos[ 0 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(BoostPosRightEnd) * BossWorldTrans.matWorld_));
	EnemyBoostRightPos.BoostStartPos[ 1 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(BoostPosRightBackStart) * BossWorldTrans.matWorld_));
	EnemyBoostRightPos.BoostEndPos[ 1 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(BoostPosRightBackEnd) * BossWorldTrans.matWorld_));

	enemyHP2DUI->Update();
	enemyHP3DUI->Update();

	LightData::GetInstance()->UpdatePointLight(LightLeftOne,normalGunLeftPos,Vector3(1,1,1),Vector3(0.1f,0.1f,0.1f),isLightActive);
	LightData::GetInstance()->UpdatePointLight(LightLeftTwo,missileGunLeftPos,Vector3(1,0.2f,0.2f),Vector3(0.1f,0.1f,0.1f),isLightActive);
	LightData::GetInstance()->UpdatePointLight(LightRightOne,normalGunRightPos,Vector3(1,1,1),Vector3(0.1f,0.1f,0.1f),isLightActive);
	LightData::GetInstance()->UpdatePointLight(LightRightTwo,missileGunRightPos,Vector3(1,0.2f,0.2f),Vector3(0.1f,0.1f,0.1f),isLightActive);
	LightData::GetInstance()->UpdatePointLight(LightBoostLeft,MyMath::Vec4ToVec3(EnemyBoostLeftPos.BoostEndPos[ 0 ]),Vector3(1,1,1),Vector3(0.1f,0.1f,0.1f),isLightActive);
	LightData::GetInstance()->UpdatePointLight(LightBoostRight,MyMath::Vec4ToVec3(EnemyBoostRightPos.BoostEndPos[ 0 ]),Vector3(1,1,1),Vector3(0.1f,0.1f,0.1f),isLightActive);

	ColTransUpdate();//当たり判定の場所アップデート

	if ( !isDead )
	{
		MyMath::CircleHit(player->GetPlayerPos(),BossWorldTrans.translation_,SoundRadius,soundDistance);
		soundVol = MaxSoundVol * soundDistance;
		ChargeSoundsVol = MaxChargeSoundVol * soundDistance;
		ChargeSoundVol = ChargeSoundsVol;
		ChargeFinSoundVol = ChargeSoundsVol;
		ExplosionSoundVol = ChargeSoundsVol;
		UltResoundSoundVol = ChargeSoundsVol;
		NormalGunSoundVol = ChargeSoundsVol;
	}
	audioManager->ChangeVolume(HeriSound,soundVol);

	return false;
}

void MiddleBossEnemy::TitleMovieUpdate(const Vector3& startPos,Vector3& endPos)
{
	if ( isSporn == false )
	{
		isSporn = true;
		BossWorldTrans.translation_ = startPos;
		EndPos = endPos;
		Vector3 startToEnd = endPos - startPos;
		Velocity = startToEnd / MaxMovieUpdateTimes;
		isTurn = true;
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
		}
	}
	
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
		}

		BossWorldTrans.SetRot(Vector3(FloatNumber(fNumbers::fZero),MyMath::GetAngle(Angle),FloatNumber(fNumbers::fZero)));

	}

	WorldTransUpdate();
	HeriHaneRotYLeft += HeriHaneRotSpeed;
	HeriHaneLeftTrans.translation_ = MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(static_cast< uint32_t >( Numbers::One )) * BossWorldTrans.matWorld_);
	HeriHaneLeftTrans.SetRot(Vector3(0,HeriHaneRotYLeft + MyMath::GetAngle(Angle),0));
	HeriHaneLeftTrans.TransferMatrix();

	HeriHaneRotYRight -= HeriHaneRotSpeed;
	HeriHaneRightTrans.translation_ = MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(static_cast< uint32_t >( Numbers::Zero )) * BossWorldTrans.matWorld_);
	HeriHaneRightTrans.SetRot(Vector3(0,HeriHaneRotYRight + MyMath::GetAngle(Angle),0));
	HeriHaneRightTrans.TransferMatrix();

	normalGunLeftPos = MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(static_cast< uint32_t >( Numbers::Two )) * BossWorldTrans.matWorld_);
	normalGunRightPos = MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(static_cast< uint32_t >( Numbers::Four )) * BossWorldTrans.matWorld_);
	missileGunLeftPos = MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(static_cast< uint32_t >( Numbers::Three )) * BossWorldTrans.matWorld_);
	missileGunRightPos = MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(static_cast< uint32_t >( Numbers::Five )) * BossWorldTrans.matWorld_);

	normalGunLeft->Update(normalGunLeftPos,Vector3(0,MyMath::GetAngle(Angle),0));
	normalGunRight->Update(missileGunRightPos,Vector3(0,MyMath::GetAngle(Angle),0));
	missileGunLeft->Update(missileGunLeftPos,Vector3(0,0,0),Vector3(0,MyMath::GetAngle(Angle),0));
	missileGunRight->Update(missileGunRightPos,Vector3(0,0,0),Vector3(0,MyMath::GetAngle(Angle),0));

	EnemyBoostLeftPos.BoostStartPos[ 0 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(BoostPosLeftStart) * BossWorldTrans.matWorld_));
	EnemyBoostLeftPos.BoostEndPos[ 0 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(BoostPosLeftEnd) * BossWorldTrans.matWorld_));
	EnemyBoostLeftPos.BoostStartPos[ 1 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(BoostPosLeftBackStart) * BossWorldTrans.matWorld_));
	EnemyBoostLeftPos.BoostEndPos[ 1 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(BoostPosLeftBackEnd) * BossWorldTrans.matWorld_));

	EnemyBoostRightPos.BoostStartPos[ 0 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(BoostPosRightStart) * BossWorldTrans.matWorld_));
	EnemyBoostRightPos.BoostEndPos[ 0 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(BoostPosRightEnd) * BossWorldTrans.matWorld_));
	EnemyBoostRightPos.BoostStartPos[ 1 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(BoostPosRightBackStart) * BossWorldTrans.matWorld_));
	EnemyBoostRightPos.BoostEndPos[ 1 ] = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(BoostPosRightBackEnd) * BossWorldTrans.matWorld_));

	enemyHP2DUI->Update();
	enemyHP3DUI->Update();

	LightData::GetInstance()->UpdatePointLight(LightLeftOne,normalGunLeftPos,Vector3(1,1,1),Vector3(0.1f,0.1f,0.1f));
	LightData::GetInstance()->UpdatePointLight(LightLeftTwo,missileGunLeftPos,Vector3(1,0.2f,0.2f),Vector3(0.1f,0.1f,0.1f));
	LightData::GetInstance()->UpdatePointLight(LightRightOne,normalGunRightPos,Vector3(1,1,1),Vector3(0.1f,0.1f,0.1f));
	LightData::GetInstance()->UpdatePointLight(LightRightTwo,missileGunRightPos,Vector3(1,0.2f,0.2f),Vector3(0.1f,0.1f,0.1f));
	LightData::GetInstance()->UpdatePointLight(LightBoostLeft,MyMath::Vec4ToVec3(EnemyBoostLeftPos.BoostEndPos[ 0 ]),Vector3(1,1,1),Vector3(0.1f,0.1f,0.1f));
	LightData::GetInstance()->UpdatePointLight(LightBoostRight,MyMath::Vec4ToVec3(EnemyBoostRightPos.BoostEndPos[ 0 ]),Vector3(1,1,1),Vector3(0.1f,0.1f,0.1f));

	ColTransUpdate();//当たり判定の場所アップデート
}

void MiddleBossEnemy::TitleUpdate(const Vector3& TrackingLocation)
{
	missileGunLeft->Update(BossWorldTrans.translation_,TrackingLocation,Vector3(0,0,0));
	missileGunRight->Update(BossWorldTrans.translation_,TrackingLocation,Vector3(0,0,0));
}

void MiddleBossEnemy::MakeTitleMissileBullet()
{
	if ( isTitleShot == false )
	{
		isTitleShot = true;
		missileGunLeft->ShotBullet();
		missileGunRight->ShotBullet();
	}
}

void MiddleBossEnemy::ResetTitleMove()
{
	isTitleShot = false;
	isSporn = false;
	Angle = FloatNumber(fNumbers::fZero);
	MovieUpdateTimes = MaxMovieUpdateTimes;
}

void MiddleBossEnemy::CSUpdate(ID3D12GraphicsCommandList* cmdList)
{
	enemyBoostParticleLeft->CSUpdate(cmdList,EnemyBoostLeftPos,BoostEndPower);
	enemyBoostParticleRight->CSUpdate(cmdList,EnemyBoostRightPos,BoostEndPower);

	explosion->CSUpdate(cmdList,isExplosion,MyMath::Vec3ToVec4(BossWorldTrans.translation_));

	particleEditorLeft->CSUpdate(cmdList,EnemyBoostLeftPos.BoostStartPos[ 1 ],EnemyBoostLeftPos.BoostEndPos[ 0 ]);
	particleEditorRight->CSUpdate(cmdList,EnemyBoostRightPos.BoostStartPos[ 1 ],EnemyBoostRightPos.BoostEndPos[ 0 ]);
	Vector3 ParticleEndPos = BossWorldTrans.translation_ - Vector3(0,Radius,0);
	UltDustParticle->CSUpdate(cmdList,isUltChargeFin,isUltPreparation,MyMath::Vec3ToVec4(ParticleEndPos));
	EnemyUltPreparationDownParticle->CSUpdate(cmdList,MyMath::Vec3ToVec4(UltPos),static_cast<uint32_t>(isUltDown));
	EnemyUltDownExplosion->CSUpdate(cmdList,MyMath::Vec3ToVec4(UltPos),static_cast< uint32_t >( isUltExplosion ));
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

void MiddleBossEnemy::AliveUpdate()
{
	if ( isStart )
	{
		Timer();

		if ( isUltTime == false )
		{
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
		}

		if ( isMoveing )
		{
			if ( isDownSpeed == false )
			{
				BossWorldTrans.translation_ = Easing::EaseInSineVec3(MoveStartPos,MovePos,MoveingTimer,MaxMoveingTimer);
			}

			if ( MoveingTimer == MaxMoveingTimer )
			{
				if ( isDownSpeed == false )
				{
					isDownSpeed = true;
					DownCount = static_cast< uint32_t >( Numbers::Zero );
					DownVelocity = BossWorldTrans.translation_ - OldPos;
					Velocity = DownVelocity / static_cast< float >( MaxDownCount );
					DownVelocity.y = static_cast< float >( Numbers::Zero );
					Velocity.y = static_cast< float >( Numbers::Zero );
				}
				else
				{
					if ( isDownSpeedFinish == false )
					{
						if ( DownCount < MaxDownCount )
						{
							DownCount++;
							DownVelocity -= Velocity;
							BossWorldTrans.translation_ += DownVelocity;
						}
						else
						{
							isDownSpeedFinish = true;
						}
					}
					else
					{
						if ( isOneMoreTime == true )
						{
							isOneMoreTime = false;
							isDownSpeed = false;
							isDownSpeedFinish = false;
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

							if ( !isDead )
							{
								missileGunLeft->ShotBullet();
								missileGunRight->ShotBullet();
							}
							
						}
						else
						{
							isMoveing = false;
						}
					}
				}
			}
			OldPos = BossWorldTrans.translation_;
		}
		else if ( attackType == AttackType::UltPreparation )
		{
			if ( UltYPos >= BossWorldTrans.translation_.y )
			{
				isUltPreparation = true;
				isUlting = true;
				isUltChargeFin = true;
				BossWorldTrans.translation_.y += UltYUpPow;
				audioManager->ChangeVolume(ChargeSound,ChargeSoundVol);

				if ( UltYPosChargeFinPos <= BossWorldTrans.translation_.y )
				{
					isUltChargeFin = false;
				}
			}
			else
			{
				attackType = AttackType::PutUltDown;
				isUltPreparation = false;
				isUltDown = true;
				ChargeSoundVol = 0.0f;
				audioManager->ChangeVolume(ChargeSound,ChargeSoundVol);
				audioManager->ChangeVolume(ChargeFinSound,ChargeFinSoundVol);
				audioManager->PlayWave(ChargeFinSound,false);
				UltPos = BossWorldTrans.translation_ - Vector3(0,Radius,0);
			}
		}
		else if ( attackType == AttackType::PutUltDown )
		{
			if ( UltPos.y >= UltDownYPos )
			{
				UltPos.y -= UltDownYPow;
			}
			else
			{
				isUltDown = false;
				isUltExplosion = true;
				audioManager->ChangeVolume(ExplosionSound,ExplosionSoundVol);
				audioManager->ChangeVolume(UltResoundSound,UltResoundSoundVol);
				audioManager->PlayWave(ExplosionSound,false);
				audioManager->PlayWave(UltResoundSound,false);
				MiddleBossUltCollider->SetAttribute(COLLISION_ATTR_ENEMY_ULT);
				attackType = AttackType::Ult;
			}
		}
		else if ( attackType == AttackType::Ult )
		{
			if ( ExplosionTime < ExplosionMaxTime )
			{
				ExplosionTime++;

				if ( UltMaxRadius > UltRadius )
				{
					UltRadius += UltAddRadiusPow;
				}
			}
			else
			{
				MiddleBossUltCollider->SetAttribute(COLLISION_ATTR_NOTATTACK);
				attackType = AttackType::Back;
				isUltExplosion = false;
				isUlting = false;
			}
		}
		else if ( attackType == AttackType::Back )
		{
			if ( EndPos.y < BossWorldTrans.translation_.y )
			{
				BossWorldTrans.translation_.y -= UltYUpPow;
			}
			else
			{
				isUltTime = false;
				attackType = AttackType::Move;
			}
		}

		if ( isBackSponePos == true )
		{
			Vector3 goPos = BackPoints[ BackPosCounter ] - BossWorldTrans.translation_;
			goPos.normalize();
			OldPos = BossWorldTrans.translation_;
			Velocity = MyMath::lerp(Velocity,goPos,BackLarpStrength);
			BossWorldTrans.translation_ += Velocity * BackSpeed;
			// 中心点の距離の２乗 <= 半径の和の２乗　なら交差
			Vector3 tmp;
			tmp = BackPoints[ BackPosCounter ] - BossWorldTrans.translation_;
			float dist = tmp.dot(tmp);
			float radius2 = BackPosRadius;
			radius2 *= radius2;

			if ( BackMissileTimes < BackMissileMaxTimes )
			{
				if ( BulletCoolTime == 0 )
				{
					BulletCoolTime = BackMissileCoolTime;
					if ( !isDead )
					{
						missileGunLeft->ShotBullet();
						missileGunRight->ShotBullet();
					}
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
					isMoveing = true;
					isDownSpeed = false;
					MoveingTimer = MaxMoveingTimer;
					MoveStartPos = BossWorldTrans.translation_;
					MovePos = BossWorldTrans.translation_;
					if ( attackType == AttackType::UltPreparationForBack )
					{
						isMoveing = false;
						isUltTime = true;
						attackType = AttackType::UltPreparation;
						audioManager->PlayWave(ChargeSound,true);
					}
				}
			}
		}

		for ( auto&& col : MiddleBossCollider )
		{
			if ( col->GetHit() )
			{
				col->Reset();
				MiddleBossHp -= Damage.NormalAttack;
				if ( MiddleBossHp > 0.0f )
				{

				}
				else
				{
					isDeadMotion = true;
					MiddleBossHp = static_cast< float >( Numbers::Zero );//0固定;
				}
				enemyHP2DUI->EnemyHpUpdate(MiddleBossHp,MaxMiddleBossHp);
				enemyHP3DUI->EnemyHpUpdate(MiddleBossHp,MaxMiddleBossHp);
			}
			if ( col->GetMeleeHit() )
			{
				col->ResetMeleeHit();
				MiddleBossHp -= Damage.BladeAttack;
				if ( MiddleBossHp > 0.0f )
				{

				}
				else
				{
					isDeadMotion = true;
					MiddleBossHp = static_cast< float >( Numbers::Zero );//0固定;
				}
				enemyHP2DUI->EnemyHpUpdate(MiddleBossHp,MaxMiddleBossHp);
				enemyHP3DUI->EnemyHpUpdate(MiddleBossHp,MaxMiddleBossHp);
			}
		}

		Angle = MyMath::Get2VecAngle(BossWorldTrans.translation_ + BossWorldTrans.LookVelocity.look,player->GetPlayerPos());

		BossWorldTrans.SetRot(Vector3(FloatNumber(fNumbers::fZero),MyMath::GetAngle(Angle),FloatNumber(fNumbers::fZero)));
	}
}

void MiddleBossEnemy::Attack()
{
	if ( attackType == AttackType::Nomal )
	{
		if ( !isDead )
		{
			normalGunLeft->ShotBullet(player->GetPlayerPos());
			normalGunRight->ShotBullet(player->GetPlayerPos());
		}
	}
	else if ( attackType == AttackType::Missile )
	{
		BulletCoolTime = MaxBulletCoolTime;
		if ( !isDead )
		{
			missileGunLeft->ShotBullet();
			missileGunRight->ShotBullet();
		}
	}
	else if ( attackType == AttackType::MoveingAttack )
	{
		if ( !isDead )
		{
			normalGunLeft->ShotBullet(player->GetPlayerPos());
			normalGunRight->ShotBullet(player->GetPlayerPos());
		}
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

	if ( isUltShoot == false )
	{
		if ( MiddleBossHp <= ( MaxMiddleBossHp / 2 ) )
		{
			isUltShoot = true;
			attackType = AttackType::UltPreparationForBack;
		}
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
		isDownSpeed = false;
		isDownSpeedFinish = false;
		isOneMoreTime = MyMath::Random(static_cast< uint32_t >( Numbers::Zero ),static_cast< uint32_t >( Numbers::One ));
		MoveingTimer = static_cast< uint32_t >( Numbers::Zero );
		MaxMoveingTimer = MoveFirstTime;
		if ( isOneMoreTime == true )
		{
			isOneMoreTime = MyMath::Random(static_cast< uint32_t >( Numbers::Zero ),static_cast< uint32_t >( Numbers::One ));
		}
		mveType = MyMath::Random(static_cast< uint32_t >( Numbers::Zero ),static_cast< uint32_t >( Numbers::Three ));
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
				BackStartPoint(tmp);
			}
		}

	}
	else if ( attackType == AttackType::UltPreparationForBack )
	{
		// 中心点の距離の２乗 <= 半径の和の２乗　なら交差
		Vector3 tmp;
		tmp = EndPos - BossWorldTrans.translation_;
		//float dist = tmp.dot(tmp);
		float radius2 = MoveSafeRadius;
		radius2 *= radius2;

		BackStartPoint(tmp);
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

void MiddleBossEnemy::ColTransUpdate()
{
	EnemyCenterWorldTrans.translation_ = BossWorldTrans.translation_ - Coladjustment;
	EnemyCenterWorldTrans.TransferMatrix();

	EnemyLeftHaneWorldTrans.translation_ = MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(static_cast< uint32_t >( Numbers::One )) * BossWorldTrans.matWorld_) - HaneColadjustment;
	EnemyLeftHaneWorldTrans.TransferMatrix();

	EnemyRightHaneWorldTrans.translation_ = MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(static_cast< uint32_t >( Numbers::Zero )) * BossWorldTrans.matWorld_) - HaneColadjustment;
	EnemyRightHaneWorldTrans.TransferMatrix();

	EnemyLeftNormalWorldTrans.translation_ = MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(static_cast< uint32_t >( Numbers::Two )) * BossWorldTrans.matWorld_) + ( normalGunLeft->GetLook().lookBack.norm() * NormalGunBackCol );
	EnemyLeftNormalWorldTrans.TransferMatrix();

	EnemyRightNormalWorldTrans.translation_ = MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(static_cast< uint32_t >( Numbers::Four )) * BossWorldTrans.matWorld_) + ( normalGunRight->GetLook().lookBack.norm() * NormalGunBackCol );
	EnemyRightNormalWorldTrans.TransferMatrix();

	EnemyLeftMissileWorldTrans.translation_ = MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(static_cast< uint32_t >( Numbers::Three )) * BossWorldTrans.matWorld_) + ( missileGunLeft->GetLook().lookBack.norm() * NormalGunBackCol );
	EnemyLeftMissileWorldTrans.TransferMatrix();

	EnemyRightMissileWorldTrans.translation_ = MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(static_cast< uint32_t >( Numbers::Five )) * BossWorldTrans.matWorld_) + ( missileGunRight->GetLook().lookBack.norm() * NormalGunBackCol );
	EnemyRightMissileWorldTrans.TransferMatrix();

	EnemyNecWorldTrans.translation_ = MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(static_cast< uint32_t >( Numbers::Six )) * BossWorldTrans.matWorld_);
	EnemyNecWorldTrans.TransferMatrix();

	EnemyHedWorldTrans.translation_ = MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(static_cast< uint32_t >( Numbers::Seven )) * BossWorldTrans.matWorld_);
	EnemyHedWorldTrans.TransferMatrix();
}

void MiddleBossEnemy::ColUpdate()
{
	MiddleBossCollider[ 0 ]->Update(EnemyCenterWorldTrans.matWorld_);
	MiddleBossCollider[ 1 ]->Update(EnemyLeftHaneWorldTrans.matWorld_);
	MiddleBossCollider[ 2 ]->Update(EnemyRightHaneWorldTrans.matWorld_);
	MiddleBossCollider[ 3 ]->Update(EnemyLeftNormalWorldTrans.matWorld_);
	MiddleBossCollider[ 4 ]->Update(EnemyRightNormalWorldTrans.matWorld_);
	MiddleBossCollider[ 5 ]->Update(EnemyLeftMissileWorldTrans.matWorld_);
	MiddleBossCollider[ 6 ]->Update(EnemyRightMissileWorldTrans.matWorld_);
	MiddleBossCollider[ 7 ]->Update(EnemyNecWorldTrans.matWorld_);
	MiddleBossCollider[ 8 ]->Update(EnemyHedWorldTrans.matWorld_);

	MiddleBossUltCollider->Update(UltWorldTrans.matWorld_,UltRadius);
}

void MiddleBossEnemy::DieMotionUpdate()
{
	if ( DieMotionCount < DieMaxMotionCount )
	{
		DieMotionCount++;
		RotY += RotAddY;
		BossWorldTrans.SetRot(Vector3(FloatNumber(fNumbers::fZero),MyMath::GetAngle(Angle),RotY));

		BossWorldTrans.translation_ -= DieMotionDown;
	}
	else
	{
		isDead = true;
		isDeadMotion = false;
		isLightActive = false;
		soundVol = 0.0f;
		if ( !isAllReadyExplosion )
		{
			if ( !isExplosion )
			{
				isExplosion = true;
			}
			else
			{
				isExplosion = false;
				isAllReadyExplosion = true;
			}
		}
	}
}

void MiddleBossEnemy::BackStartPoint(const Vector3& tmp)
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
	if ( LeftOrLight == static_cast< float >( Numbers::One ) )//左
	{
		EasingWaypoint = BossWorldTrans.translation_ + ( BossWorldTrans.LookVelocity.lookBack_lookLeft.norm() * BackBosPower );
		BackPoints.push_back(EasingWaypoint);
	}
	else if ( LeftOrLight == -static_cast< float >( Numbers::One ) )//右
	{
		EasingWaypoint = BossWorldTrans.translation_ + ( BossWorldTrans.LookVelocity.lookBack_lookRight.norm() * BackBosPower );
		BackPoints.push_back(EasingWaypoint);
	}
	Vector3 waypoint = EndPos - EasingWaypoint;
	waypoint = ( waypoint / static_cast< float >( fNumbers::fTwoPointZero ) ) + EasingWaypoint + Vector3(static_cast< float >( fNumbers::fOnePointZero ),jampHeight,static_cast< float >( fNumbers::fOnePointZero )) + BossWorldTrans.LookVelocity.lookBack * BackStrength;
	BackPoints.push_back(waypoint);
	BackPoints.push_back(EndPos);
}

Vector3 MiddleBossEnemy::GetPosition() const
{
	return MyMath::GetWorldTransform(EnemyHedWorldTrans.matWorld_);
}

Vector3 MiddleBossEnemy::GetEndPos() const
{
	return EndPos;
}

Vector3 MiddleBossEnemy::GetUltPos() const
{
	return UltPos;
}

Vector4 MiddleBossEnemy::GetUltPreparationPosition() const
{
	Vector3 ParticleEndPos = BossWorldTrans.translation_ - Vector3(0,Radius,0);
	return MyMath::Vec3ToVec4(ParticleEndPos);
}
