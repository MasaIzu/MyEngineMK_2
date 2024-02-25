#include "TitleScene.h"
#include <imgui.h>
#include"PostEffect.h"
#include <Explosion.h>
#include <Easing.h>
#include <LightData.h>


TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
	LightData::GetInstance()->ClearLight();
	CollisionManager::GetInstance()->AllClearCollider();
}

void TitleScene::Initialize()
{
	dxCommon_ = DirectXCore::GetInstance();
	winApp_ = WinApp::GetInstance();
	input_ = Input::GetInstance();
	audioManager = AudioManager::GetInstance();

	sprite_ = Sprite::Create(TextureManager::Load("sprite/Blackout.png"));
	TitleSprite_ = Sprite::Create(TextureManager::Load("sprite/TitleGameName.png"));
	StartSprite_ = Sprite::Create(TextureManager::Load("sprite/start.png"));
	EndSprite_ = Sprite::Create(TextureManager::Load("sprite/Exit.png"));
	OutLineSprite_ = Sprite::Create(TextureManager::Load("sprite/OutLine.png"));
	PushSprite = Sprite::Create(TextureManager::Load("sprite/Push.png"));
	PushSprite->SetSize(PushSpriteSize);
	SpaceSprite = Sprite::Create(TextureManager::Load("sprite/Spacekey.png"));
	SpaceSprite->SetSize(SpaceSpriteSize);

	ASprite = Sprite::Create(TextureManager::Load("sprite/ABotan.png"));
	ASprite->SetSize(ASpriteSize);
	StartSprite_->SetSize(Vector2(100,50));
	EndSprite_->SetSize(Vector2(100,50));
	OutLineSprite_->SetSize(Vector2(100,50));

	sceneManager_ = SceneManager::GetInstance();

	viewProjection_ = std::make_unique<ViewProjection>();
	viewProjection_->Initialize();
	viewProjection_->eye = { 0,0,-200 };
	viewProjection_->UpdateMatrix();

	levelData = std::make_unique<LoadLevelEditor>();
	levelData->Initialize("TitleStage",Vector3(0,0,0));

	levelData_ = std::make_unique<LoadLevelEditor>();
	levelData_->Initialize("TitleStage2",Vector3(0,0,0));

	tutorialEnemyList = levelData->GetTutorialEnemyList();
	bulletShotEnemy = levelData->GetBulletShotEnemyList();

	FirstCameraPoints = levelData->GetCameraSpline();
	SecondCameraPoints = levelData_->GetCameraSpline();

	middleBossEnemy = std::make_unique<MiddleBossEnemy>(audioManager);
	for ( TutorialEnemy* enemy : tutorialEnemyList )
	{
		enemy->Initialize();
	}
	for ( BulletShotEnemy* enemy : bulletShotEnemy )
	{
		enemy->Initialize();
	}
	DebugTrans.Initialize();

	model.reset(Model::CreateFromOBJ("ken",true));
	//model1.reset(Model::CreateFromOBJ("Ground", true));
	model_.reset(Model::CreateFromOBJ("stageTest",true));


	BlackoutPreliminaryTime = MaxBlackoutPreliminaryTime;

	EnemyStartPos = levelData_->GetBossBonePos() + Vector3(0,100,0);
	EnemyEndPos = levelData_->GetBossBonePos() + Vector3(0,10,0);

	viewProjection_->UpdateMatrix();

	movieCameraCount = MovieCameraCount::FirstCamera;


	if ( movieCameraCount == MovieCameraCount::FirstCamera )
	{
		viewProjection_->target = FirstCameraPoints[ 0 ];
		viewProjection_->eye = FirstCameraPoints[ 0 ];
	}
	else if ( movieCameraCount == MovieCameraCount::SecondCamera )
	{
		viewProjection_->target = levelData_->GetBossBonePos();
		viewProjection_->eye = SecondCameraPoints[ 0 ];
	}
	skydome = std::make_unique<Skydome>();
	skydome->Initialize();
}

void TitleScene::Update()
{

	PushSprite->SetSize(PushSpriteSize);
	SpaceSprite->SetSize(SpaceSpriteSize);
	ASprite->SetSize(ASpriteSize);

	CameraPos.z = 5.0f;

	for ( TutorialEnemy* enemy : tutorialEnemyList )
	{
		enemy->Update(Vector3(0,0,-50));
	}
	for ( BulletShotEnemy* enemy : bulletShotEnemy )
	{
		enemy->Update(Vector3(0,0,-50));
	}

	CameraUpdate();

	viewProjection_->UpdateMatrix();

	if ( input_->TriggerKey(DIK_W) || input_->TriggerKey(DIK_UP) )
	{
		OutLinePos = OutLinePosition1;
	}
	else if ( input_->TriggerKey(DIK_S) || input_->TriggerKey(DIK_DOWN) )
	{
		OutLinePos = OutLinePosition2;
	}

	if ( input_->TriggerKey(DIK_SPACE) || input_->ButtonInput(A) )
	{
		if ( OutLinePos == OutLinePosition1 )
		{
			isStartChangeScene = true;
		}
		else if ( OutLinePos == OutLinePosition2 )
		{
			isFinishGame = true;
		}
	}

	if ( isStartChangeScene )
	{
		if ( SpriteAlpha < 1 )
		{
			SpriteAlpha += 0.02f;
		}
		else
		{
			sceneManager_->ChangeScene("STAGESELECT");
		}
	}

	LightData::GetInstance()->Update();
}

void TitleScene::PostEffectDraw()
{
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	PostEffect::PreDrawScene(commandList);
	PostEffect::SetShadeNumber(shadeNumber);
	PostEffect::SetKernelSize(range);
	PostEffect::SetRadialBlur(center,intensity,samples);
	PostEffect::SetAngle(angle,angle2);

	PostEffect::PostDrawScene();
}

void TitleScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	//sprite_->Draw({640,360},{1,1,1,1});

	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();

	//// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);
	skydome->Draw(*viewProjection_.get());
	if ( movieCameraCount == MovieCameraCount::FirstCamera )
	{
		levelData->Draw(*viewProjection_.get());
		for ( TutorialEnemy* enemy : tutorialEnemyList )
		{
			enemy->Draw(*viewProjection_.get());
		}
		for ( BulletShotEnemy* enemy : bulletShotEnemy )
		{
			enemy->Draw(*viewProjection_.get());
		}
	}
	else if ( movieCameraCount == MovieCameraCount::SecondCamera )
	{
		levelData_->Draw(*viewProjection_.get());

		middleBossEnemy->Draw(*viewProjection_.get());

	}


	//3Dオブジェクト描画後処理
	Model::PostDraw();

	if ( movieCameraCount == MovieCameraCount::SecondCamera )
	{
		middleBossEnemy->FbxDraw(*viewProjection_.get());
		middleBossEnemy->ParticleDraw(*viewProjection_.get());
	}

	TitleSprite_->Draw({ 640,200 },{ 1,1,1,1 });
	StartSprite_->Draw({ 640,500 },{ 1,1,1,1 });
	EndSprite_->Draw({ 640,550 },{ 1,1,1,1 });
	OutLineSprite_->Draw({ 640,OutLinePos },{ 1,1,1,1 });

	PushSprite->Draw(PushSpritePos,{ 1,1,1,1 });
	if ( Input::GetInstance()->GetIsControllerConnection() )
	{
		ASprite->Draw(SpaceSpritePos,{ 1,1,1,1 });
	}
	else
	{
		SpaceSprite->Draw(SpaceSpritePos,{ 1,1,1,1 });
	}
	sprite_->Draw({ 640,360 },{ 1,1,1,SpriteAlpha });
}

void TitleScene::Finalize()
{
}

void TitleScene::CSUpdate()
{
	middleBossEnemy->CSUpdate(DirectXCore::GetInstance()->GetCommandList());
}

bool TitleScene::IsBreak()
{
	return isFinishGame;
}

bool TitleScene::IsSlow()
{
	return isSlowGame;
}

void TitleScene::CameraUpdate()
{
	if ( movieCameraCount == MovieCameraCount::FirstCamera )
	{
		if ( isBlackout == true )
		{
			if ( SpriteAlpha > 0.0f )
			{
				SpriteAlpha -= 0.01f;
			}
			else
			{
				isBlackout = false;
			}
		}


		Vector3 goPos = FirstCameraPoints[ CameraPosCount ] - viewProjection_->target;
		goPos.normalize();
		CameraVelocity = MyMath::lerp(CameraVelocity,goPos,0.03f);
		viewProjection_->target += CameraVelocity.norm() * MovieCameraSpeed;

		viewProjection_->eye = MyMath::lerp(viewProjection_->eye,viewProjection_->target,0.02f);

		// 中心点の距離の２乗 <= 半径の和の２乗　なら交差
		Vector3 tmp;
		tmp = FirstCameraPoints[ CameraPosCount ] - viewProjection_->target;
		float dist = tmp.dot(tmp);
		float radius2 = 5.0f;
		radius2 *= radius2;

		if ( dist <= radius2 )
		{
			if ( CameraPosCount < FirstCameraPoints.size() - 1 )
			{
				CameraPosCount++;
			}
		}
		if ( CameraPosCount >= FirstCameraPoints.size() - 1 )
		{
			if ( BlackoutPreliminaryTime > 0 )
			{
				BlackoutPreliminaryTime--;
			}
			else
			{
				SpriteAlpha += 0.01f;
			}
			if ( SpriteAlpha > 1.0f )
			{
				isBlackout = true;
				CameraPosCount = 1;
				movieCameraCount = MovieCameraCount::SecondCamera;
				middleBossEnemy->ResetTitleMove();
				CameraVelocity = { 0,0,0 };
				EasingTime = 0;
				viewProjection_->target = levelData_->GetBossBonePos();
				viewProjection_->eye = SecondCameraPoints[ 0 ];
			}
		}
	}
	else if ( movieCameraCount == MovieCameraCount::SecondCamera )
	{

		if ( isStart == false )
		{
			if ( isBlackout == true )
			{
				if ( SpriteAlpha > 0.0f )
				{
					SpriteAlpha -= 0.01f;
				}
				else
				{
					isBlackout = false;
				}
			}

			middleBossEnemy->TitleMovieUpdate(EnemyStartPos,EnemyEndPos);
			
			Vector3 goPos = SecondCameraPoints[ CameraPosCount ] - viewProjection_->eye;
			goPos.normalize();
			CameraVelocity = MyMath::lerp(CameraVelocity,goPos,0.04f);
			viewProjection_->eye += CameraVelocity.norm() * MovieCameraSpeed;

			// 中心点の距離の２乗 <= 半径の和の２乗　なら交差
			Vector3 tmp;
			tmp = SecondCameraPoints[ CameraPosCount ] - viewProjection_->eye;
			float dist = tmp.dot(tmp);
			float radius2 = 5.0f;
			radius2 *= radius2;

			if ( dist <= radius2 )
			{
				if ( CameraPosCount < SecondCameraPoints.size() - 1 )
				{
					CameraPosCount++;
				}
				else
				{
					isStart = true;
					EasingStartPos = viewProjection_->eye;
					EasingTime = EasingStartTime;
				}
			}
		}
		else
		{
			middleBossEnemy->TitleMovieUpdate(EnemyStartPos,EnemyEndPos);
			Vector3 EasingPos;
			Vector3 EasingEndPos = SecondCameraPoints[ 1 ] + Vector3(0,0,20);
			EasingPos = Easing::EaseInBackVec3(EasingStartPos,EasingEndPos,EasingTime,MaxEasingTime);
			viewProjection_->eye = MyMath::lerp(viewProjection_->eye,EasingPos,0.2f);
			if ( EasingTime < MaxEasingTime )
			{
				if ( EasingTime >= MiddleEnemyBossMakeBulletTime )
				{
					middleBossEnemy->MakeTitleMissileBullet();
				}
				EasingTime++;
			}
			else
			{
				if ( SpriteAlpha >= 1.0f )
				{
					isBlackout = true;
					isStart = false;
					CameraPosCount = 1;
					movieCameraCount = MovieCameraCount::FirstCamera;
					CameraVelocity = { 0,0,0 };
					BlackoutPreliminaryTime = MaxBlackoutPreliminaryTime;
					viewProjection_->eye = FirstCameraPoints[ 0 ];
					viewProjection_->target = FirstCameraPoints[ 0 ];
				}
				else
				{
					SpriteAlpha += 0.02f;
				}
			}
		}
	}
	//middleBossEnemy->TitleUpdate(SecondCameraPoints[ 1 ]);

}
