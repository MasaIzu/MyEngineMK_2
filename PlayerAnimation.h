#pragma once
#include "PlayerEnum.h"
#include "WorldTransform.h"
#include <FbxModel.h>
#include <FBXObject3d.h>

class PlayerAnimation
{

public://基本関数
	PlayerAnimation();
	~PlayerAnimation();

	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void FbxDraw(const WorldTransform& worldTransform,const ViewProjection& viewProjection_);

private://メンバ関数
	

public://Setter

	void SetAnimation(const uint32_t& animNumber,const uint32_t& startCount,const uint32_t& maxCount,const bool& loop);

public://Getter
	

private://クラス関連
	std::unique_ptr<FBXModel> fbxModel_;
	std::unique_ptr<FBXObject3d> fbxObj3d_;

private://イーナムクラス

	

private://別クラスから値をもらう
	

private://プレイヤークラス変数

	bool isLoop = false;
	bool isStart = false;

	uint32_t AnmNumNumber_ = 7;
	uint32_t nowAnmFCount_ = 0;
	uint32_t maxFcount = 25;

};