#pragma once
#include "Model.h"
#include "WorldTransform.h"
///<summary>
///道路
/// </summary>
class BackGround {
public:
	///<summary>
	///初期化
	/// </summary>
	void Initialize(Model* modelLeft, Model* modelRight);

	///<summary>
	///更新
	/// </summary>
	void Update(float speed);

	///<summary>
	///描画
	/// </summary>
	void Draw(ViewProjection viewProjection);

	/// <summary>
	/// タイトル画面用
	/// </summary>
	void Demo();

private:
	//ワールド変換データ
	WorldTransform worldTransformRight_[2];
	WorldTransform worldTransformLeft_[2];
	//モデル
	Model* modelRight_;
	Model* modelLeft_;

	float speed_;
};