#include "PlayerShootingState.h"
#include "Player.h"
#include "PlayerNormalState.h"


void PlayerShootingState::Initialize()
{
}

void PlayerShootingState::Update(Player* player,PlayerMovement* playerMovement)
{
	//移動クラス
	player->PlayerMove(playerMovement->Move(player->GetStateMaterial()));

	player->TransitionTo(new PlayerNormalState);
}

void PlayerShootingState::AttackUpdate([[maybe_unused]] Player* player,[[maybe_unused]] PlayerMovement* playerMovement,[[maybe_unused]] const Vector3& EnemyPos)
{
	
}
