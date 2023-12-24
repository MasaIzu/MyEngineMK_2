#include "SerialNumber.h"
#include <math.h>
#include <cmath>
#include <imgui.h>

SerialNumber::SerialNumber()
{
}

SerialNumber::~SerialNumber()
{
}

void SerialNumber::Initialize(const uint32_t& makeCount)
{
	DiscriminantCount = makeCount;
	HpSprite = std::vector<std::vector<std::unique_ptr<Sprite>>>(makeCount);
	for ( uint32_t i = 0; i < DiscriminantCount; i++ )
	{
		std::vector<std::unique_ptr<Sprite>> NumberSprite;
		FirstPos = { ReferencePointPos.x - ( Territory * i ),ReferencePointPos.y };
		for ( uint32_t j = 0; j < SpriteNumberCount; j++ )
		{
			NumberSprite.push_back(Sprite::Create(NumbersSprite::GetInstance()->GetSpriteTexCount(j)));
			NumberSprite[ j ]->SetSize(SpriteSize);
			HpSprite[ i ].push_back(std::move(NumberSprite[ j ]));
		}
		SpritePos.push_back(FirstPos);
		Number.push_back(0);
	}
}

void SerialNumber::Update(const float& nowNumber)
{
	Discriminant(nowNumber);

	ImGui::Begin("HPSPRITE");

	ImGui::SliderFloat("ReferencePointPosX",&ReferencePointPos.x,0,1000);
	ImGui::SliderFloat("ReferencePointPosY",&ReferencePointPos.y,0,1000);
	ImGui::SliderFloat("SpriteSizeX",&SpriteSize.x,0,100);
	ImGui::SliderFloat("SpriteSizeY",&SpriteSize.y,0,100);
	ImGui::SliderFloat("Territory",&Territory,0,100);
	ImGui::End();

	for ( uint32_t i = 0; i < DiscriminantCount; i++ )
	{
		for ( uint32_t j = 0; j < SpriteNumberCount; j++ )
		{
			HpSprite[ i ][ j ]->SetSize(SpriteSize);
		}
		SpritePos[ i ] = { ReferencePointPos.x - ( Territory * i ),ReferencePointPos.y };
	}
}

void SerialNumber::Draw()
{
	for ( uint32_t i = 0; i < DiscriminantCount; i++ )
	{
		for ( uint32_t j = 0; j < SpriteNumberCount; j++ )
		{
			if ( Number[i] == j )
			{
				HpSprite[ i ][ j ]->Draw(SpritePos[ i ],White);
				break;
			}
		}
	}
}

void SerialNumber::Discriminant(const float& nowNumber)
{
	for ( uint32_t i = 0; i < DiscriminantCount; i++ )
	{
		Number[i] = ReturnNumber(nowNumber,i);
	}
}

uint32_t SerialNumber::ReturnNumber(const float& nowNumber,const uint32_t& place)
{
	uint32_t number = static_cast< uint32_t >( nowNumber / std::pow(10,place) );

	return number % 10;
}
