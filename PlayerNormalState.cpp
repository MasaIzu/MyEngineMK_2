#include"PlayerNormalState.h"
#include"Player.h"

//#include"PlayerSwordAttack.h"
#include"PlayerShootingState.h"
#include"PlayerMissileShootingState.h"
#include <Numbers.h>

void PlayerNormalState::Initialize() {
	PlayerState::Initialize();
	canMove_ = true;
	stateName = "NORMAL";
}

void PlayerNormalState::Update(Player* player,const Vector3& EnemyPos) {
	//攻撃
	if ( input_->MouseInputing(static_cast< int >( Numbers::Zero )) || input_->ButtonInput(LT) )
	{
		player->TransitionTo(new PlayerShootingState);
	}
	else if ( input_->MouseInputTrigger(static_cast< int >( Numbers::One )) || input_->ButtonInput(RT) )
	{
		player->TransitionTo(new PlayerShootingState);
	}
	else if ( input_->PushKey(DIK_E) || input_->PButtonTrigger(LB) )
	{
		player->TransitionTo(new PlayerMissileShootingState);
	}

	//移動クラス
	std::unique_ptr<PlayerMovement> playerMovement = std::make_unique<PlayerMovement>();
	player->PlayerMove(playerMovement->Move(player->GetStateMaterial()));
}