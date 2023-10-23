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
}

void EnemyHPUI::Update()
{
	ImGui::Begin("EnemyUI");

	ImGui::SliderFloat("ScaleX",&Scale.x,0,MaxScale.x);
	ImGui::SliderFloat("ScaleY",&Scale.y,0,MaxScale.y);
	ImGui::Text("Scale:%f,%f",Scale.x,Scale.y);

	ImGui::End();
}



void EnemyHPUI::Draw(const Vector3& Position,const float& nowHp,const float& maxHp,const ViewProjection& viewProjection)
{
	HPBarBackBarSprite->SetScale(maxHp);
	HPBackSprite->SetScale(nowHp);
	HP->SetScale(nowHp);


	HPBarBackBarSprite->SetScale(BackBarSprite,BackBarSprite);
	HPBackSprite->SetScale(Vector2(Scale.x,Scale.y),MaxScale);
	HP->SetScale(Vector2(Scale.x,Scale.y),MaxScale);

	HPBarBackBarSprite->Draw(Position,Vector4(1,1,1,1),maxHp,viewProjection);
	HPBackSprite->Draw(Position,Vector4(1,1,1,1),maxHp,viewProjection);
	HP->Draw(Position,Vector4(1,1,1,1),maxHp,viewProjection);
}
