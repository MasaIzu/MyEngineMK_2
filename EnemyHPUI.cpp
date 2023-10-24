#include "EnemyHPUI.h"
#include <TextureManager.h>
#include <imgui.h>

EnemyHPUI::EnemyHPUI()
{
}

EnemyHPUI::~EnemyHPUI()
{
}

void EnemyHPUI::Initialize(Sprite3D* hpSprite,Sprite3D* hpBackSprite,Sprite3D* hpBarBackBarSprite)
{
	HP = hpSprite;
	HP->SetAnchorPoint(Vector2(0.0f,0.5f));
	HPBackSprite = hpBackSprite;
	HPBackSprite->SetAnchorPoint(Vector2(0.0f,0.5f));
	HPBarBackBarSprite = hpBarBackBarSprite;
	HPBarBackBarSprite->SetAnchorPoint(Vector2(0.0f,0.5f));
	hpUpdate = std::make_unique<HpUpdate>(HpMax);
}

void EnemyHPUI::Update()
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

void EnemyHPUI::EnemyHpUpdate(const uint32_t& nowHp,const uint32_t& MaxHp)
{
	HpSize = HpMax * ( static_cast< float >( nowHp ) / static_cast< float >( MaxHp ) );
	hpUpdate->EasingMaterial(HpSize);
}

void EnemyHPUI::Draw(const Vector3& Position,const ViewProjection& viewProjection)
{
	HPBarBackBarSprite->Draw(Position,Vector4(1,1,1,1),viewProjection);
	HPBackSprite->Draw(Position,RedColor,viewProjection);
	HP->Draw(Position,Vector4(1,1,1,1),viewProjection);
}
