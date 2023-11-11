#pragma once
#include "Vector3.h"
#include "WorldTransform.h"
#include <Input.h>
#include "PlayerStruct.h"

class PlayerMovement
{

public://基本関数
	PlayerMovement();
	~PlayerMovement();

	//Move
	Vector3 Move(const WorldTransform& worldTransform,const bool& onGround);
	//ブースト
	Vector3 UpBoost(const bool& onGround);
	//Angle
	void PlayerAngle(const bool& isAtack);
	//スライディングマテリアル
	bool SlidingMaterial(const WorldTransform& worldTransform);
	//スライディングアップデート
	Vector3 SlidingUpdate(float& slidingSpeed,const float& maxSlidingSpeed,const uint32_t& Step);

private://メンバ関数

	//アングル
	void PlayerAngleSetter(const float& angle);

	//どっちにスライディングしたのか
	void CheckSliding();

	//ブースト後の燃料アップデート
	void BoostFuelUpdate(const bool& onGround);

public://Setter

public://Getter
	bool GetIsRotFinish();
	bool GetIsPushMoveKey();
	bool GetIsBoost();
	float GetPlayerAngle();
	float GetFuel();
	PushKey GetPushedKey();
private://クラス関連
	Input* input_ = nullptr;

	PlayerMoveRot playerMoveRot;
	PushKey pushKey;
private://コンスト
	const float FallAcc = -0.035f;
	const float FallVYMin = -1.0f;

	const float UpAcc = 0.0003f;
	const float UpVYMax = 0.7f;
	const float FuelMax = 1000.0f;
private://別クラスから値をもらう

private://クラス変数
	bool isRotFinish = false;
	bool isPlayerAttack = false;
	bool isBoost = false;
	bool isBoostCoolTimeFinish = false;
	bool isBoostSuccess = false;

	uint32_t SlidingNumber = 0;
	uint32_t BoostCoolTime = 0;
	uint32_t MaxBoostCoolTime = 90;

	float PlayerMoveRotation = 0.0f;
	float PlayerTrunSpeed = 0.4f;
	float UpBoostSpeed = 0.0f;
	float FallSpeed = 0.0f;
	float Fuel = FuelMax;
	float FuelUsedBoost = 200.0f;
	float FuelUsedUpBoost = 5.0f;
	float FuelRecoveryOnGround = 10.0f;
	float FuelRecoveryInTheSky = 1.0f;

	Vector3 playerAllMoveMent;//移動量
	Vector3 playerSlidingMoveMent;//スライディング移動量
	Vector3 DirectionOfMovement;
	Vector3 FallVec;

};