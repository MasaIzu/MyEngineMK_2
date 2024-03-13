#include "StageSelect.h"
#include "MyMath.h"
#include <PostEffect.h>

StageSelect::StageSelect()
{

}

StageSelect::~StageSelect()
{
	
}

void StageSelect::Initialize()
{
	dxCommon_ = DirectXCore::GetInstance();
	winApp_ = WinApp::GetInstance();
	input_ = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();

	sprite_ = Sprite::Create(TextureManager::Load("sprite/Blackout.png"));
	StageSelectSprite_ = Sprite::Create(TextureManager::Load("sprite/StageSelect1.png"));
	OneToOneSprite_ = Sprite::Create(TextureManager::Load("sprite/1-1_1.png"));
	OneToTwoSprite_ = Sprite::Create(TextureManager::Load("sprite/1-1_2.png"));
	SelectBarSprite_ = Sprite::Create(TextureManager::Load("sprite/SelectBar.png"));
	NowLoadingSprite_ = Sprite::Create(TextureManager::Load("sprite/NowLoading.png"));
	PushSprite = Sprite::Create(TextureManager::Load("sprite/Push.png"));
	PushSprite->SetSize(PushSpriteSize);
	SpaceSprite = Sprite::Create(TextureManager::Load("sprite/Spacekey.png"));
	SpaceSprite->SetSize(SpaceSpriteSize);

	ASprite = Sprite::Create(TextureManager::Load("sprite/ABotan.png"));
	ASprite->SetSize(ASpriteSize);

	viewProjection_ = std::make_unique<ViewProjection>();
	viewProjection_->Initialize();

	LightViewProjection = std::make_unique<ViewProjection>();
	LightViewProjection->Initialize();
	LightViewProjection->eye = { 0,100,0 };
	LightViewProjection->UpdateMatrix();

	isSelectBarTop = false;
	isSelectBarDown = true;
	SelectBarPos = OneToTwoPos;
}

void StageSelect::Update()
{

	if ( isBlackoutStart == false )
	{
		if ( SpriteAlpha > 0 )
		{
			SpriteAlpha -= 0.01f;
		}
	}
	else
	{
		if ( SpriteAlpha < 1 )
		{
			SpriteAlpha += 0.008f;
		}
		else
		{
			if ( isTop )
			{

			}
			else
			{
				NowLoadingAlpha = 1.0f;
				if ( isNext )
				{
					sceneManager_->ChangeScene("STAGE2");
				}
				else
				{
					isNext = true;
				}
			}
		}
	}

	if ( isSelectBarTop == false )
	{
		//if ( input_->TriggerKey(DIK_W) )
		//{
		//	isSelectBarTop = true;
		//	isSelectBarDown = false;
		//	SelectBarPos = OneToOnePos;
		//}
	}
	else
	{
		if ( input_->TriggerKey(DIK_S) )
		{
			isSelectBarTop = false;
			isSelectBarDown = true;
			SelectBarPos = OneToTwoPos;
		}
	}

	if ( input_->TriggerKey(DIK_SPACE) || input_->ButtonInput(A) )
	{
		isBlackoutStart = true;
		if ( isSelectBarTop == false )
		{
			isTop = false;
		}
		else
		{
			isTop = true;
		}
	}

}

void StageSelect::PostEffectDraw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	PostEffect::PreDrawScene(commandList);
	PostEffect::SetShadeNumber(shadeNumber);
	PostEffect::SetKernelSize(range);
	PostEffect::SetRadialBlur(center, intensity, samples);
	PostEffect::SetAngle(angle, angle2);


	PostEffect::PostDrawScene();
}

void StageSelect::BackgroundDraw()
{
}

void StageSelect::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	
	Model::PreDraw(commandList);//// 3Dオブジェクト描画前処理
	
	Model::PostDraw();//3Dオブジェクト描画後処理

	StageSelectSprite_->Draw({ 640,360 },{ 1,1,1,1 });
	SelectBarSprite_->Draw(SelectBarPos,{ 1,1,1,1 });
	//OneToOneSprite_->Draw(OneToOnePos,{ 1,1,1,1 });
	OneToTwoSprite_->Draw(OneToTwoPos,{ 1,1,1,1 });
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
	NowLoadingSprite_->Draw({ 640,360 },{ 1,1,1,NowLoadingAlpha });
}

void StageSelect::Finalize()
{
}

void StageSelect::CSUpdate()
{

}

bool StageSelect::IsBreak()
{
	return isFinishGame;
}

bool StageSelect::IsSlow()
{
	return isSlowGame;
}
