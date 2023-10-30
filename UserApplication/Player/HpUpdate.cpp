#include "HpUpdate.h"
#include <Easing.h>

HpUpdate::HpUpdate(const float& MaxHp)
{
	BackHpDownSize = MaxHp;
}

HpUpdate::~HpUpdate()
{
}

void HpUpdate::EasingMaterial(const float& end)
{
	HpDownEasingTime = 0;
	EasingTimeInAdvance = 0;
	EndHpEasingPos = end;
	isTimeAdvances = false;
	if ( isFirstBoot )
	{
		isFirstBoot = false;
	}
}

void HpUpdate::EasingMaterial(const float& end,const uint32_t& timeInMaxAdvance,const uint32_t& MaxEasingTime)
{
	HpDownEasingTime = 0;
	HpDownMaxEasingTime = MaxEasingTime;
	EasingTimeInAdvance = 0;
	EasingTimeInMaxAdvance = timeInMaxAdvance;

	EndHpEasingPos = end;
	isTimeAdvances = false;
	if ( isFirstBoot )
	{
		isFirstBoot = false;
	}
}

float HpUpdate::Update()
{
	if ( !isFirstBoot )
	{
		if ( isTimeAdvances == false )
		{
			if ( EasingTimeInAdvance < EasingTimeInMaxAdvance )
			{
				EasingTimeInAdvance++;
			}
			else
			{
				StartHpEasingPos = BackHpDownSize;
				isTimeAdvances = true;
			}
		}
		if ( isTimeAdvances )
		{
			if ( HpDownEasingTime < HpDownMaxEasingTime )
			{
				HpDownEasingTime++;
				BackHpDownSize = Easing::EaseOutQuart(StartHpEasingPos,EndHpEasingPos,HpDownEasingTime,HpDownMaxEasingTime);
				return BackHpDownSize;
			}
		}
	}
	return BackHpDownSize;
}
