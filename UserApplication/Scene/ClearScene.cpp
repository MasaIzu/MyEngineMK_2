#include "ClearScene.h"

void ClearScene::Initialize()
{
	input = Input::GetInstance();

	loserTexture_ = TextureManager::Load("sprite/Clear.png");
	sprite_ = Sprite::Create(loserTexture_);
	sceneManager_ = SceneManager::GetInstance();

}


void ClearScene::Update()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		sceneManager_->ChangeScene("STAGESELECT");
	}
}

void ClearScene::PostEffectDraw()
{
}

void ClearScene::Draw()
{
	sprite_->Draw({ 640,360 }, { 1,1,1,1 });
}

void ClearScene::Finalize()
{
}

void ClearScene::CopyData()
{
}

void ClearScene::CSUpdate()
{
}

bool ClearScene::IsBreak()
{
	return isFinishGame;
}
