#include "MediumBossStage.h"
#include "MyMath.h"
#include <PostEffect.h>
#include <ParticleManager.h>

MediumBossStage::MediumBossStage()
{

}

MediumBossStage::~MediumBossStage()
{
	collisionManager->AllClearCollider();
}

void MediumBossStage::Initialize()
{
	dxCommon_ = DirectXCore::GetInstance();
	winApp_ = WinApp::GetInstance();
	input_ = Input::GetInstance();

	sprite_ = Sprite::Create(TextureManager::Load("sprite/Blackout.png"));

	skydome = std::make_unique<Skydome>();
	skydome->Initialize();

	viewProjection_ = std::make_unique<ViewProjection>();
	viewProjection_->Initialize();
	viewProjection_->eye = { 0,0,-50 };
	viewProjection_->UpdateMatrix();

	levelData = std::make_unique<LoadLevelEditor>();
	levelData->Initialize("stage2",Vector3(0,0,0));

	player_ = std::make_unique<Player>();
	player_->Initialize(Vector3(0,20,0),viewProjection_.get());

	player_->SetFirstMoveSpline(levelData->GetFirstSpline());
	player_->SetCameraModeNotFree(false);

	gameCamera = std::make_unique<GameCamera>(WinApp::window_width,WinApp::window_height);
	gameCamera->Initialize(viewProjection_.get(),MyMath::GetAngle(180.0f),player_->GetPlayerPos());

	gameCamera->SetFreeCamera(false);
	gameCamera->SetCameraMode(false);

	bossEnemy = std::make_unique<BossEnemy>();
	bossEnemy->Initialize(levelData->GetBossSpline()[ 0 ],viewProjection_.get());
	bossEnemy->SetStageMoveSpline(levelData->GetBossSpline());

	middleBossEnemy = std::make_unique<MiddleBossEnemy>();
	middleBossEnemy->Initialize(player_.get());

	sceneManager_ = SceneManager::GetInstance();
	collisionManager = CollisionManager::GetInstance();
}

void MediumBossStage::Update()
{
	player_->SetCameraModeNotFree(true);
	player_->SetCameraNeedInformation(gameCamera->GetCameraAngle(),gameCamera->GetEyeToTagetVecDistance(120.0f),gameCamera->GetCameraDistanse(),gameCamera->GetMaxDistance());
	player_->Update();

	if ( player_->GetHitFirstRail() )
	{
		gameCamera->SetCameraMode(true);
	}

	//if (player_->GetHowReturnSpline(5)) {

	//	sceneManager_->ChangeScene("GAMEPLAY");
	//}

	gameCamera->SetPlayerPosition(player_->GetPlayerPos());
	gameCamera->Update();

	bossEnemy->StagingUpdate();

	if ( VsBoss == false )
	{
	//if (bossEnemy->GetFinishSpline()) {
		Vector3 end = Vector3(0,10,150);
		//if (bossEnemy->GetBodyNoAlpha()) {
		VsBoss = middleBossEnemy->MovieUpdate(bossEnemy->GetSplinePos(),end);
		/*}
	}*/
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
		sceneManager_->ChangeScene("ClearScene");
	}

	if ( player_->GetFinishMove() == false )
	{
		if ( SpriteAlpha > 0 )
		{
			SpriteAlpha -= 0.01f;
		}
	}
	else
	{
		if ( SpriteAlpha < 1 )
		{
			SpriteAlpha += 0.007f;
		}
		else
		{
			sceneManager_->ChangeScene("STAGE2");
		}
	}

	LockOn();

	player_->AttackUpdate(middleBossEnemy->GetPosition(),isLockOn);

	//全ての衝突をチェック
	collisionManager->CheckAllCollisions();
}

void MediumBossStage::PostEffectDraw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	PostEffect::PreDrawScene(commandList);
	PostEffect::SetShadeNumber(shadeNumber);
	PostEffect::SetKernelSize(range);
	PostEffect::SetRadialBlur(center,intensity,samples);
	PostEffect::SetAngle(angle,angle2);

	PostEffect::PostDrawScene();
}

void MediumBossStage::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);//// 3Dオブジェクト描画前処理

	//skydome->Draw(*viewProjection_.get());
	levelData->Draw(*viewProjection_.get());

	bossEnemy->Draw();

	middleBossEnemy->Draw(*viewProjection_.get());
	player_->Draw();

	Model::PostDraw();//3Dオブジェクト描画後処理

	player_->FbxDraw();

	middleBossEnemy->DrawSprite(*viewProjection_.get());
	player_->DrawSprite();

	sprite_->Draw({ 640,360 },{ 1,1,1,SpriteAlpha });
}

void MediumBossStage::Finalize()
{
}


void MediumBossStage::CSUpdate()
{

}

bool MediumBossStage::IsBreak()
{
	return isFinishGame;
}

void MediumBossStage::LockOn()
{
	Vector3 EnemyPos = middleBossEnemy->GetPosition();

	Vector3 forwardVector = ( viewProjection_->target - viewProjection_->eye ).norm();
	Vector3	toCameraVector = ( EnemyPos - viewProjection_->eye ).norm();

	float dotProduct = forwardVector.dot(toCameraVector);

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
			Matrix4 matViewProjectionViewport = viewProjection_->matView * viewProjection_->matProjection * Viewport;

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
			Matrix4 matViewProjectionViewport = viewProjection_->matView * viewProjection_->matProjection * Viewport;

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
