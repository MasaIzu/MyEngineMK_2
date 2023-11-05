#pragma once
#include "Vector3.h"
#include "Model.h"
#include "WorldTransform.h"
#include <Input.h>
#include "SplinePosition.h"

/// <summary>
/// ボスクラス
/// </summary>
class BossEnemy {

public://基本関数
	BossEnemy();
	~BossEnemy();

	//初期化
	void Initialize(const Vector3& Pos, ViewProjection* viewProjection);
	//更新
	void Update();
	//描画
	void Draw();


	//演出用動き
	void StagingUpdate();

private://関数
	
	void WorldTransUpdate();//移動の値更新

public://Setter
	//ボスのステージ移動のスプライン
	void SetStageMoveSpline(const std::vector<Vector3>& points) { MoveSpline->SetNotSplineVector(points); }

public://Getter
	//指定したは所にいるか
	bool GetHowReturnSpline(const uint32_t& HowIndex)const { return MoveSpline->GetHowReturnIndex(HowIndex); }
	//スプラインは終わったのか
	bool GetFinishSpline()const { return MoveSpline->GetFinishSpline(); }
	//アルファは下がりきったか
	bool GetBodyNoAlpha()const;
	//ポジションはどこか
	Vector3 GetSplinePos()const { return MoveSpline->GetFinalSplineVecPoint(); }

private://const関連
	static const uint32_t BossEnemyBodyCount = 20;

private://クラス関連
	Input* input_ = nullptr;
	std::unique_ptr<Model> model_;
	WorldTransform BossWorldTrans[BossEnemyBodyCount];

	WorldTransform DebugWorldTrans;
	ViewProjection* viewProjection_ = nullptr;

	//スプライン
	std::unique_ptr<SplinePosition> MoveSpline;//途中のスプライン

private://イーナムクラス


private://別クラスから値をもらう


private://EnemyBossクラス変数

	float EnemySplineUpdate = 0.015f;
	float MaxScale = 10.0f;
	float LerpStrength = 0.1f;
	float DownAlpha = 0.015f;
	float AddDownAlpha = 0.0006f;
	float BobyAppha = 0.15f;

	Vector3 BonePos;

};