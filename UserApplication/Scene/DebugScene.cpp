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

#include <iostream>
#include <filesystem>

DebugScene::DebugScene() {}
DebugScene::~DebugScene() {
	LightData::GetInstance()->ClearLight();
}

// 指定ディレクトリのファイルリストを取得
std::vector<std::string> getFileList(const std::filesystem::path& directory) {
	std::vector<std::string> fileList;
	for ( const auto& entry : std::filesystem::directory_iterator(directory) )
	{
		if ( entry.is_regular_file() )
		{
			fileList.push_back(entry.path().filename().string());
		}
	}
	return fileList;
}

void DebugScene::Initialize() {

	dxCommon_ = DirectXCore::GetInstance();
	winApp_ = WinApp::GetInstance();
	input_ = Input::GetInstance();

	viewProjection_ = std::make_unique<ViewProjection>();
	viewProjection_->Initialize();
	viewProjection_->eye = { 0,5,-100 };
	viewProjection_->UpdateMatrix();

	worldTransform_.Initialize();
	worldTransform_.scale_ = Vector3(100, 100, 100);

	int MaxParticleCountB = 50000;
	particleEditor = std::make_unique<ParticleEditor>();
	particleEditor->Initialize(MaxParticleCountB);
	particleEditor->SetTextureHandle(TextureManager::Load("sprite/effect4.png"));
}

void DebugScene::Update() {

	worldTransform_.TransferMatrix();

	particleEditor->EditUpdate();

	ImGui::Begin("PostEffect");
	ImGui::SetCursorPos(ImVec2(900,20));
	if ( shadeNumber == 0 )
	{
		ImGui::Text("Nothing");
		ImGui::SliderInt("shadeNumber",&shadeNumber,0,3);

	}
	else if ( shadeNumber == 1 )
	{
		ImGui::Text("averageBlur");
		ImGui::SliderInt("shadeNumber",&shadeNumber,0,3);

		ImGui::SliderInt("range",&range,0,20);
	}
	else if ( shadeNumber == 2 )
	{
		ImGui::Text("RadialBlurBlur");
		ImGui::SliderInt("shadeNumber",&shadeNumber,0,3);

		ImGui::SliderFloat("centerX",&center.x,0,1);
		ImGui::SliderFloat("centerY",&center.y,0,1);
		ImGui::SliderFloat("intensity",&intensity,0,1);
		ImGui::SliderInt("samples",&samples,0,20);
	}
	else if ( shadeNumber == 3 )
	{
		ImGui::Text("RadialBlurBlur");
		ImGui::SliderInt("shadeNumber",&shadeNumber,0,3);
	}
	else if ( shadeNumber == 4 )
	{
		ImGui::Text("CloseFilta");
		ImGui::SliderInt("shadeNumber",&shadeNumber,0,4);
		ImGui::SliderFloat("angle",&angle,0.0f,180.0f);
		ImGui::SliderFloat("angle2",&angle2,0.0f,180.0f);
	}

	ImGui::SliderFloat("CameraPosZ",&viewProjection_->eye.z,-2000,0);
	ImGui::End();

	viewProjection_->UpdateMatrix();
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

	particleEditor->Draw(*viewProjection_);

	PostEffect::PostDrawScene();
}

void DebugScene::CSUpdate()
{
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	particleEditor->CSUpdate(commandList,MyMath::Vec3ToVec4(Vector3(0,-2,0)));
}

bool DebugScene::IsBreak()
{
	return isFinishGame;
}

bool DebugScene::IsSlow()
{
	return isSlowGame;
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
	//3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画



#pragma endregion
}

void DebugScene::Finalize()
{
}
