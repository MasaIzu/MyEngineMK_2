#include "PlayerDamageHitUI.h"
#include <TextureManager.h>
#include <MyMath.h>
#include "Numbers.h"

PlayerDamageHitUI::PlayerDamageHitUI()
{
}

PlayerDamageHitUI::~PlayerDamageHitUI()
{
}

void PlayerDamageHitUI::Initialize()
{
	NoiseSprite = Sprite::Create(TextureManager::Load("sprite/yokoNoise.png"));
	MaxSize = WinApp::GetInstance()->GetWindowSize();
}

void PlayerDamageHitUI::Update()
{
	if ( isMakeNoise )
	{
		isMakeNoise = false;
		isUpdateNoise = true;
	}
	if ( isUpdateNoise )
	{
		if ( DrawTime < MaxDrawTime )
		{
			DrawTime++;
		}
		else
		{
			isUpdateNoise = false;
		}
	}
}

void PlayerDamageHitUI::Draw()
{
	if ( isUpdateNoise )
	{
		//for ( auto&& Pos : NoisePos )
		//{
		//	NoiseSprite->Draw(Pos,Vector4(1,1,1,1));
		//}
	}
}

void PlayerDamageHitUI::MakeNoise()
{
	isMakeNoise = true;
	isUpdateNoise = false;
	DrawTime = static_cast< uint32_t >( Numbers::Zero );

	//for ( auto&& Pos : NoisePos )
	//{
	//	Pos.x = MyMath::Uint32ToFloat(MyMath::Random(static_cast< uint32_t >( Numbers::Zero ),static_cast< uint32_t >( MaxSize.x )));
	//	Pos.y = MyMath::Uint32ToFloat(MyMath::Random(static_cast< uint32_t >( Numbers::Zero ),static_cast< uint32_t >( MaxSize.y )));
	//}

}
