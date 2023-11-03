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
	Vector3 Move(const WorldTransform& worldTransform);
	//Angle
	void PlayerAngle(const bool& isAtack);

private://メンバ関数

	//アングル
	void PlayerAngleSetter(const float& angle);

public://Setter

public://Getter
	bool GetIsRotFinish();
	bool GetIsPushMoveKey();
	float GetPlayerAngle();
	PushKey GetPushedKey();
private://クラス関連
	Input* input_ = nullptr;

	PlayerMoveRot playerMoveRot;
	PushKey pushKey;
private://イーナムクラス

private://別クラスから値をもらう

private://クラス変数
	bool isRotFinish = false;

	float PlayerMoveRotation = 0.0f;
	float PlayerTrunSpeed = 0.4f;

	Vector3 playerAllMoveMent;//移動量

};