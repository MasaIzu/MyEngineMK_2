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
#include "BoxCollision.h"
#include "GameCamera.h"
#include <CollisionManager.h>
#include "DebugCamera.h"

#include"FbxAnimation.h"


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



private: // メンバ変数
	WinApp* winApp_ = nullptr;
	DirectXCore* dxCommon_ = nullptr;
	Input* input_ = nullptr;


	//ビュープロジェクション
	ViewProjection viewProjection_;
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


	std::unique_ptr<DebugCamera>debugCamera;


	Vector3 ambientColor;
	Vector3 diffuseColor;
	Vector3 specularColor;
	Vector3 rimColor;

	float minThreshold = 0.0f;
	float maxThreshold = 1.0f;
	float rimLightPow = 10.0f;

	//当たり判定
	CollisionManager* collisionManager = nullptr;
	//シーンマネージャー
	SceneManager* sceneManager_ = nullptr;
};