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

	skydome = std::make_unique<Skydome>();
	skydome->Initialize();
}

void DebugScene::Update() {

	player_->SetCameraModeNotFree(true);
	player_->SetCameraNeedInformation(gameCamera->GetCameraAngle(),gameCamera->GetEyeToTagetVecDistance(120.0f),gameCamera->GetCameraDistanse(),gameCamera->GetMaxDistance());
	player_->Update();

	gameCamera->SetPlayerPosition(player_->GetPlayerPos());
	gameCamera->Update();

	worldTransform_.TransferMatrix();

	bool fal = false;
	player_->AttackUpdate(Vector3(0,0,0),fal);

	std::string selectedFileName;  // 選択されたファイル名を保存する変数

	 // ウィンドウを作成
	ImGui::Begin("File List");

	std::vector<std::string> fileList = getFileList("Resources/ParticleData/");
	static int selectedIndex = -1;  // 選択されている項目のインデックス

	if ( ImGui::BeginListBox("##filelist") )
	{
		for ( int i = 0; i < fileList.size(); i++ )
		{
			const bool isSelected = ( selectedIndex == i );
			if ( ImGui::Selectable(fileList[ i ].c_str(),isSelected) )
			{
				selectedIndex = i;
				selectedFileName = fileList[ i ];  // 選択されたファイル名を保存
			}

			// 選択された項目を表示領域内にスクロール
			if ( isSelected )
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndListBox();
	}

	ImGui::End();


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
	skydome->Draw(*viewProjection_.get());
	//model->Draw(worldTransform_,*viewProjection_.get());
	//ground->Draw(*viewProjection_);
	levelData->Draw(*viewProjection_.get());
	//3Dオブジェクト描画後処理
	Model::PostDraw();

	player_->FbxDraw();

	player_->ParticleDraw();

#pragma endregion

#pragma region 前景スプライト描画



#pragma endregion
}

void DebugScene::Finalize()
{
}
