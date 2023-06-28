#pragma once
#include <ViewProjection.h>
#include <WorldTransform.h>
#include <Model.h>

class TutorialEnemy {

public://基本関数
	TutorialEnemy(const Vector3& BonePos_);
	~TutorialEnemy();

	void Initialize();
	void Update();
	void Draw(ViewProjection& viewProjection_);

private:
	//敵を見つけた時の動きと見つけてない時の動き
	void PlayerFoundMove();
	void PlayerNotFoundMove();
	//上の関数のタイマー
	void PlayerFoundMoveTimer();
	void PlayerNotFoundMoveTimer();

	//プレーヤーの移動の値更新
	void WorldTransUpdate();
private:
	std::unique_ptr<Model> model_;
	WorldTransform enemyWorldTrans;
private:
	enum class NotFoundPhase {
		Walk,//歩く
		Stop,//止まる
	};

	//フェーズ
	NotFoundPhase NotFoundPhase_ = NotFoundPhase::Walk;
private:
	bool isPlayerFound = 0;

	uint32_t WalkTime = 0;
	uint32_t StopTime = 0;

	float TerritoryRadius = 5.0f;

	Vector3 enemyMoveMent;
	Vector3 BonePos;

	

};