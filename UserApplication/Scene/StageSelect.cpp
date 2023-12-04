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


	sprite_ = Sprite::Create(TextureManager::Load("sprite/Blackout.png"));

	viewProjection_ = std::make_unique<ViewProjection>();
	viewProjection_->Initialize();
	
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

void StageSelect::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	
	Model::PreDraw(commandList);//// 3Dオブジェクト描画前処理
	
	Model::PostDraw();//3Dオブジェクト描画後処理

	sprite_->Draw({ 640,360 },{ 1,1,1,SpriteAlpha });
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
