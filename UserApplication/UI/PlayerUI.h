#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include <Sprite.h>
#include "HpUpdate.h"
#include "PlayerOperationUI.h"
#include "SerialNumber.h"

class PlayerUI
{
public:
	PlayerUI();
	~PlayerUI();

	//初期化
	void Initialize(const float& playerFuel);
	//更新
	void Update(const float& nowBoost,const bool& isAlive);
	void AttackReticleUpdate(const bool& LockOn);
	void PlayerHpUpdate(const float& nowHp,const float& MaxHp);
	void GunUpdate(const float& gunBullet,const bool& isReload);
	void ExplosionGunUpdate(const float& gunBullet,const bool& isReload);
	//描画
	void Draw();

public:
	void SetReticlePosition(const Vector2& position);

	bool GetIsDieDirection();

private:
	const float HpBarMaxSize = 200.0f;
	const float BoostBarMaxSize = 400.0f;
private:

	std::unique_ptr<Sprite> ReticleOutline;
	std::unique_ptr<Sprite> Reticle;
	std::unique_ptr<Sprite> HPBar;
	std::unique_ptr<Sprite> HPBackSprite;
	std::unique_ptr<Sprite> HPBarBackBarSprite;
	std::unique_ptr<Sprite> BoostBarSprite;
	std::unique_ptr<Sprite> BoostBarBackBarSprite;
	std::unique_ptr<Sprite> DieOutLineSprite;
	std::unique_ptr<Sprite> DestroySprite;
	std::unique_ptr<Sprite> HP;
	std::unique_ptr<Sprite> NormalSprite;
	std::unique_ptr<Sprite> YokoBouSprite;
	std::unique_ptr<Sprite> YokoBouExplosionSprite;
	std::unique_ptr<Sprite> ReloadNormalSprite;
	std::unique_ptr<Sprite> ReloadExplosionSprite;
	std::unique_ptr<Sprite> ExplosionESprite;

	std::unique_ptr<SerialNumber> serialHPNumber;
	std::unique_ptr<SerialNumber> serialGunWeaponNumber;
	std::unique_ptr<SerialNumber> serialExplosionGunWeaponNumber;
	std::unique_ptr<HpUpdate> hpUpdate;
	std::unique_ptr<PlayerOperationUI> playerOperationUI;
private:

	bool isTimeAdvances = false;
	bool isPlayerDieDisplay = false;
	bool isMaxRatio = false;
	bool isDieDirection = false;
	bool isReload = false;
	bool isExplosionReload = false;

	uint32_t HpDownEasingTime = 0;
	uint32_t HpDownMaxEasingTime = 60;
	uint32_t EasingTimeInAdvance = 0;
	uint32_t EasingTimeInMaxAdvance = 40;
	uint32_t DieUIMaxDisplayTime = 60;
	uint32_t DieUIDisplayTime = 0;

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
	float DieOutLineRatioX = 1.0f;
	float DieOutLineRatioY = 0.0f;
	float DieOutLineRatioYMaxRatio = 1.0f;
	float DieOutLineRatioYMinRatio = 0.0f;
	float AddRatio = 0.06f;
	float SubRatio = 0.04f;
	float SubAlpha = 0.06f;
	float DieRed = 0.8f;
	float DieGreen = 0.8f;
	float DieBlue = 0.8f;
	float DieAlpha = 0.0f;
	float Territory = 25.0f;
	float GunWeaponTerritory = 16.0f;

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
	Vector2 DieBackLinePos = { 640,360 };
	Vector2 HPSpritePos = { 157,555 };
	Vector2 HPSpriteSize = { 45,29 };
	Vector2 ReferencePointPos = { 330,554 };
	Vector2 SpriteSize = { 28,27 };
	Vector2 GunBulletUIPos = { 1028,540 };
	Vector2 ExplosionGunBulletUIPos = { 1165,540 };
	Vector2 GunBulletUISize = { 16,16 };
	Vector2 NormalPos = { 1016,580 };
	Vector2 NormalSize = { 84,20 };
	Vector2 BouPos = { 1014,563 };
	Vector2 BouExplosionPos = { 1164,563 };
	Vector2 BouSize = { 120,1 };
	Vector2 ReloadUIPos = { 1016,540 };
	Vector2 ReloadUISize = { 111,22 };
	Vector2 ReloadExplosionUIPos = { 1164,540 };
	Vector2 ExplosionUIPos = { 1164,581 };
	Vector2 ExplosionUISize = { 110,29 };

	Vector4 WhiteColor = { 1.0f,1.0f,1.0f,1.0f };
	Vector4 RedColor = { 1.0f,0.1f,0.1f,1.0f };
	Vector4 ReticleColor;
	Vector4 LockOnColor = { 1.0f,0.1f,0.1f,1.0f };
	Vector4 NotLockOnColor = { 1.0f,1.0f,1.0f,1.0f };
	Vector4 HPDownBarColor;
	Vector4 HPBarColor;
	Vector4 DestroyColor = { 1.0f,1.0f,1.0f,1.0f };

};