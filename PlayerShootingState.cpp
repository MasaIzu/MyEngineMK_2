#include "PlayerShootingState.h"
#include "Player.h"
#include "PlayerNormalState.h"

void PlayerShootingState::Initialize()
{
}

void PlayerShootingState::Update(Player* player,const Vector3& EnemyPos)
{
	player->NormalGunShoot(EnemyPos);
	//player->PlayerMove();

	player->TransitionTo(new PlayerNormalState);
}
