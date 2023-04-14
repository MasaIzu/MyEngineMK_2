#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "affin.h"



class Enemy {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Move();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection_);

	//死んでるか
	bool IsDead() const { return isDead_; }

	//向きを変える
	Vector3 bVelocity(Vector3& velocity, WorldTransform& worldTransform);
	//ワールド座標を取得
	Vector3 GetWorldPosition();


private:

	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	std::unique_ptr<Model> model_ = nullptr;
	
	
	//デスフラグ
	bool isDead_ = false;


};