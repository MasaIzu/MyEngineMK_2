#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

class HpUpdate
{
public:
	HpUpdate(const float& MaxHp);
	~HpUpdate();


	//初期化
	void EasingMaterial(const float& end);
	void EasingMaterial(const float& end,const uint32_t& timeInMaxAdvance,const uint32_t& MaxEasingTime);
	//更新
	float Update();

private:
	bool isFirstBoot = true;
	bool isTimeAdvances = false;

	uint32_t HpDownEasingTime = 0;
	uint32_t HpDownMaxEasingTime = 60;
	uint32_t EasingTimeInAdvance = 0;
	uint32_t EasingTimeInMaxAdvance = 40;

	float StartHpEasingPos = 0.0f;
	float EndHpEasingPos = 0.0f;
	float BackHpDownSize = 0.0f;

};