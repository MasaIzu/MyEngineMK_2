#include "DebugScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>
#include <fstream>
//#include "FbxLoader.h"
#include"ImGuiManager.h"
#include <CollisionAttribute.h>
#include "Collision.h"
#include"PostEffect.h"
#include"WinApp.h"
#include <LightData.h>


DebugScene::DebugScene() {}
DebugScene::~DebugScene() {
	LightData::GetInstance()->Destroy();
}

void DebugScene::Initialize() {

	dxCommon_ = DirectXCore::GetInstance();
	winApp_ = WinApp::GetInstance();
	input_ = Input::GetInstance();

	viewProjection_ = std::make_unique<ViewProjection>();
	viewProjection_->Initialize();
	viewProjection_->eye = { 0,0,-1000 };
	viewProjection_->UpdateMatrix();

	worldTransform_.Initialize();
	worldTransform_.scale_ = Vector3(100, 100, 100);

	model.reset(Model::CreateFromOBJ("Ground", true));
	model1.reset(Model::CreateFromOBJ("Ground", true));

	levelData = std::make_unique<LoadLevelEditor>();
	levelData->Initialize("MiddleBossStage",Vector3(0,0,0));

	player_ = std::make_unique<Player>();
	player_->Initialize(Vector3(0,20,0),viewProjection_.get());

	player_->SetCameraModeNotFree(false);

	gameCamera = std::make_unique<GameCamera>(WinApp::window_width,WinApp::window_height);
	gameCamera->Initialize(viewProjection_.get(),MyMath::GetAngle(180.0f),player_->GetPlayerPos());

	gameCamera->SetFreeCamera(false);
	gameCamera->SetCameraMode(false);

}

void DebugScene::Update() {

	player_->SetCameraModeNotFree(true);
	player_->SetCameraNeedInformation(gameCamera->GetCameraAngle(),gameCamera->GetEyeToTagetVecDistance(120.0f),gameCamera->GetCameraDistanse(),gameCamera->GetMaxDistance());
	player_->Update();

	gameCamera->SetPlayerPosition(player_->GetPlayerPos());
	gameCamera->Update();

	worldTransform_.TransferMatrix();
	ImGui::Begin("XXXXX");


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

	bool fal = false;
	player_->AttackUpdate(Vector3(0,0,0),fal);

	LightData::GetInstance()->Update();
}

void DebugScene::PostEffectDraw()
{
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	PostEffect::PreDrawScene(commandList);
	PostEffect::SetShadeNumber(shadeNumber);
	PostEffect::SetKernelSize(range);
	PostEffect::SetRadialBlur(center, intensity, samples);
	PostEffect::SetAngle(angle, angle2);

	Model::PreDraw(commandList);


	Model::PostDraw();

	Model::PreDraw(commandList);

	Model::PostDraw();


	PostEffect::PostDrawScene();
}

void DebugScene::CSUpdate()
{

	player_->CSUpdate(DirectXCore::GetInstance()->GetCommandList());
}

bool DebugScene::IsBreak()
{
	return isFinishGame;
}

void DebugScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画

	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();


#pragma endregion

#pragma region 3Dオブジェクト描画

	//// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);
	//model->Draw(worldTransform_,*viewProjection_.get());
	//ground->Draw(*viewProjection_);
	levelData->Draw(*viewProjection_.get());
	//3Dオブジェクト描画後処理
	Model::PostDraw();

	player_->FbxDraw();


#pragma endregion

#pragma region 前景スプライト描画



#pragma endregion
}

void DebugScene::Finalize()
{
}
