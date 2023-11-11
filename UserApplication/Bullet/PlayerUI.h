#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include <Sprite.h>
#include "HpUpdate.h"

class PlayerUI
{
public:
	PlayerUI();
	~PlayerUI();

	//初期化
	void Initialize(const float& playerFuel);
	//更新
	void Update(const float& nowBoost);
	void AttackReticleUpdate(const bool& LockOn);
	void PlayerHpUpdate(const uint32_t& nowHp,const uint32_t& MaxHp);
	//描画
	void Draw();

public:
	void SetReticlePosition(const Vector2& position);

private:
	const float HpBarMaxSize = 200.0f;
	const float BoostBarMaxSize = 400.0f;
private:

	std::unique_ptr<Sprite> ReticleOutline;
	std::unique_ptr<Sprite> Reticle;
	std::unique_ptr<Sprite> HP;
	std::unique_ptr<Sprite> HPBackSprite;
	std::unique_ptr<Sprite> HPBarBackBarSprite;
	std::unique_ptr<Sprite> BoostBarSprite;
	std::unique_ptr<Sprite> BoostBarBackBarSprite;

	std::unique_ptr<HpUpdate> hpUpdate;

private:

	bool isTimeAdvances = false;

	uint32_t HpDownEasingTime = 0;
	uint32_t HpDownMaxEasingTime = 60;
	uint32_t EasingTimeInAdvance = 0;
	uint32_t EasingTimeInMaxAdvance = 40;

	float HpDownBar = 0.0f;
	float StartHpEasingPos = 0.0f;
	float EndHpEasingPos = HpBarMaxSize;
	float ReticleLockLerpPower = 0.6f;
	float ReticleNotLockLerpPower = 0.4f;
	float HPBarAlpha = 1.0f;
	float HPAdjustment = 10.0f;
	float BoostAdjustment = 17.0f;
	float ReeticleOutLineSize = 192.0f;
	float AnchorPointOnePointFive = 0.5f;
	float MaxBoostFuel = 0.0f;

	Vector2 ReticlePosition = { 640,360 };
	Vector2 KeepReticlePosition;
	Vector2 ReticleNormalPosition = { 640,360 };
	Vector2 BoostBarPosition = { 640 - (BoostBarMaxSize / 2),620 };
	Vector2 BoostBarBackBarPosition = { 632 - ( BoostBarMaxSize / 2 ),620 };
	Vector2 HpPosition = { 140.0f,580.0f };
	Vector2 HpBarBackBarPosition = { 135.0f,580.0f };
	Vector2 HpSize = { HpBarMaxSize,6.0f };
	Vector2 BackHpDownSize = { HpBarMaxSize,6.0f };
	Vector2 BoostFuelSize = { HpBarMaxSize,6.0f };

	Vector4 WhiteColor = { 1.0f,1.0f,1.0f,1.0f };
	Vector4 ReticleColor;
	Vector4 LockOnColor = { 1.0f,0.1f,0.1f,1.0f };
	Vector4 NotLockOnColor = { 1.0f,1.0f,1.0f,1.0f };
	Vector4 HPDownBarColor;
	Vector4 HPBarColor;

};