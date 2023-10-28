#include "EnemyHP2DUI.h"
#include <TextureManager.h>
#include <imgui.h>

EnemyHP2DUI::EnemyHP2DUI()
{
}

EnemyHP2DUI::~EnemyHP2DUI()
{
}

void EnemyHP2DUI::Initialize()
{

	HP = Sprite::Create(TextureManager::Load("sprite/WhiteBar.png"));
	HP->SetAnchorPoint({ 0.0f,0.5f });
	HP->SetSize(HpSize);

	HPBackSprite = Sprite::Create(TextureManager::Load("sprite/WhiteBar.png"));
	HPBackSprite->SetAnchorPoint({ 0.0f,0.5f });
	HPBackSprite->SetSize(HpSize);

	HPBarBackBarSprite = Sprite::Create(TextureManager::Load("sprite/HpBarBackBar.png"));
	HPBarBackBarSprite->SetAnchorPoint({ 0.0f,0.5f });

	hpUpdate = std::make_unique<HpUpdate>(HpBarMaxSize);
}

void EnemyHP2DUI::Update()
{
	BackHpDownSize.x = hpUpdate->Update();
	HPBackSprite->SetSize(BackHpDownSize);
}

void EnemyHP2DUI::EnemyHpUpdate(const uint32_t& nowHp,const uint32_t& MaxHp)
{
	HpSize.x = HpBarMaxSize * ( static_cast< float >( nowHp ) / static_cast< float >( MaxHp ) );
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
