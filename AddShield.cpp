#include "AddShield.h"
#include <ParticleEditor.h>

AddShield::AddShield()
{
	ShildModel.reset(Model::CreateFromOBJ("Manshon",true));
}

AddShield::~AddShield()
{
}

void AddShield::Initialize()
{
	WorldTransform shieldPos;
	ShieldsTrans.push_back(shieldPos);

	colCount.push_back(ParticleEditor::AddCollision(shieldPos.translation_,shieldPos.scale_));

	count = static_cast< uint32_t >( colCount.size() - 1 );

	isChange = true;
}

void AddShield::Update(const Vector3& colPos,const Vector3& colScale)
{
	if ( isChange )
	{
		ShieldsTrans[ count ].translation_ = colPos;
		ShieldsTrans[ count ].scale_ = colScale;
		ParticleEditor::ChangeCollision(colCount[ count ],colPos,colScale);
	}
}

void AddShield::Draw(const ViewProjection& ViewProjection_,const ViewProjection& LightViewProjection_)
{
	for ( auto&& trans : ShieldsTrans )
	{
		ShildModel->Draw(trans,ViewProjection_,LightViewProjection_);
	}
}

void AddShield::ShadowDraw(const ViewProjection& lightViewProjection_)
{
	for ( auto&& trans : ShieldsTrans )
	{
		ShildModel->ShadowDraw(trans,lightViewProjection_);
	}
}

void AddShield::SetFin()
{
	isChange = false;
}

