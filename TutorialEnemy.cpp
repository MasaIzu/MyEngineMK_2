#include "TutorialEnemy.h"

TutorialEnemy::TutorialEnemy()
{
}

TutorialEnemy::~TutorialEnemy()
{
}

void TutorialEnemy::Initialize()
{

	model_.reset(Model::CreateFromOBJ("sphereColor", true));
	enemyWorldTrans.Initialize();

}

void TutorialEnemy::Update()
{
	if (isPlayerFound) {

	}
	else {

	}

}

void TutorialEnemy::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(enemyWorldTrans, viewProjection_);
}

void TutorialEnemy::PlayerFoundMove()
{

}

void TutorialEnemy::PlayerNotFoundMove()
{

}
