#pragma once
#include "DirectXCore.h"
#include "GameScene.h"
#include "TextureManager.h"
#include "WinApp.h"
#include "Input.h"
#include "FPS.h"
#include "ImGuiManager.h"
#include "SceneManager.h"
#include <memory>

class Framework {
public:
	//virtualメンバ関数
	//初期化
	virtual void Initialize();

	//更新
	virtual void Update();

	//ポストエフェクト
	virtual void PostEffectDraw()=0;

	//描画
	virtual void Draw() = 0;

	//終了処理
	virtual void Finalize();

	//終了チェック
	virtual bool isPlayMyGame();

	//コンピュートシェーダーで計算した後のコピー処理
	virtual void CopyData() = 0;


public://デストラクタ
	virtual ~Framework() = default;

public:
	//メンバ関数
	//実行
	void Run();

	WinApp* GetWinApp() { return winApp_; }
	DirectXCore* GetDirectXCore() { return directXCore_; }

protected:

	//シングルトン
	WinApp* winApp_ = nullptr;
	DirectXCore* directXCore_ = nullptr;
	Input* input_ = nullptr;
	SceneManager* sceneManager_;
	TextureManager* TextureManager_ = nullptr;

	//シングルトン以外
	std::unique_ptr<ImGuiManager> imGui;
	std::unique_ptr <FPS> fps;
	std::unique_ptr <AbstractSceneFactory> sceneFactory_;


	bool isEndRequst = false;
};