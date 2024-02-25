#include "ClearUI.h"
#include <TextureManager.h>
#include "Easing.h"
#include <imgui.h>

ClearUI::ClearUI()
{
}

ClearUI::~ClearUI()
{
}

void ClearUI::Initialize()
{
	ClearSprite = Sprite::Create(TextureManager::Load("sprite/MissionComplete.png"));
}

bool ClearUI::Update()
{

	if ( EasingTime < MaxEasingTime )
	{
		EasingTime++;
	}
	else
	{
		isFinishEasing = true;
	}

	SpriteX = Easing::easeOutCubic(StartPos,EndPos,EasingTime,MaxEasingTime);

	return isFinishEasing;
}

void ClearUI::Draw()
{
	ClearSprite->Draw(Vector2(SpriteX,SpriteY),Vector4(1,1,1,1));
}
