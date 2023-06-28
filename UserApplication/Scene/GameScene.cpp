#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>
#include <fstream>
#include "FbxLoader.h"
#include"ImGuiManager.h"
#include <CollisionAttribute.h>
#include "Collision.h"
#include"PostEffect.h"
#include"WinApp.h"

GameScene::GameScene() {}
GameScene::~GameScene() {
	
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCore::GetInstance();
	winApp_ = WinApp::GetInstance();
	input_ = Input::GetInstance();

	//当たり判定
	collisionManager = CollisionManager::GetInstance();


	sceneManager_ = SceneManager::GetInstance();

	viewProjection_ = std::make_unique<ViewProjection>();
	viewProjection_->Initialize();
	viewProjection_->eye = { 0,0,-50 };
	viewProjection_->UpdateMatrix();

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,-120,100 };
	worldTransform_.rotation_ = { 0,0,0 };
	worldTransform_.scale_ = { 0.4f,0.4f,0.4f };
	worldTransform_.TransferMatrix();

	//fbxModel = std::make_unique<FbxModel>();
	//fbxLoad = FbxLoader::GetInstance();

	//fbxModel.reset(FbxLoader::GetInstance()->LoadModelFromFile("Player"));
	//fbxModel->Initialize();
	//anim = std::make_unique<FbxAnimation>();
	//anim->Load("Player");

	//model_.reset(Model::CreateFromOBJ("UFO", true));

	ParticleMan = std::make_unique<ParticleManager>();
	ParticleMan->Initialize();
	ParticleMan->SetTextureHandle(TextureManager::Load("effect4.png"));

	player_ = std::make_unique<Player>();
	player_->Initialize();

	gameCamera = std::make_unique<GameCamera>(WinApp::window_width, WinApp::window_height);
	gameCamera->Initialize(viewProjection_.get(),MyMath::GetAngle(180.0f), player_->GetPlayerPos());

	ground = std::make_unique<Ground>();
	ground->Initialze();

	tutorialEnemy = std::make_unique<TutorialEnemy>();
	tutorialEnemy->Initialize();

}

void GameScene::Update() {


	ImGui::Begin("Phase");

	ImGui::Text("ParticleSize:%d", ParticleMan->GetParticlesListSize());

	ImGui::End();

	frem += 0.001f;

	//fbxModel->ModelAnimation(frem, anim->GetAnimation(static_cast<int>(0)));

	if (shadeNumber == 0) {
		ImGui::Begin("averageBlur");
		ImGui::SliderInt("shadeNumber", &shadeNumber, 0, 3);

		ImGui::SliderInt("range", &range, 0, 20);
		ImGui::SetCursorPos(ImVec2(0, 20));
		ImGui::End();
	}
	else if (shadeNumber == 1) {
		ImGui::Begin("RadialBlurBlur");
		ImGui::SliderInt("shadeNumber", &shadeNumber, 0, 3);

		ImGui::SliderFloat("centerX", &center.x, 0, 1);
		ImGui::SliderFloat("centerY", &center.y, 0, 1);
		ImGui::SliderFloat("intensity", &intensity, 0, 1);
		ImGui::SliderInt("samples", &samples, 0, 20);
		ImGui::SetCursorPos(ImVec2(0, 20));
		ImGui::End();
	}
	else if (shadeNumber == 2) {
		ImGui::Begin("RadialBlurBlur");
		ImGui::SliderInt("shadeNumber", &shadeNumber, 0, 3);

		ImGui::SetCursorPos(ImVec2(0, 20));
		ImGui::End();
	}
	else if (shadeNumber == 3) {
		ImGui::Begin("CloseFilta");
		ImGui::SliderInt("shadeNumber", &shadeNumber, 0, 3);
		ImGui::SliderFloat("angle", &angle, 0.0f, 180.0f);
		ImGui::SliderFloat("angle2", &angle2, 0.0f, 180.0f);

		ImGui::SetCursorPos(ImVec2(0, 20));
		ImGui::End();
	}

	if (input_->PushKey(DIK_0)) {
		//for (int i = 0; i < 1000; i++) {
			ParticleMan->Add(Pos, Verocty, MaxFream);
		//}
	}
	

	ParticleMan->Update();

	player_->SetCameraRot(gameCamera->GetCameraAngle());
	player_->Update();

	gameCamera->SetCameraPosition(player_->GetPlayerPos());
	gameCamera->Update();

}

void GameScene::PostEffectDraw()
{
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	PostEffect::PreDrawScene(commandList);

	PostEffect::SetShadeNumber(shadeNumber);
	PostEffect::SetKernelSize(range);
	PostEffect::SetRadialBlur(center, intensity, samples);
	PostEffect::SetAngle(angle, angle2);

	Model::PreDraw(commandList);

	ground->Draw(*viewProjection_.get());

	player_->Draw(*viewProjection_.get());

	tutorialEnemy->Draw(*viewProjection_.get());

	Model::PostDraw();

	//FBXモデル
	FbxModel::PreDraw(commandList);
	//fbxModel->Draw(worldTransform_, viewProjection_);
	FbxModel::PostDraw();

	////パーティクル
	ParticleMan->CSUpdate(commandList);
	ParticleManager::PreDraw(commandList);

	ParticleMan->Draw(*viewProjection_.get());


	ParticleManager::PostDraw();

	PostEffect::PostDrawScene();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画

	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	ParticleManager::PreDraw(commandList);

	

	ParticleManager::PostDraw();

	//// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	//model_->Draw(worldTransform_, viewProjection_);

	//ground->Draw(*viewProjection_.get());

	//player_->Draw(*viewProjection_.get());

	//3Dオブジェクト描画後処理
	Model::PostDraw();



	FbxModel::PreDraw(commandList);

	//fbxModel->Draw(worldTransform_, viewProjection_);

	FbxModel::PostDraw();

	//ParticleMan->CSUpdate(commandList);
	//ParticleManager::PreDraw(commandList);
	//
	//ParticleMan->Draw(*viewProjection_.get());
	//

	//ParticleManager::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画

	

#pragma endregion
}

void GameScene::Finalize()
{
}

void GameScene::CopyData()
{

	ParticleMan->CopyData();

}

