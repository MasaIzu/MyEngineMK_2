#include "StageSelect.h"
#include "MyMath.h"

void StageSelect::Initialize()
{
	dxCommon_ = DirectXCore::GetInstance();
	winApp_ = WinApp::GetInstance();
	input_ = Input::GetInstance();

	viewProjection_ = std::make_unique<ViewProjection>();
	viewProjection_->Initialize();
	viewProjection_->eye = { 0,0,-50 };
	viewProjection_->UpdateMatrix();

	levelData = std::make_unique<LoadLevelEditor>();
	levelData->Initialize("stageSelect");

	player_ = std::make_unique<Player>();
	player_->Initialize();

	gameCamera = std::make_unique<GameCamera>(WinApp::window_width, WinApp::window_height);
	gameCamera->Initialize(viewProjection_.get(), MyMath::GetAngle(180.0f), player_->GetPlayerPos());
}

void StageSelect::Update()
{
}

void StageSelect::PostEffectDraw()
{
}

void StageSelect::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	
	Model::PreDraw(commandList);//// 3D�I�u�W�F�N�g�`��O����

	levelData->Draw(*viewProjection_.get());


	player_->Draw(*viewProjection_.get());
	
	Model::PostDraw();//3D�I�u�W�F�N�g�`��㏈��
}

void StageSelect::Finalize()
{
}

void StageSelect::CopyData()
{
}

void StageSelect::CSUpdate()
{
}
