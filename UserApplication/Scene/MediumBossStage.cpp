#include "MediumBossStage.h"
#include "MyMath.h"
#include <PostEffect.h>

MediumBossStage::MediumBossStage()
{
}

MediumBossStage::~MediumBossStage()
{
	collisionManager->AllClearCollider();
}

void MediumBossStage::Initialize()
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
	levelData->Initialize("stage2");

	player_ = std::make_unique<Player>();
	player_->Initialize(Vector3(0, 20, 0), viewProjection_.get());

	player_->SetFirstMoveSpline(levelData->GetFirstSpline());
	player_->SetCameraModeNotFree(false);

	gameCamera = std::make_unique<GameCamera>(WinApp::window_width, WinApp::window_height);
	gameCamera->Initialize(viewProjection_.get(), MyMath::GetAngle(180.0f), player_->GetPlayerPos());

	gameCamera->SetFreeCamera(false);
	gameCamera->SetCameraMode(false);

	bossEnemy = std::make_unique<BossEnemy>();
	bossEnemy->Initialize(levelData->GetBossSpline()[0], viewProjection_.get());
	bossEnemy->SetStageMoveSpline(levelData->GetBossSpline());

	sceneManager_ = SceneManager::GetInstance();
	collisionManager = CollisionManager::GetInstance();
}

void MediumBossStage::Update()
{
	player_->SetCameraModeNotFree(true);
	player_->SetCameraRot(gameCamera->GetCameraAngle());
	player_->SetEyeToTagetVecDistance(gameCamera->GetEyeToTagetVecDistance(120.0f));
	player_->SetCameraDistance(gameCamera->GetCameraDistanse());
	player_->SetCameraMaxDistance(gameCamera->GetMaxDistance());
	player_->Update();

	if (player_->GetHitFirstRail()) {
		gameCamera->SetCameraMode(true);
	}

	//if (player_->GetHowReturnSpline(5)) {

	//	sceneManager_->ChangeScene("GAMEPLAY");
	//}

	gameCamera->SetPlayerPosition(player_->GetPlayerPos());
	gameCamera->Update();

	bossEnemy->StagingUpdate();

	if (bossEnemy->GetFinishSpline()) {
		//sceneManager_->ChangeScene("STAGESELECT");
	}

	if (player_->GetPlayerPos().y < -250.0f) {
		sceneManager_->ChangeScene("STAGE2");
	}

	//�S�Ă̏Փ˂��`�F�b�N
	collisionManager->CheckAllCollisions();
}

void MediumBossStage::PostEffectDraw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	PostEffect::PreDrawScene(commandList);
	PostEffect::SetShadeNumber(shadeNumber);
	PostEffect::SetKernelSize(range);
	PostEffect::SetRadialBlur(center, intensity, samples);
	PostEffect::SetAngle(angle, angle2);

	////�p�[�e�B�N��
	ParticleManager::PreDraw(commandList);
	//player_->ParticleDraw(*viewProjection_.get());
	ParticleManager::PostDraw();

	PostEffect::PostDrawScene();
}

void MediumBossStage::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);//// 3D�I�u�W�F�N�g�`��O����

	//skydome->Draw(*viewProjection_.get());
	levelData->Draw(*viewProjection_.get());

	bossEnemy->Draw(*viewProjection_.get());
	player_->Draw(*viewProjection_.get());

	Model::PostDraw();//3D�I�u�W�F�N�g�`��㏈��

	player_->DrawSprite(*viewProjection_.get());

}

void MediumBossStage::Finalize()
{
}

void MediumBossStage::CopyData()
{
	////�p�[�e�B�N��
	player_->CopyParticle();
}

void MediumBossStage::CSUpdate()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	////�p�[�e�B�N��
	player_->CSUpdate(commandList);
}
