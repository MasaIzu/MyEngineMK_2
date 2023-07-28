#include "MainGame.h"
#include "SceneFactory.h"


void MainGame::Initialize()
{
	// 基底クラスの初期化処理
	Framework::Initialize();

	sceneFactory_ = std::make_unique<SceneFactory>();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_.get());
	SceneManager::GetInstance()->ChangeScene("STAGESELECT");
}

void MainGame::Finalize()
{
	// 基底クラスの終了処理
	Framework::Finalize();
}

void MainGame::Update()
{
	// 基底クラスの更新処理
	Framework::Update();
}

void MainGame::PostEffectDraw()
{
	sceneManager_->PostEffectDraw();
}

void MainGame::Draw()
{
	// ゲームシーンの描画
	sceneManager_->Draw();
}

void MainGame::CopyData()
{
	sceneManager_->CopyData();
}

void MainGame::CSUpdate()
{
	sceneManager_->CSUpdate();
}
