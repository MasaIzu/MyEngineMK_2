#pragma once
#include "Sprite.h"

class ClearUI
{

public://基本関数
	ClearUI();
	~ClearUI();

	//初期化
	void Initialize();
	//更新
	bool Update();
	//描画
	void Draw();

private://メンバ関数

public://Setter

public://Getter

private://クラス関連
	std::unique_ptr<Sprite> ClearSprite;

private://イーナムクラス

private://別クラスから値をもらう

private://クラス変数

	bool isFinishEasing = false;

	uint32_t EasingTime = 0;
	uint32_t MaxEasingTime = 150;

	float StartPos = -500.0f;
	float EndPos = 200.0f;
	float SpriteX = StartPos;
	float SpriteY = 120.0f;

};