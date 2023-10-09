#pragma once

#include "DirectXCore.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "BaseScene.h"
#include "SceneManager.h"

#include<memory>
#include<vector>
#include "GameCamera.h"
#include <CollisionManager.h>

#include "Ground.h"

#include "Player.h"
#include "TutorialEnemy.h"
#include "BulletShotEnemy.h"
#include "LoadLevelEditor.h"

#include "TouchableObject.h"
#include <Skydome.h>

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
	//CSアップデート
	void CSUpdate() override;

	bool IsBreak() override;

	bool CheckReticle();

private: // メンバ変数
	WinApp* winApp_ = nullptr;
	DirectXCore* dxCommon_ = nullptr;
	Input* input_ = nullptr;

	std::unique_ptr<ViewProjection> viewProjection_;//ビュープロジェクション
	std::unique_ptr<Model> model_;// 3Dモデル
	WorldTransform worldTransform_;//ワールド変換データ
	CollisionManager* collisionManager = nullptr;//当たり判定
	SceneManager* sceneManager_ = nullptr;//シーンマネージャー
	std::unique_ptr<GameCamera> gameCamera;
	std::unique_ptr<Ground> ground;
	std::unique_ptr<Player> player_;
	//std::unique_ptr<TutorialEnemy> tutorialEnemy;
	std::list<BulletShotEnemy*> bulletShotEnemy;
	std::unique_ptr<LoadLevelEditor> levelData;
	std::list<TutorialEnemy*> tutorialEnemyList;
	std::unique_ptr<TouchableObject> touchableObject;
	std::unique_ptr<Skydome> skydome;

	uint32_t textureHandle_ = 0;//テクスチャハンドル

	bool isSpline = false;
	bool isFinish = false;
	bool isFinishGame = false;

	int shadeNumber = 3;
	int range = 0;//ぼかし強度
	int samples = 5;

	uint32_t MaxFream = 60;

	float intensity = 0.1f;
	float angle = 45.0f;
	float angle2 = 135.0f;
	float cameraSraide = 0.0f;
	float dist = 0.0f;
	float radius = 0.0f;
	float larpTime = 0.0f;

	Vector2 center = {0.5f,0.5f};
	Vector2 tmp;

	Vector3 Pos = { 0, 0, -25 };
	Vector3 Verocty = { 0, 0.1f, 0 };
	Vector3 EnemyPos;
	Vector3 eye;
};