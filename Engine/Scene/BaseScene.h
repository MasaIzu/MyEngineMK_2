#pragma once
#include "WinApp.h"
#include"DirectXCore.h"

class SceneManager;

//シーン基底クラス
class BaseScene {

public:
	//virtualメンバ関数
	//初期化
	virtual void Initialize() = 0;

	//更新
	virtual void Update() = 0;

	virtual void PostEffectDraw()=0;

	//描画
	virtual void Draw() = 0;

	//描画
	virtual void Finalize() = 0;

public://メンバ関数

	virtual ~BaseScene() = default;

	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; };


private:
	//シーンマネージャー
	SceneManager* sceneManager_ = nullptr;

};