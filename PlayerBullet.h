#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "Input.h"
#include "affin.h"
#include <memory>
#include "ViewProjection.h"


class PlayerBullet {

public://基本関数
	PlayerBullet();
	~PlayerBullet();

	void Initialize();
	void Update();
	void Draw(ViewProjection& viewProjection_);

private:
	//プレーヤーの移動
	void BulletMove();

	//打ち出す球を作る
	void MakePlayerBullet();

public://Getter
	Vector3 GetPlayerPos(const uint32_t& bulletCount)const { return MyMath::GetWorldTransform(playerBulletWorldTrans[bulletCount].matWorld_); }

public://Setter


private://const関連
	static const uint32_t AllBulletCount = 5;


private://クラス関連
	std::unique_ptr<Model> model_;
	WorldTransform playerBulletWorldTrans[AllBulletCount];

private://別クラスから値をもらう


private://プレイヤークラス変数
	bool isBulletAlive[AllBulletCount];

	float playerBulletSpeed = 1.0f;
	Vector3 playerBulletMoveMent;//移動量


};