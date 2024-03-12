#include "MediumBossStage.h"
#include "MyMath.h"
#include <PostEffect.h>
#include <ParticleManager.h>
#include <SphereCollider.h>
#include <Numbers.h>
#include <Framework.h>
#include <PostEffectManager.h>

MediumBossStage::MediumBossStage()
{

}

MediumBossStage::~MediumBossStage()
{
	LightData::GetInstance()->ClearLight();
	collisionManager->AllClearCollider();

	if ( audioManager != nullptr )
	{
		audioManager->AllStop();
	}
}

void MediumBossStage::Initialize()
{
	dxCommon_ = DirectXCore::GetInstance();
	winApp_ = WinApp::GetInstance();
	input_ = Input::GetInstance();
	audioManager = AudioManager::GetInstance();

	sprite_ = Sprite::Create(TextureManager::Load("sprite/Blackout.png"));

	skydome = std::make_unique<Skydome>();
	skydome->Initialize();

	viewProjection_ = std::make_unique<ViewProjection>();
	viewProjection_->Initialize();
	viewProjection_->eye = { 0,0,-50 };
	viewProjection_->UpdateMatrix();

	levelData = std::make_unique<LoadLevelEditor>();
	levelData->Initialize("MiddleBossStage",Vector3(0,0,0));

	player_ = std::make_unique<Player>();
	player_->Initialize(Vector3(0,20,0),viewProjection_.get(),audioManager);

	player_->SetCameraModeNotFree(false);

	gameCamera = std::make_unique<GameCamera>(WinApp::window_width,WinApp::window_height);
	gameCamera->Initialize(viewProjection_.get(),MyMath::GetAngle(180.0f),player_->GetPlayerPos());

	gameCamera->SetFreeCamera(false);
	gameCamera->SetCameraMode(false);

	middleBossEnemy = std::make_unique<MiddleBossEnemy>(audioManager);
	middleBossEnemy->Initialize(player_.get());

	sceneManager_ = SceneManager::GetInstance();
	collisionManager = CollisionManager::GetInstance();

	clearUI = std::make_unique<ClearUI>();
	clearUI->Initialize();

	DustParticle = std::make_unique<ParticleEditor>();
	DustParticle->Initialize("FieldParticle");
	DustParticle->SetTextureHandle(TextureManager::Load("sprite/effect4.png"));


	for ( int i = 0; i < SafeColCount; i++ )
	{
		Vector3 BillPos = levelData->GetBillPosition(i);
		BillPos.y = UltEndPos.y;
		ColliderPos[ i ] = BillPos + ((levelData->GetBillPosition(i) - UltEndPos ).norm() * SafeRadius);
		SafeCollider[ i ] = new SphereCollider(Vector4(FloatNumber(fNumbers::fZero),SafeRadius,FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fZero)),SafeRadius);
		CollisionManager::GetInstance()->AddCollider(SafeCollider[ i ]);
		SafeCollider[ i ]->SetAttribute(COLLISION_ATTR_ENEMY_ULTSAFEZONE);
		SafeCollider[ i ]->Update(MyMath::Translation(ColliderPos[ i ]));
	}
	Pos.Initialize();
	Pos.translation_ = ColliderPos[ 0 ];
	Pos.scale_ = Vector3(SafeRadius,SafeRadius,SafeRadius);
	Pos.TransferMatrix();
	model_.reset(Model::CreateFromOBJ("sphereBulletEnemy",true));

	TenToZero = std::make_unique<SerialNumber>();
	TenToZero->SetAllContent(Territory,TenToZeroUIPos,SpriteSize);
	TenToZero->Initialize(2);

	Minutes = std::make_unique<SerialNumber>();
	Minutes->SetAllContent(Territory,MinutesUIPos,SpriteSize);
	Minutes->Initialize(2);

	Tenten = Sprite::Create(TextureManager::Load("sprite/Tenten.png"));
	Tenten->SetSize(SpriteSize);

	Framework::Time = 0;

	BGM = audioManager->LoadAudio("Resources/Sound/BGM.mp3",BGMVol,false);
	audioManager->ChangeVolume(BGM,BGMVol);
	audioManager->PlayWave(BGM,true);
}

void MediumBossStage::Update()
{
	audioManager->ChangeVolume(BGM,BGMVol);
	player_->SetCameraModeNotFree(true);
	player_->SetCameraNeedInformation(gameCamera->GetCameraAngle(),gameCamera->GetEyeToTagetVecDistance(120.0f),gameCamera->GetCameraDistanse(),gameCamera->GetMaxDistance());
	player_->Update();

	gameCamera->SetPlayerPosition(player_->GetPlayerPos());
	gameCamera->Update();

	if ( VsBoss == false )
	{
		VsBoss = middleBossEnemy->MovieUpdate(start,end);
	}
	else
	{
		middleBossEnemy->Update();
	}

	if ( player_->GetPlayerPos().y < -250.0f )
	{
		sceneManager_->ChangeScene("STAGE2");
	}

	if ( middleBossEnemy->GetIsDead() )
	{
		if ( !clearUI->Update() )
		{

		}
		else
		{
			if ( SpriteAlpha < 1 )
			{
				SpriteAlpha += AddSpriteAlpha;
			}
			else
			{
				sceneManager_->ChangeScene("ClearScene");
			}
		}
	}
	else
	{
		if ( player_->GetFinishDieDirection() == false )
		{
			if ( SpriteAlpha > 0 )
			{
				SpriteAlpha -= DownSpriteAlpha;
			}
		}
		else
		{
			if ( SpriteAlpha < 1 )
			{
				SpriteAlpha += AddSpriteAlpha;
			}
			else
			{
				sceneManager_->ChangeScene("STAGESELECT");
			}
		}
	}
	//isSlowGame = false;
	if ( middleBossEnemy->GetIsDieMotion() )
	{
		//isSlowGame = true;
	}
	isSlowGame = true;

	LockOn();

	player_->AttackUpdate(middleBossEnemy->GetPosition(),isLockOn);

	if ( middleBossEnemy->GetIsUltExplosion() )
	{
		isUltExplosion = false;
		if ( intensity < MaxIntensity )
		{
			intensity += IntensityPow;
		}
	}
	else
	{
		isUltExplosion = true;
		if ( intensity > 0 )
		{
			intensity -= IntensityBackPow;
		}
		else
		{
			intensity = 0;
		}
	}

	//全ての衝突をチェック
	collisionManager->CheckAllCollisions();

	if ( middleBossEnemy->GetIsUlting() )
	{
		if ( AmbientColor > MinAmbientColor )
		{
			AmbientColor -= AmbientPow;
		}
	}
	else
	{
		if ( AmbientColor < MaxAmbientColor )
		{
			AmbientColor += AmbientPow;
		}
	}
	LightData::GetInstance()->SetAmbientColor(AmbientColor);
	LightData::GetInstance()->Update();

	float DidSeconds = static_cast< float >( Framework::Time / 60 );
	float MinutesAddSprite = static_cast< float >( DidSeconds / 60 );
	float TenToZeroNum = static_cast< float >( static_cast< uint32_t >( DidSeconds ) % 60 );

	Minutes->Update(MinutesAddSprite);
	TenToZero->Update(TenToZeroNum);

	//ImGui::Begin("Pos");

	//ImGui::SliderFloat("ClearTimePosX",&TenToZeroUIPos.x,0,1000);
	//ImGui::SliderFloat("ClearTimePosY",&TenToZeroUIPos.y,0,500);

	//ImGui::End();

	Vector3 forwardVector = ( viewProjection_->target - viewProjection_->eye ).norm();
	Vector3	toCameraVector = ( middleBossEnemy->GetUltPos() - viewProjection_->eye ).norm();

	float dotProduct = forwardVector.dot(toCameraVector);

	center = MyMath::GetWindowPos(viewProjection_->matView,viewProjection_->matProjection,WinApp::GetInstance()->GetWindowSize(),middleBossEnemy->GetUltPos(),dotProduct,isInversion);
	float sendIntensity = intensity;

	if ( isInversion )
	{
		sendIntensity = sendIntensity * -1;
	}

	RadialBlurPostEffect::SetRadialBlur(center,sendIntensity,samples);
}

void MediumBossStage::PostEffectDraw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	PostEffect::PreDrawScene(commandList);
	PostEffect::SetShadeNumber(shadeNumber);
	PostEffect::SetKernelSize(range);
	PostEffect::SetAngle(angle,angle2);

	Model::PreDraw(commandList);//// 3Dオブジェクト描画前処理

	skydome->Draw(*viewProjection_.get());
	levelData->Draw(*viewProjection_.get());

	middleBossEnemy->Draw(*viewProjection_.get());
	player_->Draw();

	//model_->Draw(Pos,*viewProjection_.get());

	Model::PostDraw();//3Dオブジェクト描画後処理

	middleBossEnemy->FbxDraw(*viewProjection_.get());
	player_->FbxDraw();

	levelData->ParticleDraw(*viewProjection_.get());
	middleBossEnemy->ParticleDraw(*viewProjection_.get());
	player_->ParticleDraw();
	DustParticle->Draw(*viewProjection_.get());

	//middleBossEnemy->DrawSprite(*viewProjection_.get());
	//player_->DrawSprite();

	//clearUI->Draw();

	sprite_->Draw({ 640,360 },{ 1,1,1,SpriteAlpha });

	PostEffect::PostDrawScene();
}

void MediumBossStage::BackgroundDraw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	Model::PreDraw(commandList);//// 3Dオブジェクト描画前処理

	levelData->Draw(*viewProjection_.get());

	middleBossEnemy->Draw(*viewProjection_.get());
	player_->Draw();

	Model::PostDraw();//3Dオブジェクト描画後処理

	//middleBossEnemy->FbxDraw(*viewProjection_.get());
	//player_->FbxDraw();
}

void MediumBossStage::Draw()
{
	//// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);//// 3Dオブジェクト描画前処理

	//skydome->Draw(*viewProjection_.get());
	//levelData->Draw(*viewProjection_.get());

	//middleBossEnemy->Draw(*viewProjection_.get());
	//player_->Draw();

	//model_->Draw(Pos,*viewProjection_.get());

	Model::PostDraw();//3Dオブジェクト描画後処理

	//middleBossEnemy->FbxDraw(*viewProjection_.get());
	//player_->FbxDraw();

	//middleBossEnemy->ParticleDraw(*viewProjection_.get());
	//player_->ParticleDraw();
	//DustParticle->Draw(*viewProjection_.get());

	middleBossEnemy->DrawSprite(*viewProjection_.get());
	player_->DrawSprite();

	TenToZero->Draw();
	Tenten->Draw(TenToZeroUIPos - TentenMinus,Vector4(1,1,1,1));
	Minutes->Draw();

	clearUI->Draw();

	sprite_->Draw({ 640,360 },{ 1,1,1,SpriteAlpha });
}

void MediumBossStage::Finalize()
{
}


void MediumBossStage::CSUpdate()
{
	levelData->CSUpdate(DirectXCore::GetInstance()->GetCommandList());
	middleBossEnemy->CSUpdate(DirectXCore::GetInstance()->GetCommandList());
	player_->CSUpdate(DirectXCore::GetInstance()->GetCommandList());
	DustParticle->CSUpdate(DirectXCore::GetInstance()->GetCommandList(),static_cast< uint32_t >( isUltExplosion ),static_cast<uint32_t>(middleBossEnemy->GetIsUltChargeFin()),middleBossEnemy->GetUltPreparationPosition(),middleBossEnemy->GetUltParticleSpeed());
}

bool MediumBossStage::IsBreak()
{
	return isFinishGame;
}

bool MediumBossStage::IsSlow()
{
	return isSlowGame;
}

void MediumBossStage::LockOn()
{
	Vector3 EnemyPos = middleBossEnemy->GetPosition();

	Vector3 forwardVector = ( viewProjection_->target - viewProjection_->eye ).norm();
	Vector3	toCameraVector = ( EnemyPos - viewProjection_->eye ).norm();

	float dotProduct = forwardVector.dot(toCameraVector);

	if ( !middleBossEnemy->GetIsDead() )
	{
		if ( isLockOn == false )
		{
			if ( dotProduct > 0 )
			{
				Vector2 windowWH = Vector2(WinApp::GetInstance()->GetWindowSize().x,WinApp::GetInstance()->GetWindowSize().y);

				//ビューポート行列
				Matrix4 Viewport =
				{ windowWH.x / 2,0,0,0,
				0,-windowWH.y / 2,0,0,
				0,0,1,0,
				windowWH.x / 2, windowWH.y / 2,0,1 };

				//ビュー行列とプロジェクション行列、ビューポート行列を合成する
				Matrix4 matView = viewProjection_->matView;
				Matrix4 matProjection = viewProjection_->matProjection;

				Matrix4 matViewProjectionViewport = matView * matProjection * Viewport;

				//ワールド→スクリーン座標変換(ここで3Dから2Dになる)
				EnemyPos = MyMath::DivVecMat(EnemyPos,matViewProjectionViewport);

				if ( ( 0 < EnemyPos.x && EnemyPos.x < WinApp::GetInstance()->GetWindowSize().x ) &&
					( 0 < EnemyPos.y && EnemyPos.y < WinApp::GetInstance()->GetWindowSize().y ) )
				{
					isLockOn = true;
					player_->SetReticlePosition(Vector2(EnemyPos.x,EnemyPos.y));
				}
				else
				{
					isLockOn = false;
				}
			}
			else
			{
				isLockOn = false;
			}
		}
		else
		{
			if ( dotProduct > 0 )
			{
				Vector2 windowWH = Vector2(WinApp::GetInstance()->GetWindowSize().x,WinApp::GetInstance()->GetWindowSize().y);

				//ビューポート行列
				Matrix4 Viewport =
				{ windowWH.x / 2,0,0,0,
				0,-windowWH.y / 2,0,0,
				0,0,1,0,
				windowWH.x / 2, windowWH.y / 2,0,1 };

				//ビュー行列とプロジェクション行列、ビューポート行列を合成する
				Matrix4 matView = viewProjection_->matView;
				Matrix4 matProjection = viewProjection_->matProjection;

				Matrix4 matViewProjectionViewport = matView * matProjection * Viewport;

				//ワールド→スクリーン座標変換(ここで3Dから2Dになる)
				EnemyPos = MyMath::DivVecMat(EnemyPos,matViewProjectionViewport);

				if ( ( 0 < EnemyPos.x && EnemyPos.x < WinApp::GetInstance()->GetWindowSize().x ) &&
					( 0 < EnemyPos.y && EnemyPos.y < WinApp::GetInstance()->GetWindowSize().y ) )
				{
					isLockOn = true;
					player_->SetReticlePosition(Vector2(EnemyPos.x,EnemyPos.y));
				}
				else
				{
					isLockOn = false;
				}
			}
			else
			{
				isLockOn = false;
			}
		}
	}
	else
	{
		isLockOn = false;
		player_->SetReticlePosition(Vector2(WinApp::GetInstance()->GetWindowSize().x / 2,WinApp::GetInstance()->GetWindowSize().y / 2));
	}
}
