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

	DieOutLineSprite = Sprite::Create(TextureManager::Load("sprite/DownOutLine.png"));
	DieOutLineSprite->SetRatio(DieOutLineRatioX,DieOutLineRatioY);

	DestroySprite = Sprite::Create(TextureManager::Load("sprite/Destroy.png"));

	serialNumber = std::make_unique<SerialNumber>();
	serialNumber->Initialize(4);

	hpUpdate = std::make_unique<HpUpdate>(HpBarMaxSize);
	playerOperationUI = std::make_unique<PlayerOperationUI>();
	playerOperationUI->Initialize();


	MaxBoostFuel = playerFuel;

	BoostFuelSize.x = BoostBarMaxSize;
	BoostBarSprite->SetSize(BoostFuelSize);
	BoostBarBackBarSprite->SetSizeX(BoostFuelSize.x + BoostAdjustment);
}

void PlayerUI::Update(const float& nowBoost,const bool& isAlive)
{
	BackHpDownSize.x = hpUpdate->Update();
	HPBackSprite->SetSize(BackHpDownSize);
	HPDownBarColor = LockOnColor;
	HPDownBarColor.w = HPBarAlpha;
	HPBarColor = WhiteColor;
	HPBarColor.w = HPBarAlpha;


	BoostFuelSize.x = BoostBarMaxSize * ( nowBoost / MaxBoostFuel );
	BoostBarSprite->SetSize(BoostFuelSize);

	if ( !isAlive )
	{
		if ( isMaxRatio )
		{
			if ( DieUIDisplayTime < DieUIMaxDisplayTime )
			{
				DieUIDisplayTime++;
			}
			else
			{
				if ( DieOutLineRatioYMinRatio < DieOutLineRatioY )
				{
					DieOutLineRatioY -= SubRatio;
					DieAlpha -= SubAlpha;
				}
				else
				{
					isPlayerDieDisplay = false;
					isDieDirection = true;
				}
			}
		}
		else
		{
			if ( DieOutLineRatioY < DieOutLineRatioYMaxRatio )
			{
				isPlayerDieDisplay = true;
				DieOutLineRatioY += AddRatio;
				DieAlpha += AddRatio;
			}
			else
			{
				isMaxRatio = true;
			}
		}
		DestroyColor = { DieRed ,DieGreen,DieBlue,DieAlpha };
		DieOutLineSprite->SetRatio(DieOutLineRatioX,DieOutLineRatioY);
		DestroySprite->SetRatio(DieOutLineRatioX,DieOutLineRatioY);
	}
	playerOperationUI->Update();
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
	serialNumber->Update(nowHp);
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
	playerOperationUI->Draw();
	serialNumber->Draw();
	if ( isPlayerDieDisplay )
	{
		DieOutLineSprite->Draw(DieBackLinePos,WhiteColor);
		DestroySprite->Draw(DieBackLinePos,DestroyColor);
	}
}

void PlayerUI::SetReticlePosition(const Vector2& position)
{
	KeepReticlePosition = position;
}

bool PlayerUI::GetIsDieDirection()
{
	return isDieDirection;
}
