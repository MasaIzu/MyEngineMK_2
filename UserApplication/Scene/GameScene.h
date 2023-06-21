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

#include"FbxAnimation.h"
#include "FbxLoader.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public BaseScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	void PostEffectDraw() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	// 終了処理
	void Finalize() override;

	//コピー処理
	void CopyData() override;

private: // メンバ変数
	WinApp* winApp_ = nullptr;
	DirectXCore* dxCommon_ = nullptr;
	Input* input_ = nullptr;


	//ビュープロジェクション
	std::unique_ptr<ViewProjection> viewProjection_;
	//ワールド変換データ
	WorldTransform worldTransform_;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// 3Dモデル
	std::unique_ptr<Model> model_;


	//FBX関係
	float frem = 0;
	std::unique_ptr<FbxModel>fbxModel;
	std::unique_ptr<FbxAnimation>anim;
	FbxLoader* fbxLoad = nullptr;

	int shadeNumber = 0;

	//ぼかし強度
	int range = 10;


	Vector2 center = {0.5f,0.5f};
	float intensity = 0.1f;
	int samples = 5;

	std::unique_ptr<ParticleManager> ParticleMan;

	//当たり判定
	CollisionManager* collisionManager = nullptr;
	//シーンマネージャー
	SceneManager* sceneManager_ = nullptr;

	Vector3 Pos = { 0, -20, 0 };
	Vector3 Verocty = { 0, 0.1f, 0 };
	uint32_t MaxFream = 300;

	std::unique_ptr<GameCamera> gameCamera;

};