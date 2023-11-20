#pragma once
#include "Sprite.h"
#include "Vector2.h"
#include "Vector4.h"
#include "WinApp.h"

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

public://Setter

public://Getter

private://コンスト
	static const uint32_t MaxNoiseCount = 20;
	const uint32_t MaxDrawTime = 20;
private://クラス関連
	std::unique_ptr<Sprite> NoiseSprite;


private://イーナムクラス

private://別クラスから値をもらう

private://クラス変数

	bool isMakeNoise = false;
	bool isUpdateNoise = false;

	uint32_t DrawTime = 0;

	Vector2 MaxSize;
	//std::array<Vector2,MaxNoiseCount> NoisePos;

};