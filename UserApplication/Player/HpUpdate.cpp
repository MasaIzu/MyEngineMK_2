#include "HpUpdate.h"
#include <Easing.h>
#include <Numbers.h>

HpUpdate::HpUpdate(const float& MaxHp)
{
	BackHpDownSize = MaxHp;
}

HpUpdate::~HpUpdate()
{
}

void HpUpdate::EasingMaterial(const float& end)
{
	HpDownEasingTime = static_cast< uint32_t >( Numbers::Zero );;
	EasingTimeInAdvance = static_cast< uint32_t >( Numbers::Zero );;
	EndHpEasingPos = end;
	isTimeAdvances = false;
	if ( isFirstBoot )
	{
		isFirstBoot = false;
	}
}

void HpUpdate::EasingMaterial(const float& end,const uint32_t& timeInMaxAdvance,const uint32_t& MaxEasingTime)
{
	HpDownEasingTime = static_cast< uint32_t >( Numbers::Zero );
	HpDownMaxEasingTime = MaxEasingTime;
	EasingTimeInAdvance = static_cast< uint32_t >( Numbers::Zero );
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
