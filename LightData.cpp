#include "LightData.h"

LightData* LightData::LightData_ = nullptr;

LightData* LightData::GetInstance() {
	if ( LightData_ == nullptr )
	{
		LightData_ = new LightData();
	}

	return LightData_;
}

void LightData::Destroy() {

	delete LightData_;
}

void LightData::Initialize()
{
	lightGroup = std::make_unique<LightGroup>();
	lightGroup->Initialize();
}

void LightData::Update()
{
	lightGroup->Update();
}

LightData::~LightData()
{

}

uint32_t LightData::AddPointLight(const Vector3& lightpos_,const Vector3& lightcolor_,const Vector3& lightAtten_,const bool& active_)
{
	PointLight pointLight;
	pointLight.SetLightPos(lightpos_);
	pointLight.SetLightColor(lightcolor_);
	pointLight.SetLightAtten(lightAtten_);
	pointLight.SetActive(active_);

	return lightGroup->SetPointLight(pointLight);
}

uint32_t LightData::AddSpotLight(const Vector3& lightpos_,const Vector4& lightdir_,const Vector3& lightcolor_,const Vector3& lightAtten_,const bool& active_)
{
	SpotLight spotLight;
	spotLight.SetLightPos(lightpos_);
	spotLight.SetLightDir(lightdir_);
	spotLight.SetLightColor(lightcolor_);
	spotLight.SetLightAtten(lightAtten_);
	spotLight.SetActive(active_);

	return lightGroup->SetSpotLight(spotLight);
}

uint32_t LightData::AddCircleShadow(const Vector3& casterPos_,const Vector4& dir_,const Vector3& atten_,const Vector2& factorAngle_,const bool& active_)
{
	CircleShadow circleShadow;
	circleShadow.SetCasterPos(casterPos_);
	circleShadow.SetDir(dir_);
	circleShadow.SetAtten(atten_);
	circleShadow.SetFactorAngle(factorAngle_);
	circleShadow.SetActive(active_);

	return lightGroup->SetCircleShadow(circleShadow);
}

