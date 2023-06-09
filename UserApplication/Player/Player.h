#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "Input.h"
#include "affin.h"
#include <memory>
#include "ViewProjection.h"
#include "PlayerBullet.h"
#include <Sprite.h>

class Player {

public://基本関数
	Player();
	~Player();

	void Initialize();
	void Update();
	void Draw(ViewProjection& viewProjection_);
	void DrawSprite(ViewProjection& viewProjection_);

	//パーティクルを出す用
	void CSUpdate(ID3D12GraphicsCommandList* cmdList);
	void ParticleDraw(ViewProjection& viewProjection_);
	void CopyParticle();

private:
	//プレーヤーの移動
	void Move();
	//プレイヤーの回転
	void PlayerRot();
	//プレーヤーの攻撃
	void PlayerAttack();
	//プレーヤーの移動の値更新
	void WorldTransUpdate();

public://GetterSetter
	void SetCameraRot(const Vector2& CameraRot) { cameraRot = CameraRot; }

public:
	Vector3 GetPlayerPos()const { return MyMath::GetWorldTransform(playerWorldTrans.matWorld_); }

private://クラス関連
	Input* input_ = nullptr;
	std::unique_ptr<Model> model_;
	WorldTransform playerWorldTrans;
	WorldTransform playerWorldTransForBullet;
	std::unique_ptr<PlayerBullet> playerBullet;
	// 照準スプライト
	std::unique_ptr<Sprite> AttackSprite;

private://イーナムクラス
	enum class AttackPhase {
		AttackCombo1,//
		AttackCombo2,//
		AttackCombo3,//
		AttackCombo4,//
		AttackUlt,//
		Nothing,//何もしない
	};
	//見つけてないときの動きフェーズ
	AttackPhase AttackPhase_ = AttackPhase::Nothing;

private://別クラスから値をもらう
	Vector2 cameraRot;

private://プレイヤークラス変数
	uint32_t bulletNumber = 0;

	float playerSpeed = 0.5f;
	float diagonalPlayerSpeed = 0.6f;

	Vector3 playerMoveMent;//移動量
	Vector3 ReticlePos;

};