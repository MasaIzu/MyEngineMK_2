#include "ClearScene.h"

void ClearScene::Initialize()
{
	input = Input::GetInstance();

	sprite_ = Sprite::Create(TextureManager::Load("sprite/Blackout.png"));

	Result = Sprite::Create(TextureManager::Load("sprite/Result.png"));

	ResultDetail = Sprite::Create(TextureManager::Load("sprite/ResultDetail.png"));

	PushSprite = Sprite::Create(TextureManager::Load("sprite/Push.png"));
	PushSprite->SetSize(PushSpriteSize);
	SpaceSprite = Sprite::Create(TextureManager::Load("sprite/Spacekey.png"));
	SpaceSprite->SetSize(SpaceSpriteSize);

	ASprite = Sprite::Create(TextureManager::Load("sprite/ABotan.png"));
	ASprite->SetSize(ASpriteSize);

	sceneManager_ = SceneManager::GetInstance();

}


void ClearScene::Update()
{
	if (input->TriggerKey(DIK_SPACE) || input->ButtonInput(A) )
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
	PushSprite->Draw(PushSpritePos,{ 1,1,1,1 });
	if ( Input::GetInstance()->GetIsControllerConnection() )
	{
		ASprite->Draw(SpaceSpritePos,{ 1,1,1,1 });
	}
	else
	{
		SpaceSprite->Draw(SpaceSpritePos,{ 1,1,1,1 });
	}
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
