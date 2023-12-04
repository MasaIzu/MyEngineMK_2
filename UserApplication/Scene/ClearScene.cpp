#include "ClearScene.h"

void ClearScene::Initialize()
{
	input = Input::GetInstance();

	sprite_ = Sprite::Create(TextureManager::Load("sprite/Blackout.png"));

	Result = Sprite::Create(TextureManager::Load("sprite/Result.png"));

	ResultDetail = Sprite::Create(TextureManager::Load("sprite/ResultDetail.png"));

	sceneManager_ = SceneManager::GetInstance();

}


void ClearScene::Update()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		isSceneChange = true;
	}

	if ( !isSceneChange )
	{
		if ( SpriteAlpha > 0 )
		{
			SpriteAlpha -= DownSpriteAlpha;
		}
	}
	else
	{
		if ( SpriteAlpha < 1 )
		{
			SpriteAlpha += AddSpriteAlpha;
		}
		else
		{
			sceneManager_->ChangeScene("STAGESELECT");
		}
	}

}

void ClearScene::PostEffectDraw()
{
}

void ClearScene::Draw()
{
	Result->Draw({ 640,360 }, { 1,1,1,1 });
	ResultDetail->Draw({ 640,400 },{ 1,1,1,1 });
	sprite_->Draw({ 640,360 },{ 1,1,1,SpriteAlpha });
}

void ClearScene::Finalize()
{
}

void ClearScene::CSUpdate()
{
}

bool ClearScene::IsBreak()
{
	return isFinishGame;
}
