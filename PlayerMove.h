#pragma once
#include "WorldTransform.h"
#include <Input.h>

class PlayerMove
{

public://基本関数
	PlayerMove();
	~PlayerMove();

	//Move
	void Move(const WorldTransform& worldTransform);

private://メンバ関数

public://Setter

public://Getter

private://クラス関連
	Input* input_ = nullptr;

private://イーナムクラス

private://別クラスから値をもらう

private://クラス変数

};