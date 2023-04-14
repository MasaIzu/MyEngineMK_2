#pragma once
#include "Model.h"
#include "WorldTransform.h"
///<summary>
///風
/// </summary>
class Wing {
public:
	///<summary>
	///初期化
	/// </summary>
	void Initialize(Model* model);

	///<summary>
	///更新
	/// </summary>
	void Update(Vector3 pos);

	///<summary>
	///描画
	/// </summary>
	void Draw(ViewProjection viewProjection);

	//判定
	void Distance();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_;
	//時間とフラグ
	bool make;
	int time;
};