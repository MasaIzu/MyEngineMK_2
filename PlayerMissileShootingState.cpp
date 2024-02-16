#include "PlayerMissileShootingState.h"
#include "Player.h"
#include "PlayerNormalState.h"

void PlayerMissileShootingState::Initialize()
{
}

void PlayerMissileShootingState::Update(Player* player,PlayerMovement* playerMovement)
{
	//移動クラス
	player->PlayerMove(playerMovement->Move(player->GetStateMaterial()));

	player->TransitionTo(new PlayerNormalState);
}

void PlayerMissileShootingState::AttackUpdate(Player* player,PlayerMovement* playerMovement,const Vector3& EnemyPos)
{
}
