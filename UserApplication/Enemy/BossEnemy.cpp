#include "BossEnemy.h"

BossEnemy::BossEnemy()
{
}

BossEnemy::~BossEnemy()
{

}

void BossEnemy::Initialize(const Vector3& Pos, ViewProjection* viewProjection)
{
	BonePos = Pos;
	viewProjection_ = viewProjection;
	for (uint32_t i = 0; i < BossEnemyBodyCount; i++) {
		BossWorldTrans[i].Initialize();
		BossWorldTrans[i].translation_ = BonePos;
		float Scale = MaxScale - (MaxScale / BossEnemyBodyCount * static_cast<float>(i));
		BossWorldTrans[i].scale_ = Vector3(Scale, Scale, Scale);
	}
	
	WorldTransUpdate();

	model_.reset(Model::CreateFromOBJ("sphereNormalEnemy", true));

	MoveSpline = std::make_unique<SplinePosition>();
}

void BossEnemy::Update()
{
	
}

void BossEnemy::Draw()
{
	for (uint32_t i = 0; i < BossEnemyBodyCount; i++) {
		model_->Draw(BossWorldTrans[i], *viewProjection_);
	}
}

void BossEnemy::CSUpdate()
{
}

void BossEnemy::ParticleDraw()
{
}

void BossEnemy::CopyParticle()
{
}

void BossEnemy::StagingUpdate()
{
	MoveSpline->Update(EnemySplineUpdate);
	BossWorldTrans[0].translation_ = MoveSpline->NowPos;
	for (uint32_t i = 1; i < BossEnemyBodyCount; i++) {
		BossWorldTrans[i].translation_ = Vector3::lerp(BossWorldTrans[i].translation_, BossWorldTrans[i - 1].translation_, 0.1f);
	}

	if (GetFinishSpline()) {
		for (uint32_t i = 0; i < BossEnemyBodyCount; i++) {
			if (BossWorldTrans[i].alpha > 0) {
				BossWorldTrans[i].alpha += -0.015f + (static_cast<float>(i) * 0.0006f);
			}
		}
	}

	WorldTransUpdate();
}

void BossEnemy::WorldTransUpdate()
{
	for (uint32_t i = 0; i < BossEnemyBodyCount; i++) {
		BossWorldTrans[i].TransferMatrix();
	}
}

bool BossEnemy::GetBodyNoAlpha() const
{
	if (BossWorldTrans[BossEnemyBodyCount - 1].alpha <= 0.15f) {
		return true;
	}
	return false;
}
