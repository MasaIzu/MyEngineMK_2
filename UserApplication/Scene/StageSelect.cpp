#include "StageSelect.h"
#include "MyMath.h"
#include <PostEffect.h>

StageSelect::StageSelect()
{

}

StageSelect::~StageSelect()
{
	collisionManager->AllClearCollider();
}

void StageSelect::Initialize()
{
	dxCommon_ = DirectXCore::GetInstance();
	winApp_ = WinApp::GetInstance();
	input_ = Input::GetInstance();

	skydome = std::make_unique<Skydome>();
	skydome->Initialize();

	sprite_ = Sprite::Create(TextureManager::Load("sprite/Blackout.png"));

	viewProjection_ = std::make_unique<ViewProjection>();
	viewProjection_->Initialize();
	viewProjection_->eye = { 0,0,-50 };
	viewProjection_->UpdateMatrix();

	levelData = std::make_unique<LoadLevelEditor>();
	levelData->Initialize("stageSelect",Vector3(0,0,0));

	player_ = std::make_unique<Player>();
	player_->Initialize(Vector3(0, 20, 0), viewProjection_.get());

	player_->SetCameraModeNotFree(false);

	gameCamera = std::make_unique<GameCamera>(WinApp::window_width, WinApp::window_height);
	gameCamera->Initialize(viewProjection_.get(), MyMath::GetAngle(180.0f), player_->GetPlayerPos());

	gameCamera->SetFreeCamera(false);
	gameCamera->SetCameraMode(false);

	sceneManager_ = SceneManager::GetInstance();
	collisionManager = CollisionManager::GetInstance();
}

void StageSelect::Update()
{
	player_->SetCameraModeNotFree(true);
	player_->SetCameraNeedInformation(gameCamera->GetCameraAngle(),gameCamera->GetEyeToTagetVecDistance(120.0f),gameCamera->GetCameraDistanse(),gameCamera->GetMaxDistance());
	player_->Update();

	if (player_->GetPlayerPos().y < -250.0f) {
		sceneManager_->ChangeScene("STAGESELECT");
	}
	gameCamera->SetPlayerPosition(player_->GetPlayerPos());
	gameCamera->Update();

	//全ての衝突をチェック
	collisionManager->CheckAllCollisions();

	if ( isBlackoutStart == false )
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
			SpriteAlpha += 0.008f;
		}
	}

}

void StageSelect::PostEffectDraw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	PostEffect::PreDrawScene(commandList);
	PostEffect::SetShadeNumber(shadeNumber);
	PostEffect::SetKernelSize(range);
	PostEffect::SetRadialBlur(center, intensity, samples);
	PostEffect::SetAngle(angle, angle2);


	PostEffect::PostDrawScene();
}

void StageSelect::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	
	Model::PreDraw(commandList);//// 3Dオブジェクト描画前処理

	//skydome->Draw(*viewProjection_.get());

	levelData->Draw(*viewProjection_.get());


	player_->Draw();
	
	Model::PostDraw();//3Dオブジェクト描画後処理
	player_->FbxDraw();
	player_->DrawSprite();

	sprite_->Draw({ 640,360 },{ 1,1,1,SpriteAlpha });
}

void StageSelect::Finalize()
{
}


void StageSelect::CSUpdate()
{

}

bool StageSelect::IsBreak()
{
	return isFinishGame;
}
