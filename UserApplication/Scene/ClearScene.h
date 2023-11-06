#pragma once
#include "BaseScene.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "Input.h"
#include "GameScene.h"
#include "SceneManager.h"


// タイトルシーン
class ClearScene : public BaseScene
{
public: // メンバ関数

	ClearScene() = default;
	~ClearScene() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	//ポストエフェクト
	void PostEffectDraw() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;


	//CSアップデート
	void CSUpdate() override;

	//ブレイクするか
	bool IsBreak() override;

private:
	Input* input = nullptr;
	SceneManager* sceneManager_ = nullptr;
	std::unique_ptr<Sprite> sprite_ = nullptr;
	std::unique_ptr<Sprite> Result = nullptr;
	std::unique_ptr<Sprite> ResultDetail = nullptr;

	bool isFinishGame = false;
	bool isSceneChange = false;

	float SpriteAlpha = 1.0f;
	float AddSpriteAlpha = 0.01f;
	float DownSpriteAlpha = 0.01f;
};

