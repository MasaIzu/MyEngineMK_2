#pragma once
#include"PlayerState.h"
#include"PlayerMovement.h"
//待機
class PlayerNormalState : public PlayerState
{
public:
	void Initialize()override;
	void Update(Player* player,PlayerMovement* playerMovement) override;
	void AttackUpdate([[maybe_unused]] Player* player,[[maybe_unused]] PlayerMovement* playerMovement,[[maybe_unused]] const Vector3& EnemyPos) override;
private:

};