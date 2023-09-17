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

	viewProjection_ = std::make_unique<ViewProjection>();
	viewProjection_->Initialize();
	viewProjection_->eye = { 0,0,-50 };
	viewProjection_->UpdateMatrix();

	levelData = std::make_unique<LoadLevelEditor>();
	levelData->Initialize("stageSelect");

	player_ = std::make_unique<Player>();
	player_->Initialize(Vector3(0, 20, 0), viewProjection_.get());

	player_->SetFirstMoveSpline(levelData->GetFirstSpline());
	player_->SetSpline(levelData->GetStage2SropSpline());
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
	player_->SetCameraRot(gameCamera->GetCameraAngle());
	player_->SetEyeToTagetVecDistance(gameCamera->GetEyeToTagetVecDistance(80.0f));
	player_->SetCameraDistance(gameCamera->GetCameraDistanse());
	player_->SetCameraMaxDistance(gameCamera->GetMaxDistance());
	player_->Update();

	if (player_->GetHitFirstRail()) {
		gameCamera->SetCameraMode(true);
	}
	if (player_->GetHit2ndRail()) {
		gameCamera->SetCameraMode(true);
	}

	if (player_->GetHowReturnSpline(5)) {

		sceneManager_->ChangeScene("GAMEPLAY");
	}
	if (player_->GetHowReturnSpline2ndRail(6)) {

		sceneManager_->ChangeScene("STAGE2");
	}

	if (player_->GetPlayerPos().y < -250.0f) {
		sceneManager_->ChangeScene("STAGESELECT");
	}
	gameCamera->SetPlayerPosition(player_->GetPlayerPos());
	gameCamera->Update();

	//全ての衝突をチェック
	collisionManager->CheckAllCollisions();
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

	////パーティクル
	ParticleManager::PreDraw(commandList);
	player_->ParticleDraw(*viewProjection_.get());
	ParticleManager::PostDraw();

	PostEffect::PostDrawScene();
}

void StageSelect::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	
	Model::PreDraw(commandList);//// 3Dオブジェクト描画前処理

	//skydome->Draw(*viewProjection_.get());

	levelData->Draw(*viewProjection_.get());


	player_->Draw(*viewProjection_.get());
	
	Model::PostDraw();//3Dオブジェクト描画後処理

	player_->DrawSprite(*viewProjection_.get());

}

void StageSelect::Finalize()
{
}

void StageSelect::CopyData()
{
	////パーティクル
	player_->CopyParticle();
}

void StageSelect::CSUpdate()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	////パーティクル
	player_->CSUpdate(commandList);
}
