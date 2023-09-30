#include "MiddleBossEnemy.h"

MiddleBossEnemy::MiddleBossEnemy()
{
	MovieUpdateTimes = 120.0f;
	model_.reset(Model::CreateFromOBJ("sphereNormalEnemy", true));
	BossWorldTrans.scale_ = Vector3(5.0f, 5.0f, 5.0f);
	BossWorldTrans.Initialize();
}

MiddleBossEnemy::~MiddleBossEnemy()
{
}

void MiddleBossEnemy::Initialize(const Vector3& Pos)
{
}

void MiddleBossEnemy::Update()
{
}

void MiddleBossEnemy::Draw(ViewProjection& viewProjection_)
{
	if (isSporn) {
		model_->Draw(BossWorldTrans, viewProjection_);
	}
}

void MiddleBossEnemy::MovieUpdate(const Vector3& StartPos, Vector3& EndPos)
{
	if (isStart == false) {
		isStart = true;
		isSporn = true;
		BossWorldTrans.translation_ = StartPos;
		this->EndPos = EndPos;
		Vector3 startToEnd = EndPos - StartPos;
		Velocity = startToEnd / MovieUpdateTimes;
	}
	else {
		if (MovieUpdateTimes > 0.0f) {

			MovieUpdateTimes -= 1.0f;
			BossWorldTrans.translation_ += Velocity;
		}
		else {

		}
	}

	WorldTransUpdate();
}

void MiddleBossEnemy::WorldTransUpdate()
{
	BossWorldTrans.TransferMatrix();
}
