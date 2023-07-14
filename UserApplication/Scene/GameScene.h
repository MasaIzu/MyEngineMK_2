#pragma once

#include "DirectXCore.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "affin.h"
#include"ParticleManager.h"

#include "BaseScene.h"
#include "SceneManager.h"

#include "FbxModel.h"
#include<memory>
#include<vector>
#include "GameCamera.h"
#include <CollisionManager.h>

#include "Ground.h"

#include "Player.h"
#include "TutorialEnemy.h"
#include "LoadLevelEditor.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public BaseScene {

public: // メンバ関数

	GameScene();
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;
	// 終了処理
	void Finalize() override;
	//コピー処理
	void CopyData() override;
	//ポストエフェクトを掛けるやつ
	void PostEffectDraw() override;

	bool CheckReticle();

private: // メンバ変数
	WinApp* winApp_ = nullptr;
	DirectXCore* dxCommon_ = nullptr;
	Input* input_ = nullptr;


	
	std::unique_ptr<ViewProjection> viewProjection_;//ビュープロジェクション
	std::unique_ptr<Model> model_;// 3Dモデル
	WorldTransform worldTransform_;//ワールド変換データ
	uint32_t textureHandle_ = 0;//テクスチャハンドル

	int shadeNumber = 3;

	//ぼかし強度
	int range = 0;


	Vector2 center = {0.5f,0.5f};
	float intensity = 0.1f;
	int samples = 5;

	float angle = 45.0f;
	float angle2 = 135.0f;

	float cameraSraide = 0.0f;

	std::unique_ptr<ParticleManager> ParticleMan;

	//当たり判定
	CollisionManager* collisionManager = nullptr;
	//シーンマネージャー
	SceneManager* sceneManager_ = nullptr;

	Vector3 Pos = { 0, 0, -25 };
	Vector3 Verocty = { 0, 0.1f, 0 };
	uint32_t MaxFream = 60;

	std::unique_ptr<GameCamera> gameCamera;

	std::unique_ptr<Ground> ground;

	std::unique_ptr<Player> player_;

	std::unique_ptr<TutorialEnemy> tutorialEnemy;
	std::unique_ptr<LoadLevelEditor> levelData;

	float dist;
	float radius;
	Vector2 tmp;

	Vector3 EnemyPos;
};