#include "PlayerBullet.h"

PlayerBullet::PlayerBullet()
{
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		isBulletAlive[i] = 0;
	}
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Initialize()
{

	model_.reset(Model::CreateFromOBJ("sphere", true));
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		playerBulletWorldTrans[i].Initialize();
		playerBulletWorldTrans[i].scale_ = { 0.4f,0.4f,0.4f };
		playerBulletWorldTrans[i].TransferMatrix();

		isBulletAlive[i] = 0;
	}


}

void PlayerBullet::Update()
{
	BulletMove();
}

void PlayerBullet::Draw(ViewProjection& viewProjection_)
{
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		if (isBulletAlive[i] == true) {
			model_->Draw(playerBulletWorldTrans[i], viewProjection_);
		}
	}
}

void PlayerBullet::BulletMove()
{

	for (uint32_t i = 0; i < AllBulletCount; i++) {
		if (isBulletAlive[i] == true) {
			playerBulletWorldTrans[i].translation_ = { 0,0,0 };
		}
	}

}

void PlayerBullet::MakePlayerBullet()
{
	for (uint32_t i = 0; i < AllBulletCount; i++) {
		if (isBulletAlive[i] == false) {
			isBulletAlive[i] = true;
			break;
		}
	}
}
