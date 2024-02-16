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

void PlayerShootingState::AttackUpdate(Player* player,PlayerMovement* playerMovement,const Vector3& EnemyPos)
{
	if ( playerMovement->GetIsRotFinish() )
	{
		player->NormalGunShoot(EnemyPos);
	}
}
