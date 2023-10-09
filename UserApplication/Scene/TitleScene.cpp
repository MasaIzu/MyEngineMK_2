#include "TitleScene.h"
#include <imgui.h>
#include"PostEffect.h"
#include <Explosion.h>
#include <Easing.h>


void TitleScene::Initialize()
{
	dxCommon_ = DirectXCore::GetInstance();
	winApp_ = WinApp::GetInstance();
	input_ = Input::GetInstance();

	sprite_ = Sprite::Create(TextureManager::Load("sprite/Blackout.png"));
	TitleSprite_ = Sprite::Create(TextureManager::Load("sprite/TitleGameName.png"));
	StartSprite_ = Sprite::Create(TextureManager::Load("sprite/start.png"));
	EndSprite_ = Sprite::Create(TextureManager::Load("sprite/Exit.png"));
	OutLineSprite_ = Sprite::Create(TextureManager::Load("sprite/OutLine.png"));

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

	middleBossEnemy = std::make_unique<MiddleBossEnemy>();
	middleBossEnemy->TitleInitialize();
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
	EnemyEndPos = levelData_->GetBossBonePos();

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

}

void TitleScene::Update()
{
	if ( shadeNumber == 0 )
	{
		ImGui::Begin("Not");
		ImGui::SliderInt("shadeNumber",&shadeNumber,0,4);

		ImGui::End();
	}
	else if ( shadeNumber == 1 )
	{
		ImGui::Begin("averageBlur");
		ImGui::SliderInt("shadeNumber",&shadeNumber,0,4);

		ImGui::SliderInt("range",&range,0,20);
		ImGui::SetCursorPos(ImVec2(0,20));
		ImGui::End();
	}
	else if ( shadeNumber == 2 )
	{
		ImGui::Begin("RadialBlurBlur");
		ImGui::SliderInt("shadeNumber",&shadeNumber,0,4);

		ImGui::SliderFloat("centerX",&center.x,0,1);
		ImGui::SliderFloat("centerY",&center.y,0,1);
		ImGui::SliderFloat("intensity",&intensity,0,1);
		ImGui::SliderInt("samples",&samples,0,20);
		ImGui::SetCursorPos(ImVec2(0,20));
		ImGui::End();
	}
	else if ( shadeNumber == 3 )
	{
		ImGui::Begin("RadialBlurBlur");
		ImGui::SliderInt("shadeNumber",&shadeNumber,0,4);

		ImGui::SetCursorPos(ImVec2(0,20));
		ImGui::End();
	}
	else if ( shadeNumber == 4 )
	{
		ImGui::Begin("CloseFilta");
		ImGui::SliderInt("shadeNumber",&shadeNumber,0,4);
		ImGui::SliderFloat("angle",&angle,0.0f,180.0f);
		ImGui::SliderFloat("angle2",&angle2,0.0f,180.0f);

		ImGui::SetCursorPos(ImVec2(0,20));
		ImGui::End();
	}
	ImGui::Begin("CloseFilta");

	ImGui::Text("CameraPosCount %d",CameraPosCount);

	ImGui::End();


	//if (input_->TriggerKey(DIK_SPACE))
	//{
	//	sceneManager_->ChangeScene("STAGESELECT");
	//}

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

	if ( input_->TriggerKey(DIK_SPACE) )
	{
		if ( OutLinePos == OutLinePosition1 )
		{
			sceneManager_->ChangeScene("STAGESELECT");
		}
		else if ( OutLinePos == OutLinePosition2 )
		{
			isFinishGame = true;
		}
	}

}

void TitleScene::PostEffectDraw()
{
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	PostEffect::PreDrawScene(commandList);
	PostEffect::SetShadeNumber(shadeNumber);
	PostEffect::SetKernelSize(range);
	PostEffect::SetRadialBlur(center,intensity,samples);
	PostEffect::SetAngle(angle,angle2);

	Model::PreDraw(commandList);


	Model::PostDraw();

	////パーティクル
	Explosion::PreDraw(commandList);

	Explosion::PostDraw();


	Model::PreDraw(commandList);

	Model::PostDraw();

	MeshParticle::PreDraw(commandList);
	MeshParticle::PostDraw();

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

	//model_->Draw(DebugTrans,*viewProjection_.get());

	//3Dオブジェクト描画後処理
	Model::PostDraw();

	TitleSprite_->Draw({ 640,200 },{ 1,1,1,1 });
	StartSprite_->Draw({ 640,500 },{ 1,1,1,1 });
	EndSprite_->Draw({ 640,550 },{ 1,1,1,1 });
	OutLineSprite_->Draw({ 640,OutLinePos },{ 1,1,1,1 });

	sprite_->Draw({ 640,360 },{ 1,1,1,SpriteAlpha });

}

void TitleScene::Finalize()
{
}

void TitleScene::CopyData()
{
}

void TitleScene::CSUpdate()
{

}

bool TitleScene::IsBreak()
{
	return isFinishGame;
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

			middleBossEnemy->MovieUpdate(EnemyStartPos,EnemyEndPos);

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
			Vector3 EasingPos;
			Vector3 EasingEndPos = SecondCameraPoints[ 1 ] + Vector3(0,0,20);
			EasingPos = Easing::EaseInBackVec3(EasingStartPos,EasingEndPos,EasingTime,MaxEasingTime);
			viewProjection_->eye = MyMath::lerp(viewProjection_->eye,EasingPos,0.2f);
			if ( EasingTime < MaxEasingTime )
			{
				if ( EasingTime >= MiddleEnemyBossMakeBulletTime )
				{
					middleBossEnemy->MakeMissileBullet();
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
	middleBossEnemy->TitleUpdate(SecondCameraPoints[ 1 ]);

}
