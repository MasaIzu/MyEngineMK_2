﻿#include "GameScene.h"
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
	worldTransform_.translation_ = { 0,10,0 };
	worldTransform_.scale_ = { 0.01f,0.01f,0.01f };
	worldTransform_.TransferMatrix();

	ParticleMan = std::make_unique<ParticleManager>();
	ParticleMan->Initialize();
	ParticleMan->SetTextureHandle(TextureManager::Load("effect4.png"));

	player_ = std::make_unique<Player>();
	player_->Initialize();

	gameCamera = std::make_unique<GameCamera>(WinApp::window_width, WinApp::window_height);
	gameCamera->Initialize(viewProjection_.get(), MyMath::GetAngle(180.0f), player_->GetPlayerPos());

	ground = std::make_unique<Ground>();
	ground->Initialze();

	tutorialEnemy = std::make_unique<TutorialEnemy>(Vector3(0, 10, 0));
	tutorialEnemy->Initialize();

	levelData = std::make_unique<LoadLevelEditor>();
	levelData->Initialize("Task");

	collisionManager = CollisionManager::GetInstance();
}

void GameScene::Update() {

	CheckReticle();
	ImGui::Begin("Phase");

	ImGui::Text("ParticleSize:%d", ParticleMan->GetParticlesListSize());

	ImGui::Text("EnemyPos:%f,%f,%f", EnemyPos.x, EnemyPos.y, EnemyPos.z);

	ImGui::End();

	
	if (shadeNumber == 0) {
		ImGui::Begin("Not");
		ImGui::SliderInt("shadeNumber", &shadeNumber, 0, 4);

		ImGui::End();
	}
	else if (shadeNumber == 1) {
		ImGui::Begin("averageBlur");
		ImGui::SliderInt("shadeNumber", &shadeNumber, 0, 4);

		ImGui::SliderInt("range", &range, 0, 20);
		ImGui::SetCursorPos(ImVec2(0, 20));
		ImGui::End();
	}
	else if (shadeNumber == 2) {
		ImGui::Begin("RadialBlurBlur");
		ImGui::SliderInt("shadeNumber", &shadeNumber, 0, 4);

		ImGui::SliderFloat("centerX", &center.x, 0, 1);
		ImGui::SliderFloat("centerY", &center.y, 0, 1);
		ImGui::SliderFloat("intensity", &intensity, 0, 1);
		ImGui::SliderInt("samples", &samples, 0, 20);
		ImGui::SetCursorPos(ImVec2(0, 20));
		ImGui::End();
	}
	else if (shadeNumber == 3) {
		ImGui::Begin("RadialBlurBlur");
		ImGui::SliderInt("shadeNumber", &shadeNumber, 0, 4);

		ImGui::SetCursorPos(ImVec2(0, 20));
		ImGui::End();
	}
	else if (shadeNumber == 4) {
		ImGui::Begin("CloseFilta");
		ImGui::SliderInt("shadeNumber", &shadeNumber, 0, 4);
		ImGui::SliderFloat("angle", &angle, 0.0f, 180.0f);
		ImGui::SliderFloat("angle2", &angle2, 0.0f, 180.0f);

		ImGui::SetCursorPos(ImVec2(0, 20));
		ImGui::End();
	}

	if (input_->PushKey(DIK_0)) {
		ParticleMan->Add(Pos, Verocty, MaxFream);
	}

	ParticleMan->Update();

	player_->SetCameraRot(gameCamera->GetCameraAngle());
	player_->Update();

	gameCamera->SetCameraPosition(player_->GetPlayerPos());
	gameCamera->Update();

	tutorialEnemy->Update(player_->GetPlayerPos());

	//全ての衝突をチェック
	collisionManager->CheckAllCollisions();
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

	Model::PostDraw();

	////パーティクル
	ParticleMan->CSUpdate(commandList);
	player_->CSUpdate(commandList);
	ParticleManager::PreDraw(commandList);
	ParticleMan->Draw(*viewProjection_.get());
	player_->ParticleDraw(*viewProjection_.get());
	ParticleManager::PostDraw();


	Model::PreDraw(commandList);

	Model::PostDraw();



	PostEffect::PostDrawScene();
}

bool GameScene::CheckReticle()
{
	Vector3 EnemyPos = tutorialEnemy->GetTutorialEnemyPos();

	Vector2 windowWH = Vector2(WinApp::GetInstance()->GetWindowSize().x, WinApp::GetInstance()->GetWindowSize().y);

	//ビューポート行列
	Matrix4 Viewport =
	{ windowWH.x / 2,0,0,0,
	0,-windowWH.y / 2,0,0,
	0,0,1,0,
	windowWH.x / 2, windowWH.y / 2,0,1 };

	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4 matViewProjectionViewport = viewProjection_->matView * viewProjection_->matProjection * Viewport;

	//ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	this->EnemyPos = MyMath::DivVecMat(EnemyPos, matViewProjectionViewport);

	return false;
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画

	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();

	player_->DrawSprite(*viewProjection_.get());

#pragma endregion

#pragma region 3Dオブジェクト描画
	ParticleManager::PreDraw(commandList);



	ParticleManager::PostDraw();

	//// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	ground->Draw(*viewProjection_.get());
	player_->Draw(*viewProjection_.get());
	levelData->Draw(*viewProjection_.get());
	tutorialEnemy->Draw(*viewProjection_.get());

	//3Dオブジェクト描画後処理
	Model::PostDraw();


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
	player_->CopyParticle();
}

