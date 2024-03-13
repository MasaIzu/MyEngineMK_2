#pragma once

#include "DirectXCore.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "ParticleCS.h"
#include "BaseScene.h"
#include "SceneManager.h"

#include<memory>
#include<vector>
#include "GameCamera.h"
#include <CollisionManager.h>

#include "Ground.h"

#include "Player.h"
#include "LoadLevelEditor.h"

#include "TouchableObject.h"
#include "Skydome.h"
#include "ParticleEditor.h"
#include <DebugCamera.h>

/// <summary>
/// デバッグシーン
/// </summary>
class DebugScene : public BaseScene {

public: // メンバ関数

	DebugScene();
	~DebugScene();

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

	//ポストエフェクトを掛けるやつ
	void PostEffectDraw() override;

	//背景描画
	void BackgroundDraw() override;
	//CSアップデート
	void CSUpdate() override;
	//ブレイクするか
	bool IsBreak() override;

	//スロウ
	bool IsSlow() override;

private: // メンバ変数
	WinApp* winApp_ = nullptr;
	DirectXCore* dxCommon_ = nullptr;
	Input* input_ = nullptr;

	WorldTransform worldTransform_;//ワールド変換データ
	std::unique_ptr<ViewProjection> viewProjection_;//ビュープロジェクション
	std::unique_ptr<ViewProjection> LightViewProjection;//ライトビュープロジェクション
	std::unique_ptr<Ground> ground;
	std::unique_ptr<Model> model;
	std::unique_ptr<Model> model1;
	std::unique_ptr<LoadLevelEditor> levelData;
	std::unique_ptr<Player> player_;
	std::unique_ptr<DebugCamera> debugCamera;
	std::unique_ptr<Skydome> skydome;
	std::unique_ptr<ParticleEditor>particleEditor;

	int shadeNumber = 3;
	int range = 0;//ぼかし強度
	int samples = 0;
	bool isFinishGame = false;
	bool isSlowGame = false;

	uint32_t MaxFream = 60;

	float intensity = 0.1f;
	float angle = 45.0f;
	float angle2 = 135.0f;
	float cameraPos = -100.0f;

	Vector2 center = { 0.5f,0.5f };

	Vector3 CameraPos;

};