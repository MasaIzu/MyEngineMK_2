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
	void Draw(const Vector3& Position);

private://コンスト
	const float HpMax = 20.0f;

private:

	std::unique_ptr<Sprite> HP;
	std::unique_ptr<Sprite> HPBackSprite;
	std::unique_ptr<Sprite> HPBarBackBarSprite;

	std::unique_ptr<HpUpdate> hpUpdate;

private:

	float YSize = 0.28f;
	float HpSize = HpMax;
	float BackHpDownSize = 0.0f;
	Vector2 Scale = { HpMax,YSize };
	Vector2 MaxScale = { HpMax,YSize };

	Vector2 BackBarSprite = { 20.8f,16.0f };

	Vector4 RedColor = { 1.0f,0.1f,0.1f,1.0f };
};