#include "EnemyHP3DUI.h"
#include <TextureManager.h>
#include <imgui.h>

EnemyHP3DUI::EnemyHP3DUI()
{
}

EnemyHP3DUI::~EnemyHP3DUI()
{
}

void EnemyHP3DUI::Initialize()
{

	HP = Sprite3D::Create(TextureManager::Load("sprite/WhiteBar.png"));
	HPBackSprite = Sprite3D::Create(TextureManager::Load("sprite/WhiteBar.png"));
	HPBarBackBarSprite = Sprite3D::Create(TextureManager::Load("sprite/HpBarBackBar.png"));

	HP->SetAnchorPoint(Vector2(0.0f,0.5f));
	HPBackSprite->SetAnchorPoint(Vector2(0.0f,0.5f));
	HPBarBackBarSprite->SetAnchorPoint(Vector2(0.0f,0.5f));
	hpUpdate = std::make_unique<HpUpdate>(HpMax);
}

void EnemyHP3DUI::Update()
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

void EnemyHP3DUI::EnemyHpUpdate(const uint32_t& nowHp,const uint32_t& MaxHp)
{
	HpSize = HpMax * ( static_cast< float >( nowHp ) / static_cast< float >( MaxHp ) );
	hpUpdate->EasingMaterial(HpSize);
}

void EnemyHP3DUI::Draw(const Vector3& Position,const ViewProjection& viewProjection)
{
	HPBarBackBarSprite->Draw(Position,Vector4(1,1,1,1),viewProjection);
	HPBackSprite->Draw(Position,RedColor,viewProjection);
	HP->Draw(Position,Vector4(1,1,1,1),viewProjection);
}
