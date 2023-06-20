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

	viewProjection_->Initialize();
	viewProjection_->eye = { 0,0,-100 };
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

	gameCamera = std::make_unique<GameCamera>();
	gameCamera->Initialize(viewProjection_, WinApp::window_width, WinApp::window_height);
}

void GameScene::Update() {


	ImGui::Begin("Phase");

	ImGui::Text("ParticleSize:%d", ParticleMan->GetParticlesListSize());

	ImGui::End();

	frem += 0.01f;

	//fbxModel->ModelAnimation(frem, anim->GetAnimation(static_cast<int>(0)));

	if (input_->PushKey(DIK_0)) {
		for (int i = 0; i < 1000; i++) {
			ParticleMan->Add(Pos, Verocty, MaxFream);
		}
	}
	

	ParticleMan->Update();

	/*gameCamera->SetCameraPosition(Vector3(0,0,-50));
	gameCamera->Update();*/
}

void GameScene::PostEffectDraw()
{
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	PostEffect::PreDrawScene(commandList);
	PostEffect::SetShadeNumber(shadeNumber);
	PostEffect::SetKernelSize(range);
	PostEffect::SetRadialBlur(center, intensity, samples);

	Model::PreDraw(commandList);

	//model_->Draw(worldTransform_, viewProjection_);

	Model::PostDraw();

	FbxModel::PreDraw(commandList);

	//fbxModel->Draw(worldTransform_, viewProjection_);

	FbxModel::PostDraw();

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


	//3Dオブジェクト描画後処理
	Model::PostDraw();



	FbxModel::PreDraw(commandList);

	//fbxModel->Draw(worldTransform_, viewProjection_);

	FbxModel::PostDraw();

	ParticleMan->CSUpdate(commandList);
	ParticleManager::PreDraw(commandList);
	
	ParticleMan->Draw(*viewProjection_);
	

	ParticleManager::PostDraw();

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

