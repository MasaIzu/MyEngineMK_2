#pragma once
#include <ViewProjection.h>
#include <WorldTransform.h>
#include <Model.h>

class TutorialEnemy {

public://äÓñ{ä÷êî
	TutorialEnemy();
	~TutorialEnemy();

	void Initialize();
	void Update();
	void Draw(ViewProjection& viewProjection_);

private:
	void PlayerFoundMove();
	void PlayerNotFoundMove();
private:
	std::unique_ptr<Model> model_;
	WorldTransform enemyWorldTrans;

private:
	bool isPlayerFound = 0;
	Vector3 enemyMoveMent;



};