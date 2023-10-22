#include "PlayerUI.h"
#include <TextureManager.h>
#include <MyMath.h>
#include <imgui.h>
#include "Easing.h"

PlayerUI::PlayerUI()
{
}

PlayerUI::~PlayerUI()
{
}

void PlayerUI::Initialize()
{
	ReticleOutline = Sprite::Create(TextureManager::Load("sprite/circle.png"));
	ReticleOutline->SetAnchorPoint({ 0.5f,0.5f });
	ReticleOutline->SetSize(Vector2(192.0f,192.0f));

	Reticle = Sprite::Create(TextureManager::Load("sprite/ReticleV2.png"));
	Reticle->SetAnchorPoint({ 0.5f,0.5f });
	//Reticle->SetSize(Vector2(96.0f,64.0f));

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

void PlayerUI::Update()
{
	BackHpDownSize.x = hpUpdate->Update();
	HPBackSprite->SetSize(BackHpDownSize);

	HPDownBarColor = LockOnColor;
	HPDownBarColor.w = HPBarAlpha;
	HPBarColor = WhiteColor;
	HPBarColor.w = HPBarAlpha;
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


	ImGui::Begin("PlayerUI");

	ImGui::SliderFloat("HpPositionX",&HpPosition.x,0,static_cast<float>(WinApp::GetInstance()->window_width));
	ImGui::SliderFloat("HpPositionY",&HpPosition.y,0,static_cast< float >(WinApp::GetInstance()->window_height));
	ImGui::Text("HpPosition:%f,%f",HpPosition.x,HpPosition.y);

	ImGui::SliderFloat("HpSizeX",&HpSize.x,0,static_cast< float >( WinApp::GetInstance()->window_width ));
	ImGui::SliderFloat("HpSizeY",&HpSize.y,0,static_cast< float >( WinApp::GetInstance()->window_height ));
	ImGui::Text("HpSize:%f,%f",HpSize.x,HpSize.y);

	ImGui::End();

	HP->SetSize(HpSize);
}

void PlayerUI::PlayerHpUpdate(const uint32_t& nowHp,const uint32_t& MaxHp)
{
	HpSize.x = HpBarMaxSize * ( static_cast< float >( nowHp ) / static_cast< float >( MaxHp ));
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
}

void PlayerUI::SetReticlePosition(const Vector2& position)
{
	KeepReticlePosition = position;
}