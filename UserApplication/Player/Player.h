#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "Input.h"
#include "affin.h"
#include <memory>
#include "ViewProjection.h"
#include "PlayerBullet.h"

class Player {

public://基本関数
	Player();
	~Player();

	void Initialize();
	void Update();
	void Draw(ViewProjection& viewProjection_);
	
private:
	//プレーヤーの移動
	void Move();
	//プレイヤーの回転
	void PlayerRot();
	//プレーヤーの移動の値更新
	void WorldTransUpdate();

public://GetterSetter
	void SetCameraRot(const float& CameraRot) { cameraRot = CameraRot; }

public:
	Vector3 GetPlayerPos()const { return MyMath::GetWorldTransform(playerWorldTrans.matWorld_); }

private://クラス関連
	Input* input_ = nullptr;
	std::unique_ptr<Model> model_;
	WorldTransform playerWorldTrans;
	std::unique_ptr<PlayerBullet> playerBullet;

private://プレイヤークラス変数
	float playerSpeed = 1.0f;
	Vector3 playerMoveMent;//移動量


private://別クラスから値をもらう
	float cameraRot;

};