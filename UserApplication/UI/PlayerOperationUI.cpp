#include "PlayerOperationUI.h"
#include <TextureManager.h>
#include <imgui.h>
#include "Input.h"

PlayerOperationUI::PlayerOperationUI()
{
}

PlayerOperationUI::~PlayerOperationUI()
{
}

void PlayerOperationUI::Initialize()
{
	AttackUISprite = Sprite::Create(TextureManager::Load("sprite/Attack.png"));
	AttackUISprite->SetSize(UISize);

	AttackMissileUISprite = Sprite::Create(TextureManager::Load("sprite/Missile.png"));
	AttackMissileUISprite->SetSize(UISize);

	BoostUISprite = Sprite::Create(TextureManager::Load("sprite/Boost.png"));
	BoostUISprite->SetSize(UISize);

	OrUISprite = Sprite::Create(TextureManager::Load("sprite/or.png"));
	OrUISprite->SetSize(OrUISize);

	MouseLeftSprite = Sprite::Create(TextureManager::Load("sprite/MousLeft.png"));
	MouseLeftSprite->SetSize(MousUISize);
	MouseRightSprite = Sprite::Create(TextureManager::Load("sprite/MousRight.png"));
	MouseRightSprite->SetSize(MousUISize);

	EKeySprite = Sprite::Create(TextureManager::Load("sprite/Ekey.png"));
	EKeySprite->SetSize(EkeySize);

	ShiftKeySprite = Sprite::Create(TextureManager::Load("sprite/Shift.png"));
	ShiftKeySprite->SetSize(EkeySize);

	UpBoostSprite = Sprite::Create(TextureManager::Load("sprite/UpBoost.png"));
	UpBoostSprite->SetSize(UISize);

	SpaceSprite = Sprite::Create(TextureManager::Load("sprite/Spacekey.png"));
	SpaceSprite->SetSize(SpacekeySize);

	XBotanSprite = Sprite::Create(TextureManager::Load("sprite/XBotan.png"));
	XBotanSprite->SetSize(EkeySize);
	ABotanSprite = Sprite::Create(TextureManager::Load("sprite/ABotan.png"));
	ABotanSprite->SetSize(EkeySize);
	L1Sprite = Sprite::Create(TextureManager::Load("sprite/L1.png"));
	L1Sprite->SetSize(EkeySize);
	L2Sprite = Sprite::Create(TextureManager::Load("sprite/L2.png"));
	L2Sprite->SetSize(EkeySize);
	R2Sprite = Sprite::Create(TextureManager::Load("sprite/R2.png"));
	R2Sprite->SetSize(EkeySize);

}

void PlayerOperationUI::Update()
{

}

void PlayerOperationUI::Draw()
{
	if ( Input::GetInstance()->GetIsControllerConnection() )
	{
		AttackUISprite->Draw(AttackUIPos,WhiteColor);
		AttackMissileUISprite->Draw(MissileUIPos,WhiteColor);
		BoostUISprite->Draw(BoostUIPos,WhiteColor);
		UpBoostSprite->Draw(UpBoostUIPos,WhiteColor);

		XBotanSprite->Draw(XBotanPos,WhiteColor);
		ABotanSprite->Draw(ABotanPos,WhiteColor);

		L1Sprite->Draw(EkeyPos,WhiteColor);
		L2Sprite->Draw(MousLeftUIPos,WhiteColor);
		R2Sprite->Draw(MousRightUIPos,WhiteColor);
		OrUISprite->Draw(OrUIPos,WhiteColor);
	}
	else
	{
		AttackUISprite->Draw(AttackUIPos,WhiteColor);
		AttackMissileUISprite->Draw(MissileUIPos,WhiteColor);
		BoostUISprite->Draw(BoostUIPos,WhiteColor);
		UpBoostSprite->Draw(UpBoostUIPos,WhiteColor);

		MouseLeftSprite->Draw(MousLeftUIPos,WhiteColor);
		MouseRightSprite->Draw(MousRightUIPos,WhiteColor);
		OrUISprite->Draw(OrUIPos,WhiteColor);
		EKeySprite->Draw(EkeyPos,WhiteColor);
		ShiftKeySprite->Draw(ShiftkeyPos,WhiteColor);
		SpaceSprite->Draw(SpaceUIPos,WhiteColor);
	}
}
