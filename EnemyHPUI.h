#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include <Sprite.h>
#include "HpUpdate.h"
#include <Sprite3D.h>

class EnemyHPUI
{
public:
	EnemyHPUI();
	~EnemyHPUI();

	//初期化
	void Initialize(Sprite3D* hpSprite,Sprite3D* hpBackSprite,Sprite3D* hpBarBackBarSprite);
	//更新
	void Update();
	void EnemyHpUpdate(const uint32_t& nowHp,const uint32_t& MaxHp);
	//描画
	void Draw(const Vector3& Position,const ViewProjection& viewProjection);

private://コンスト
	const float HpMax = 20.0f;

private:

	Sprite3D* HP = nullptr;
	Sprite3D* HPBackSprite = nullptr;
	Sprite3D* HPBarBackBarSprite = nullptr;

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