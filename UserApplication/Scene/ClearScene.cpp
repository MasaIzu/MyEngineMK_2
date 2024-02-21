#include "ClearScene.h"
#include "Framework.h"
#include <PostEffect.h>

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

	Tenten = Sprite::Create(TextureManager::Load("sprite/Tenten.png"));
	Tenten->SetSize(SpriteSize);

	ClearTime = Sprite::Create(TextureManager::Load("sprite/ClearTime.png"));

	sceneManager_ = SceneManager::GetInstance();

	TenToZero = std::make_unique<SerialNumber>();
	TenToZero->SetAllContent(Territory,TenToZeroUIPos,SpriteSize);
	TenToZero->Initialize(2);

	Minutes = std::make_unique<SerialNumber>();
	Minutes->SetAllContent(Territory,MinutesUIPos,SpriteSize);
	Minutes->Initialize(2);
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

	ImGui::Begin("Pos");

	ImGui::SliderFloat("ClearTimePosX",&ClearTimePos.x,0,1000);
	ImGui::SliderFloat("ClearTimePosY",&ClearTimePos.y,0,500);

	ImGui::End();


	float DidSeconds = static_cast< float >( Framework::Time / 60 );
	float MinutesAddSprite = static_cast< float >( DidSeconds / 60 );
	float TenToZeroNum = static_cast< float >( static_cast< uint32_t >( DidSeconds ) % 60 );

	Minutes->Update(MinutesAddSprite);
	TenToZero->Update(TenToZeroNum);
}

void ClearScene::PostEffectDraw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = DirectXCore::GetInstance()->GetCommandList();
	PostEffect::PreDrawScene(commandList);
	PostEffect::SetShadeNumber(3);


	PostEffect::PostDrawScene();
}

void ClearScene::Draw()
{
	Result->Draw({ 640,360 }, { 1,1,1,1 });
	ResultDetail->Draw({ 640,400 },{ 1,1,1,1 });
	PushSprite->Draw(PushSpritePos,{ 1,1,1,1 });
	TenToZero->Draw();
	Tenten->Draw(TenToZeroUIPos - TentenMinus,Vector4(1,1,1,1));
	Minutes->Draw();
	ClearTime->Draw(ClearTimePos,Vector4(1,1,1,1));
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

bool ClearScene::IsSlow()
{
	return isSlowGame;
}
