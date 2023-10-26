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
	HPBackSprite = Sprite::Create(TextureManager::Load("sprite/WhiteBar.png"));
	HPBarBackBarSprite = Sprite::Create(TextureManager::Load("sprite/HpBarBackBar.png"));

	HP->SetAnchorPoint(Vector2(0.0f,0.5f));
	HPBackSprite->SetAnchorPoint(Vector2(0.0f,0.5f));
	HPBarBackBarSprite->SetAnchorPoint(Vector2(0.0f,0.5f));
	hpUpdate = std::make_unique<HpUpdate>(HpMax);
}

void EnemyHP2DUI::Update()
{
	BackHpDownSize = hpUpdate->Update();

	HPBarBackBarSprite->SetScale(BackBarSprite,BackBarSprite);
	HPBackSprite->SetScale(Vector2(BackHpDownSize,Scale.y),MaxScale);
	HP->SetScale(Vector2(HpSize,Scale.y),MaxScale);

	ImGui::Begin("EnemyUI");

	ImGui::SliderFloat("ScaleX",&Scale.x,0,MaxScale.x);
	ImGui::SliderFloat("ScaleY",&Scale.y,0,MaxScale.y);
	ImGui::Text("Scale:%f,%f",Scale.x,Scale.y);

	ImGui::End();
}

void EnemyHP2DUI::EnemyHpUpdate(const uint32_t& nowHp,const uint32_t& MaxHp)
{
	HpSize = HpMax * ( static_cast< float >( nowHp ) / static_cast< float >( MaxHp ) );
	hpUpdate->EasingMaterial(HpSize);
}

void EnemyHP2DUI::Draw(const Vector3& Position)
{
	HPBarBackBarSprite->Draw(Position,Vector4(1,1,1,1));
	HPBackSprite->Draw(Position,RedColor);
	HP->Draw(Position,Vector4(1,1,1,1));
}
