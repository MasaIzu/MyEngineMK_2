#include "PlayerUI.h"
#include <TextureManager.h>
#include <MyMath.h>
#include <imgui.h>
#include "Easing.h"
#include "Numbers.h"

PlayerUI::PlayerUI()
{
}

PlayerUI::~PlayerUI()
{
}

void PlayerUI::Initialize(const float& playerFuel)
{
	ReticleOutline = Sprite::Create(TextureManager::Load("sprite/circle.png"));
	ReticleOutline->SetAnchorPoint({ AnchorPointOnePointFive,AnchorPointOnePointFive });
	ReticleOutline->SetSize(Vector2(ReeticleOutLineSize,ReeticleOutLineSize));

	Reticle = Sprite::Create(TextureManager::Load("sprite/ReticleV2.png"));
	Reticle->SetAnchorPoint({ AnchorPointOnePointFive,AnchorPointOnePointFive });

	HP = Sprite::Create(TextureManager::Load("sprite/WhiteBar.png"));
	HP->SetAnchorPoint({ FloatNumber(fNumbers::fZero),AnchorPointOnePointFive});
	HP->SetSize(HpSize);

	HPBackSprite = Sprite::Create(TextureManager::Load("sprite/WhiteBar.png"));
	HPBackSprite->SetAnchorPoint({ FloatNumber(fNumbers::fZero),AnchorPointOnePointFive });
	HPBackSprite->SetSize(HpSize);

	HPBarBackBarSprite = Sprite::Create(TextureManager::Load("sprite/HpBarBackBar.png"));
	HPBarBackBarSprite->SetAnchorPoint({ FloatNumber(fNumbers::fZero),AnchorPointOnePointFive });
	HPBarBackBarSprite->SetSizeX(HpSize.x + HPAdjustment);

	BoostBarSprite = Sprite::Create(TextureManager::Load("sprite/WhiteBar.png"));
	BoostBarSprite->SetAnchorPoint({ FloatNumber(fNumbers::fZero),AnchorPointOnePointFive });

	BoostBarBackBarSprite = Sprite::Create(TextureManager::Load("sprite/HpBarBackBar.png"));
	BoostBarBackBarSprite->SetAnchorPoint({ FloatNumber(fNumbers::fZero),AnchorPointOnePointFive });

	hpUpdate = std::make_unique<HpUpdate>(HpBarMaxSize);

	MaxBoostFuel = playerFuel;

	BoostFuelSize.x = BoostBarMaxSize;
	BoostBarSprite->SetSize(BoostFuelSize);
	BoostBarBackBarSprite->SetSizeX(BoostFuelSize.x + BoostAdjustment);
}

void PlayerUI::Update(const float& nowBoost)
{
	BackHpDownSize.x = hpUpdate->Update();
	HPBackSprite->SetSize(BackHpDownSize);

	HPDownBarColor = LockOnColor;
	HPDownBarColor.w = HPBarAlpha;
	HPBarColor = WhiteColor;
	HPBarColor.w = HPBarAlpha;


	BoostFuelSize.x = BoostBarMaxSize * ( nowBoost / MaxBoostFuel );
	BoostBarSprite->SetSize(BoostFuelSize);


	ImGui::Begin("UI");

	ImGui::SliderFloat("BoostBarPositionX",&BoostBarPosition.x,0,1280);
	ImGui::SliderFloat("BoostBarPositionY",&BoostBarPosition.y,0,720);
	ImGui::End();

}

void PlayerUI::AttackReticleUpdate(const bool& LockOn)
{
	if ( LockOn )
	{
		ReticleColor = LockOnColor;
		ReticlePosition = MyMath::lerpVec2(ReticlePosition,KeepReticlePosition,ReticleLockLerpPower);
	}
	else
	{
		ReticleColor = NotLockOnColor;
		ReticlePosition = MyMath::lerpVec2(ReticlePosition,ReticleNormalPosition,ReticleNotLockLerpPower);
	}

	HP->SetSize(HpSize);
}

void PlayerUI::PlayerHpUpdate(const float& nowHp,const float& MaxHp)
{
	HpSize.x = HpBarMaxSize * ( nowHp / MaxHp );
	HP->SetSize(HpSize);
	hpUpdate->EasingMaterial(HpSize.x);
}


void PlayerUI::Draw()
{
	ReticleOutline->Draw(ReticlePosition,WhiteColor);
	Reticle->Draw(ReticlePosition,ReticleColor);
	HPBarBackBarSprite->Draw(HpBarBackBarPosition,WhiteColor);
	HPBackSprite->Draw(HpPosition,HPDownBarColor);
	HP->Draw(HpPosition,HPBarColor);
	BoostBarBackBarSprite->Draw(BoostBarBackBarPosition,WhiteColor);
	BoostBarSprite->Draw(BoostBarPosition,WhiteColor);
}

void PlayerUI::SetReticlePosition(const Vector2& position)
{
	KeepReticlePosition = position;
}