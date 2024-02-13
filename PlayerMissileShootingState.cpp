#include "PlayerMissileShootingState.h"
#include "Player.h"
#include "PlayerNormalState.h"

void PlayerMissileShootingState::Initialize()
{
}

void PlayerMissileShootingState::Update(Player* player,const Vector3& EnemyPos)
{
	player->MissileGunShoot(EnemyPos);
	//player->PlayerMove();

	player->TransitionTo(new PlayerNormalState);
}
