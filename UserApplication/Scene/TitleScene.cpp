#include "TitleScene.h"

void TitleScene::Initialize()
{
	input = Input::GetInstance();

	loserTexture_ = TextureManager::Load("kyomu.png");
	sprite_=  Sprite::Create(loserTexture_);
	sceneManager_ = SceneManager::GetInstance();

}

void TitleScene::Update()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		sceneManager_->ChangeScene("GAMEPLAY");
	}
}

void TitleScene::PostEffectDraw()
{
}

void TitleScene::Draw()
{
	sprite_->Draw({640,360},{1,1,1,1});
}

void TitleScene::Finalize()
{
}

void TitleScene::CopyData()
{
}

void TitleScene::CSUpdate()
{
}
