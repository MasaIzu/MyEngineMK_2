#pragma once

#include "DirectXCore.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "affin.h"
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
#include <Skydome.h>
#include <ParticleHandHanabi.h>
#include <Explosion.h>
#include <Hibana.h>


/// <summary>
/// ゲームシーン
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
	//コピー処理
	void CopyData() override;
	//ポストエフェクトを掛けるやつ
	void PostEffectDraw() override;
	//CSアップデート
	void CSUpdate() override;

private: // メンバ変数
	WinApp* winApp_ = nullptr;
	DirectXCore* dxCommon_ = nullptr;
	Input* input_ = nullptr;

	WorldTransform worldTransform_;//ワールド変換データ
	std::unique_ptr<ViewProjection> viewProjection_;//ビュープロジェクション
	std::unique_ptr<GameCamera> gameCamera;
	std::unique_ptr<Ground> ground;
	std::unique_ptr<Model> model;
	std::unique_ptr<Model> model1;

	int shadeNumber = 3;
	int range = 0;//ぼかし強度
	int samples = 5;

	uint32_t MaxFream = 60;

	float intensity = 0.1f;
	float angle = 45.0f;
	float angle2 = 135.0f;


	Vector2 center = { 0.5f,0.5f };

	Vector3 CameraPos;

	double x;     // 紐を伸ばして一周させた場合に出来る円周上の座標、０は紐が軸の真下にいる位置
	double speed; // xの速度
	double Angle;
	float    jiku_x = 320, jiku_y = 100; // 軸の位置
	float nx, ny;

};