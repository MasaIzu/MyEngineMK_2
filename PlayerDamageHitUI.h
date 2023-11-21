#pragma once
#include "Sprite.h"
#include "Vector2.h"
#include "Vector4.h"
#include "WinApp.h"
#include <array>

class PlayerDamageHitUI
{

public://基本関数
	PlayerDamageHitUI( );
	~PlayerDamageHitUI( );

	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();

	//追加関数
	void MakeNoise();

private://メンバ関数
	//再抽選
	Vector2 RePosition();

public://Setter

public://Getter

private://コンスト
	static const uint32_t MaxNoiseCount = 4;
	const uint32_t MaxDrawTime = 20;

	const float SpriteAlphaMax = 1.0f;

private://クラス関連
	std::array <std::unique_ptr<Sprite>,MaxNoiseCount> DrawSprite;

private://イーナムクラス

private://別クラスから値をもらう

private://クラス変数

	bool isMakeNoise = false;
	bool isUpdateNoise = false;
	bool isCleaningFinish = false;

	uint32_t DrawTime = 0;
	uint32_t RandMinAlphaSize = 8;
	uint32_t RandMaxAlphaSize = 33;

	float SpriteRatio = 0.5f;
	float AlphaDiv = 100.0f;
	float CleanAlpha = 0.05f;

	Vector2 SpriteSizeMin;
	Vector2 SpriteSizeMax;
	Vector2 NotDrawPosMin = { 300,100 };
	Vector2 NotDrawPosMax;
	Vector2 MaxSize;
	
	std::array<bool,MaxNoiseCount> isAlphaDownFinish;
	std::array<bool,MaxNoiseCount> isCleaning;
	std::array<float,MaxNoiseCount> SpriteAlpha;
	std::array<float,MaxNoiseCount> OperationWidth;
	std::array<Vector2,MaxNoiseCount>SpriteSize;
	std::array<Vector2,MaxNoiseCount> NoisePos;

};