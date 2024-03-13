#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <assert.h>
#include "ViewProjection.h"


/// <summary>
/// 天球
/// </summary>
class Skydome {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection_,const ViewProjection& LightViewProjection_);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	WorldTransform worldTransformG_;
	//モデル
	std::unique_ptr<Model> model_;
	std::unique_ptr<Model> modelG_;

	float Size = 1000.0f;

};