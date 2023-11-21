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
	MaxSize = WinApp::GetInstance()->GetWindowSize();

	NotDrawPosMax = Vector2(MaxSize.x - NotDrawPosMin.x,MaxSize.y - NotDrawPosMin.y);

	for ( auto&& sprite : DrawSprite )
	{
		sprite = Sprite::Create(TextureManager::Load("sprite/yokoNoise.png"));
	}
	SpriteSizeMax = DrawSprite[ 0 ]->GetSize();
	SpriteSizeMin = SpriteSizeMax * SpriteRatio;
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
		isCleaningFinish = true;
		if ( DrawTime < MaxDrawTime )
		{
			DrawTime++;
			for ( uint32_t i = 0; i < MaxNoiseCount; i++ )
			{
				if ( isAlphaDownFinish[ i ] == false )
				{
					if ( 0.0f < SpriteAlpha[ i ] )
					{
						SpriteAlpha[ i ] -= OperationWidth[ i ];
					}
					else
					{
						isAlphaDownFinish[ i ] = true;
					}
				}
				else
				{
					if ( SpriteAlpha[ i ] < 1.0f )
					{
						SpriteAlpha[ i ] += OperationWidth[ i ];
					}
					else
					{
						isAlphaDownFinish[ i ] = false;
					}
				}
			}
		}
		else
		{
			for ( uint32_t i = 0; i < MaxNoiseCount; i++ )
			{
				if ( isCleaning[ i ] == false )
				{
					isCleaning[ i ] = true;
					SpriteAlpha[ i ] -= CleanAlpha;
				}
				else
				{
					if ( 0.0f < SpriteAlpha[ i ] )
					{
						isCleaningFinish = false;
						SpriteAlpha[ i ] -= CleanAlpha;
					}
				}
			}
			if ( isCleaningFinish )
			{
				isUpdateNoise = false;
			}
		}
	}
}

void PlayerDamageHitUI::Draw()
{
	if ( isUpdateNoise )
	{
		for ( uint32_t i = 0; i < MaxNoiseCount;i++ )
		{
			DrawSprite[ i ]->Draw(NoisePos[ i ],Vector4(1,1,1,SpriteAlpha[ i ]),2);
		}
	}
}

void PlayerDamageHitUI::MakeNoise()
{
	isMakeNoise = true;
	isUpdateNoise = false;
	DrawTime = static_cast< uint32_t >( Numbers::Zero );

	Vector2 Pos;

	for ( uint32_t i = 0; i < MaxNoiseCount; i++ )
	{
		isAlphaDownFinish[ i ] = false;

		Pos.x = MyMath::Uint32ToFloat(MyMath::Random(static_cast< uint32_t >( Numbers::Zero ),static_cast< uint32_t >( MaxSize.x )));
		Pos.y = MyMath::Uint32ToFloat(MyMath::Random(static_cast< uint32_t >( Numbers::Zero ),static_cast< uint32_t >( MaxSize.y )));

		SpriteSize[ i ].x = MyMath::Uint32ToFloat(MyMath::Random(static_cast< uint32_t >( SpriteSizeMin.x ),static_cast< uint32_t >( SpriteSizeMax.x )));
		SpriteSize[ i ].y = MyMath::Uint32ToFloat(MyMath::Random(static_cast< uint32_t >( SpriteSizeMin.y ),static_cast< uint32_t >( SpriteSizeMax.y )));

		SpriteAlpha[ i ] = SpriteAlphaMax;
		OperationWidth[ i ] = MyMath::Uint32ToFloat(MyMath::Random(RandMinAlphaSize,RandMaxAlphaSize)) / AlphaDiv;

		if ( (NotDrawPosMin.x < Pos.x && Pos.x < NotDrawPosMax.x) &&
			(NotDrawPosMin.y < Pos.y && Pos.y < NotDrawPosMax.y) )
		{
			Pos = RePosition();
		}
		NoisePos[ i ] = Pos;
		DrawSprite[ i ]->SetSize(SpriteSize[ i ]);
	}

}

Vector2 PlayerDamageHitUI::RePosition()
{
	Vector2 Pos;
	Pos.x = MyMath::Uint32ToFloat(MyMath::Random(static_cast< uint32_t >( Numbers::Zero ),static_cast< uint32_t >( MaxSize.x )));
	Pos.y = MyMath::Uint32ToFloat(MyMath::Random(static_cast< uint32_t >( Numbers::Zero ),static_cast< uint32_t >( MaxSize.y )));

	if ( ( NotDrawPosMin.x < Pos.x && Pos.x < NotDrawPosMax.x ) &&
			( NotDrawPosMin.y < Pos.y && Pos.y < NotDrawPosMax.y ) )
	{
		Pos = RePosition();
	}

	return Pos;
}
