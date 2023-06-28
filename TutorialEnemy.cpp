#include "TutorialEnemy.h"
#include <imgui.h>

TutorialEnemy::TutorialEnemy(const Vector3& BonePos_)
{
	BonePos = BonePos_;
}

TutorialEnemy::~TutorialEnemy()
{
}

void TutorialEnemy::Initialize()
{

	model_.reset(Model::CreateFromOBJ("sphereColor", true));
	enemyWorldTrans.Initialize();
	enemyWorldTrans.translation_ = BonePos;
	WorldTransUpdate();



}

void TutorialEnemy::Update()
{
	if (isPlayerFound) {
		PlayerFoundMove();
	}
	else {
		PlayerNotFoundMove();
	}

	ImGui::Begin("NotFoundPhase");

	if (NotFoundPhase_ == NotFoundPhase::Walk) {
		ImGui::Text("Walk");
	}
	else if (NotFoundPhase_ == NotFoundPhase::Stop) {
		ImGui::Text("Stop");
	}

	ImGui::End();

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
	PlayerNotFoundMoveTimer();

	switch (NotFoundPhase_)
	{
	case TutorialEnemy::NotFoundPhase::Walk:



		//‰~‚ðì‚Á‚Äo‚È‚¢ˆ—‚ðì‚é

		break;
	case TutorialEnemy::NotFoundPhase::Stop:

		if (StopTime > 0) {

		}

		break;
	default:
		break;
	}

}

void TutorialEnemy::PlayerFoundMoveTimer()
{
}

void TutorialEnemy::PlayerNotFoundMoveTimer()
{
	if (WalkTime > 0) {
		WalkTime--;
	}
	if (StopTime > 0) {
		StopTime--;
	}
}

void TutorialEnemy::WorldTransUpdate()
{
	enemyWorldTrans.TransferMatrix();
}
