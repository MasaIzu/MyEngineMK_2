#include "EnemyHP2DUI.h"
#include <TextureManager.h>
#include <imgui.h>
#include <Numbers.h>

EnemyHP2DUI::EnemyHP2DUI()
{
}

EnemyHP2DUI::~EnemyHP2DUI()
{
}

void EnemyHP2DUI::Initialize()
{

	HP = Sprite::Create(TextureManager::Load("sprite/WhiteBar.png"));
	HP->SetAnchorPoint({ FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fPointFive) });
	HP->SetSize(HpSize);

	HPBackSprite = Sprite::Create(TextureManager::Load("sprite/WhiteBar.png"));
	HPBackSprite->SetAnchorPoint({ FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fPointFive) });
	HPBackSprite->SetSize(HpSize);

	HPBarBackBarSprite = Sprite::Create(TextureManager::Load("sprite/HpBarBackBar.png"));
	HPBarBackBarSprite->SetAnchorPoint({ FloatNumber(fNumbers::fZero),FloatNumber(fNumbers::fPointFive) });

	hpUpdate = std::make_unique<HpUpdate>(HpBarMaxSize);
}

void EnemyHP2DUI::Update()
{
	BackHpDownSize.x = hpUpdate->Update();
	HPBackSprite->SetSize(BackHpDownSize);
}

void EnemyHP2DUI::EnemyHpUpdate(const float& nowHp,const float& MaxHp)
{
	HpSize.x = HpBarMaxSize * ( nowHp  / MaxHp );
	HP->SetSize(HpSize);
	hpUpdate->EasingMaterial(HpSize.x);
}

void EnemyHP2DUI::Draw(const Vector2& Position)
{
	Vector2 HpBarBackBarPosition = Vector2(Position.x - HpBackBarAdjustment,Position.y);

	HPBarBackBarSprite->Draw(HpBarBackBarPosition,WhiteColor);
	HPBackSprite->Draw(Position,RedColor);
	HP->Draw(Position,WhiteColor);
}
