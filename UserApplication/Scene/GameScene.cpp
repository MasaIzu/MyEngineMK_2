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

	viewProjection_.Initialize();
	viewProjection_.UpdateMatrix();

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,-120,100 };
	worldTransform_.rotation_ = { 0,0,0 };
	worldTransform_.scale_ = { 0.4f,0.4f,0.4f };
	worldTransform_.TransferMatrix();

	fbxModel.reset(FbxLoader::GetInstance()->LoadModelFromFile("Player"));
	fbxModel->Initialize();
	anim = std::make_unique<FbxAnimation>();
	anim->Load("Player");

	model_.reset(Model::CreateFromOBJ("UFO", true));

}

void GameScene::Update() {


	/*ImGui::Begin("Phase");

	ImGui::Text("minifishesX:%f", MyMath::GetWorldTransform(minifishes[0].GetWorldTransform().matWorld_).x);
	ImGui::Text("minifishesY:%f", MyMath::GetWorldTransform(minifishes[0].GetWorldTransform().matWorld_).y);
	ImGui::Text("minifishesZ:%f", MyMath::GetWorldTransform(minifishes[0].GetWorldTransform().matWorld_).z);

	ImGui::End();*/

	frem += 0.01;

	fbxModel->ModelAnimation(frem, anim->GetAnimation(static_cast<int>(0)));


	ImGui::Begin("averageBlur");
	ImGui::SliderInt("range", &range, 0, 20);
	ImGui::SetCursorPos(ImVec2(0, 20));
	ImGui::End();
}

void GameScene::PostEffectDraw()
{
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	PostEffect::PreDrawScene(commandList);
	PostEffect::SetKernelSize(range);

	Model::PreDraw(commandList);

	//model_->Draw(worldTransform_, viewProjection_);

	Model::PostDraw();

	FbxModel::PreDraw(commandList);

	fbxModel->Draw(worldTransform_, viewProjection_);

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


#pragma endregion

#pragma region ポストエフェクトの描画


#pragma endregion

#pragma region 前景スプライト描画

	

#pragma endregion
}

void GameScene::Finalize()
{
}

