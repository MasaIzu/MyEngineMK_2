#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include <Sprite.h>
#include "HpUpdate.h"

class EnemyHP2DUI
{
public:
	EnemyHP2DUI();
	~EnemyHP2DUI();

	//初期化
	void Initialize();
	//更新
	void Update();
	void EnemyHpUpdate(const uint32_t& nowHp,const uint32_t& MaxHp);
	//描画
	void Draw(const Vector2& Position);

private://コンスト
	const float HpBarMaxSize = 300.0f;
private:

	std::unique_ptr<Sprite> HP;
	std::unique_ptr<Sprite> HPBackSprite;
	std::unique_ptr<Sprite> HPBarBackBarSprite;

	std::unique_ptr<HpUpdate> hpUpdate;

private:

	float YSize = 0.28f;
	float EndHpEasingPos = HpBarMaxSize;
	float HpBackBarAdjustment = 7.0f;

	Vector2 HpSize = { HpBarMaxSize,6.0f };
	Vector2 BackHpDownSize = { HpBarMaxSize,6.0f };
	Vector2 HpPosition = { 160.0f,580.0f };

	Vector2 BackBarSprite = { 20.8f,16.0f };

	Vector4 WhiteColor = { 1.0f,1.0f,1.0f,1.0f };
	Vector4 RedColor = { 1.0f,0.1f,0.1f,1.0f };
};